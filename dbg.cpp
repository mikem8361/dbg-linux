#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
//#include <stdint.h>
#include "no_sal2.h"
#include "pal_mstypes.h"
#include "cordebug.h"
#include "managed_callback.h"


#define DLL_PROCESS_ATTACH 1
typedef HRESULT (*CreateCordbObject)(int iDebuggerVersion, ICorDebug **ppCordb);
typedef BOOL (*DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

void *load_lib(const char *name)
{
    void *lib = dlopen(name, RTLD_LAZY);
    if (lib == nullptr) 
    {
        printf("Can't load library %s\n", name);
        return nullptr;
    }

    DllMain dllMainProc = (DllMain)dlsym(lib, "DllMain");
    if (dllMainProc == nullptr) 
    {
        printf("Can't find DllMain in library %s\n", name);
        return nullptr;
    }
    
    if (!dllMainProc(lib, DLL_PROCESS_ATTACH, nullptr))
    {
        printf("DllMain for %s returned FALSE\n", name);
        return nullptr;
    }
    printf("Library loaded %s\n", name);

    return lib;
}

int main(int argc, const char **args)
{
    printf("Hi, I'm a process %d\n", getpid());

    //void *dac_lib = load_lib("libmscordaccore.so");
    void *dbi_lib = load_lib("libmscordbi.so");

    CreateCordbObject createCordbObject = (CreateCordbObject)dlsym(dbi_lib, "CreateCordbObject");
    if (createCordbObject == nullptr) 
    {
        printf("Bummer\n");
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

/*
    pCordb->CreateProcess( 
             LPCWSTR lpApplicationName,
             LPWSTR lpCommandLine,
             LPSECURITY_ATTRIBUTES lpProcessAttributes,
             LPSECURITY_ATTRIBUTES lpThreadAttributes,
             BOOL bInheritHandles,
             DWORD dwCreationFlags,
             PVOID lpEnvironment,
             LPCWSTR lpCurrentDirectory,
             LPSTARTUPINFOW lpStartupInfo,
             LPPROCESS_INFORMATION lpProcessInformation,
             CorDebugCreateProcessFlags debuggingFlags,
             ICorDebugProcess **ppProcess) = 0;
*/
    printf("<press any key>");
    getchar();

    return 0;
}
