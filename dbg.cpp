#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "no_sal2.h"
#include "pal_char16.h"
#include "pal_mstypes.h"
#include "cordebug.h"
#include "managed_callback.h"


#define DLL_PROCESS_ATTACH 1
typedef HRESULT (*CreateCordbObject)(int iDebuggerVersion, ICorDebug **ppCordb);
typedef BOOL (*DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
typedef int (*PAL_Initialize)(int argc, const char *);
typedef HMODULE (*LoadLibraryA)(LPCSTR lpLibFileName);
typedef void *(*GetProcAddress) (HMODULE hModule, LPCSTR lpProcName);

void *load_pal()
{
    const char *name = "libmscordaccore.so";
    void *lib = dlopen(name, RTLD_LAZY | RTLD_GLOBAL);
    if (lib == nullptr) 
    {
        printf("Can't load library %s\n", name);
        return nullptr;
    }

    PAL_Initialize palInit = (PAL_Initialize)dlsym(lib, "PAL_Initialize");
    if (palInit == nullptr) 
    {
        printf("Can't find PAL_Initialize in library %s\n", name);
        return nullptr;
    }

    int palInitResult = palInit(0, nullptr);
    if (palInitResult != 0)
    {
        printf("PAL_Initialize returned error %d\n", palInitResult);
        return nullptr;        
    }

    return lib;   
}


int main(int argc, const char **args)
{
    printf("Hi, I'm a process %d\n", getpid());

    void *palLib = load_pal();
    LoadLibraryA loadLibraryA = (LoadLibraryA)dlsym(palLib, "LoadLibraryA");
    GetProcAddress getProcAddress = (GetProcAddress)dlsym(palLib, "GetProcAddress");

    HMODULE dbi_lib = loadLibraryA("libmscordbi.so");

    CreateCordbObject createCordbObject = (CreateCordbObject)getProcAddress(dbi_lib, "CreateCordbObject");
    if (createCordbObject == nullptr) 
    {
        printf("createCordbObject == nullptr\n");
        return 1;
    }
    else
    {
        printf("createCordbObject=%p\n", createCordbObject);
    }    

    ICorDebug *pCordb;
    HRESULT hr = createCordbObject(3, &pCordb);

    printf("Cordebug created hr=%X pCordb=%p\n", (int)hr, pCordb);

    hr = pCordb->Initialize();
    printf("Cordebug initialized hr=%X\n", (int)hr);

    ManagedCallback *callback = new ManagedCallback();
    hr = pCordb->SetManagedHandler(callback);
    printf("SetManagedHandler hr=%X\n", (int)hr);

    ICorDebugProcess *process = nullptr;
    if (argc > 1) 
    {
        int pid = atoi(args[1]);
        printf("Attaching to pid %d\n", pid);
        hr = pCordb->DebugActiveProcess((DWORD)pid, FALSE, &process);
        printf("DebugActiveProcess hr=%X\n", (int)hr);
    }
    else 
    {
        STARTUPINFOW si = {0};
        si.cb = sizeof(si);
        si.dwFlags = 0;

        PROCESS_INFORMATION pi = {0};
        hr = pCordb->CreateProcess( 
                 NULL,
                 (LPWSTR)u"/home/eugene/projects/coreclr/binaries/Product/linux.x64.debug/corerun /home/eugene/projects/coreclr/binaries/Product/linux.x64.debug/HelloWorld.exe",
                 NULL,
                 NULL,
                 FALSE,
                 0,
                 NULL,
                 NULL,
                 &si,
                 &pi,
                 DEBUG_NO_SPECIAL_OPTIONS,
                 &process);
        printf("CreateProcess hr=%X\n", (int)hr);
    }

    printf("<press any key>");
    getchar();

    return 0;
}
