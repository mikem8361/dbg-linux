#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "no_sal2.h"
#include "pal_char16.h"
#include "pal_mstypes.h"
#include "rpc.h"
#include "rt/ntimage.h"
#include "unknwn.h"
#include "cor.h"
#include "cordebug.h"

char *to_ascii(char16_t *src)
{
    char *result = (char *)src;
    char *dst = result;
    while (*src)
    {
        *dst++ = (char)*src++;
    }
    *dst = 0;
    return result;
}


#include "metadata.h"
#include "managed_callback.h"



#define DLL_PROCESS_ATTACH 1
typedef HRESULT (*CreateCordbObject)(int iDebuggerVersion, ICorDebug **ppCordb);
typedef BOOL (*DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
typedef int (*PAL_InitializeDLL)();
typedef int (*PAL_Initialize)(int argc, const char * const argv[]);
typedef int (*PAL_InitializeCoreCLR)(const char *szExePath, const char *szCoreCLRPath, BOOL fStayInPAL);
typedef HMODULE (*LoadLibraryA)(LPCSTR lpLibFileName);
typedef void *(*GetProcAddress) (HMODULE hModule, LPCSTR lpProcName);
typedef HRESULT (*CreateVersionStringFromModule)(DWORD pidDebuggee, LPCWSTR szModuleName, LPWSTR pBuffer, DWORD cchBuffer, DWORD* pdwLength);
typedef HRESULT (*CreateDebuggingInterfaceFromVersion)(LPCWSTR szDebuggeeVersion, IUnknown ** ppCordb);

void *load_pal()
{
    const char *name = "libcoreclrpal.so";
    void *lib = dlopen(name, RTLD_LAZY | RTLD_GLOBAL);
    if (lib == nullptr) 
    {
        printf("Can't load library %s\n", name);
        return nullptr;
    }

    PAL_InitializeDLL palInitDll = (PAL_InitializeDLL)dlsym(lib, "PAL_InitializeDLL");
    PAL_Initialize palInit = (PAL_Initialize)dlsym(lib, "PAL_Initialize");
    PAL_InitializeCoreCLR palInitCoreClr = (PAL_InitializeCoreCLR)dlsym(lib, "PAL_InitializeCoreCLR");
    if (palInit == nullptr) 
    {
        printf("Can't find PAL_Initialize in library %s\n", name);
        return nullptr;
    }

    int palInitResult = palInit(0, 0);
    if (palInitResult != 0)
    {
        printf("PAL_Initialize returned error %d\n", palInitResult);
        return nullptr;        
    }

    /*
    if (palInitCoreClr("dbg", "/home/eugene/projects/coreclr/bin/Product/Linux.x64.Debug/libcoreclr.so", TRUE) == 0) 
    {
        printf("Can't find PAL_InitializeCoreCLR in library %s\n", name);
        return nullptr;
    }*/

    return lib;   
}



int main(int argc, const char **args)
{
    printf("Hi, I'm a process %d\n", getpid());

    void *palLib = load_pal();
    LoadLibraryA loadLibraryA = (LoadLibraryA)dlsym(palLib, "LoadLibraryA");
    GetProcAddress getProcAddress = (GetProcAddress)dlsym(palLib, "GetProcAddress");

    HMODULE shim_lib = loadLibraryA("libdbgshim.so");

    CreateVersionStringFromModule createVersionStringFromModule = (CreateVersionStringFromModule)getProcAddress(shim_lib, "CreateVersionStringFromModule");
    if (createVersionStringFromModule == nullptr)
    {
        printf("createVersionStringFromModule == nullptr\n");
        return 1;
    }
    CreateDebuggingInterfaceFromVersion createDebuggingInterfaceFromVersion = (CreateDebuggingInterfaceFromVersion)getProcAddress(shim_lib, "CreateDebuggingInterfaceFromVersion");
    if (createDebuggingInterfaceFromVersion == nullptr)
    {
        printf("createDebuggingInterfaceFromVersion == nullptr\n");
        return 1;
    }    

    if (argc < 1) 
    {
        printf("No pid has been given in arguments\n");
        return 1;
    }    

    int pid = atoi(args[1]);
    printf("Attaching to pid %d\n", pid);

    WCHAR verStr[1000];
    DWORD verLen;
    HRESULT hr = createVersionStringFromModule(pid, u"libcoreclr.so", verStr, sizeof(verStr)/sizeof(verStr[0]), &verLen);
    printf("CreateVersionStringFromModule hr=%X\n", (int)hr);

    ICorDebug *pCordb = nullptr;
    hr = createDebuggingInterfaceFromVersion(verStr, (IUnknown **)&pCordb);
    printf("CreateVersionStringFromModule hr=%X pCordb=%p\n", (int)hr, pCordb);    

    if (hr != 0 || pCordb == nullptr)
    {
        printf("Can't create ICorDebug!\n");
        return 1;
    }

    hr = pCordb->Initialize();
    printf("Cordebug initialized hr=%X\n", (int)hr);

    ManagedCallback *callback = new ManagedCallback();
    hr = pCordb->SetManagedHandler(callback);
    printf("SetManagedHandler hr=%X\n", (int)hr);    

    ICorDebugProcess *process = nullptr;
    printf("Attaching to pid %d\n", pid);
    hr = pCordb->DebugActiveProcess((DWORD)pid, FALSE, &process);
    printf("DebugActiveProcess hr=%X\n", (int)hr);

    printf("<press any key>");
    getchar();

    return 0;
}
