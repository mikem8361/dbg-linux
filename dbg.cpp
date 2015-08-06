#define PAL_STDCPP_COMPAT
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


extern "C" int PAL_InitializeDLL();
extern "C" HMODULE LoadLibraryA(LPCSTR lpLibFileName);
extern "C" void *GetProcAddress (HMODULE hModule, LPCSTR lpProcName);


#ifdef EXTERNAL_PAL
#define DLL_PROCESS_ATTACH 1
typedef HRESULT (*CreateCordbObject)(int iDebuggerVersion, ICorDebug **ppCordb);
typedef BOOL (*DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
typedef int (*PAL_InitializeDLL)();
typedef int (*PAL_Initialize)(int argc, const char * const argv[]);
typedef int (*PAL_InitializeCoreCLR)(const char *szExePath, const char *szCoreCLRPath, BOOL fStayInPAL);
typedef HMODULE (*LoadLibraryA)(LPCSTR lpLibFileName);
typedef void *(*GetProcAddress) (HMODULE hModule, LPCSTR lpProcName);

void *load_pal()
{
    const char *name = "libmscordaccore.so";
    void *lib = dlopen(name, RTLD_LAZY);
    if (lib == nullptr) 
    {
        printf("Can't load library %s\n", name);
        return nullptr;
    }

    PAL_InitializeDLL palInitDll = (PAL_InitializeDLL)dlsym(lib, "PAL_InitializeDLL");

    if (palInitDll == nullptr) 
    {
        printf("Can't find PAL_Initialize in library %s\n", name);
        return nullptr;
    }

    int palInitResult = palInitDll();
    if (palInitResult != 0)
    {
        printf("PAL_Initialize returned error %d\n", palInitResult);
        return nullptr;        
    }

    
    return lib;   
}
#endif

typedef HRESULT (*EnumerateCLRs)(DWORD debuggeePID, HANDLE** ppHandleArrayOut, LPWSTR** ppStringArrayOut, DWORD* pdwArrayLengthOut);
typedef HRESULT (*CloseCLREnumeration)(HANDLE* pHandleArray, LPWSTR* pStringArray, DWORD dwArrayLength);
typedef HRESULT (*CreateVersionStringFromModule)(DWORD pidDebuggee, LPCWSTR szModuleName, LPWSTR pBuffer, DWORD cchBuffer, DWORD* pdwLength);
typedef HRESULT (*CreateDebuggingInterfaceFromVersion)(LPCWSTR szDebuggeeVersion, IUnknown ** ppCordb);

int main(int argc, const char **args)
{
    printf("Hi, I'm a process %d\n", getpid());

#ifdef EXTERNAL_PAL
    void *palLib = load_pal();
    auto loadLibraryA = (LoadLibraryA)dlsym(palLib, "LoadLibraryA");
    auto getProcAddress = (GetProcAddress)dlsym(palLib, "GetProcAddress");
#else
    PAL_InitializeDLL();
    auto getProcAddress = GetProcAddress;
    auto loadLibraryA = LoadLibraryA;
#endif    

    HMODULE shim_lib = loadLibraryA("libdbgshim.so");

    EnumerateCLRs enumerateCLRs = (EnumerateCLRs)getProcAddress(shim_lib, "EnumerateCLRs");
    if (enumerateCLRs == nullptr)
    {
        printf("enumerateCLRs == nullptr\n");
        return 1;
    }
    CloseCLREnumeration closeCLREnumeration = (CloseCLREnumeration)getProcAddress(shim_lib, "CloseCLREnumeration");
    if (closeCLREnumeration == nullptr)
    {
        printf("closeCLREnumeration == nullptr\n");
        return 1;
    }
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

    HANDLE *handleArray;
    LPWSTR *stringArray;
    DWORD arrayLength;
    HRESULT hr = enumerateCLRs(pid, &handleArray, &stringArray, &arrayLength);
    if (hr != 0 || arrayLength <= 0)
    {
        printf("EnumerateCLRs FAILED hr=%X arrayLength=%d\n", (int)hr, arrayLength);
        return 1;
    }

    WCHAR verStr[1000];
    DWORD verLen;
    hr = createVersionStringFromModule(pid, stringArray[0], verStr, sizeof(verStr)/sizeof(verStr[0]), &verLen);
    if (hr != 0)
    {
        printf("CreateVersionStringFromModule FAILED hr=%X\n", (int)hr);
        return 1;
    }

    ICorDebug *pCordb = nullptr;
    hr = createDebuggingInterfaceFromVersion(verStr, (IUnknown **)&pCordb);
    printf("CreateVersionStringFromModule hr=%X pCordb=%p\n", (int)hr, pCordb);

    closeCLREnumeration(handleArray, stringArray, arrayLength);

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
