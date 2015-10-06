#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef FEATURE_PAL
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <no_sal2.h>
#include <pal_char16.h>
#include <pal_mstypes.h>
#include <rpc.h>
#include <rt/ntimage.h>
#endif
#include <rpc.h>
#include <unknwn.h>
#include <cor.h>
#include <cordebug.h>

// Platform-specific library naming
// 
#ifdef FEATURE_PAL
#ifdef __APPLE__
#define MAKEDLLNAME_W(name) u"lib" name u".dylib"
#define MAKEDLLNAME_A(name)  "lib" name  ".dylib"
#elif defined(_AIX)
#define MAKEDLLNAME_W(name) L"lib" name L".a"
#define MAKEDLLNAME_A(name)  "lib" name  ".a"
#elif defined(__hppa__) || defined(_IA64_)
#define MAKEDLLNAME_W(name) L"lib" name L".sl"
#define MAKEDLLNAME_A(name)  "lib" name  ".sl"
#else
#define MAKEDLLNAME_W(name) u"lib" name u".so"
#define MAKEDLLNAME_A(name)  "lib" name  ".so"
#endif
#else // FEATURE_PAL
#define MAKEDLLNAME_W(name) name L".dll"
#define MAKEDLLNAME_A(name) name ".dll"
#endif

char *to_ascii(WCHAR *src)
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

#ifdef FEATURE_PAL

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
    const char *name = MAKEDLLNAME_A("mscordaccore");
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

#endif // FEATURE_PAL

typedef HRESULT (*EnumerateCLRs)(DWORD debuggeePID, HANDLE** ppHandleArrayOut, LPWSTR** ppStringArrayOut, DWORD* pdwArrayLengthOut);
typedef HRESULT (*CloseCLREnumeration)(HANDLE* pHandleArray, LPWSTR* pStringArray, DWORD dwArrayLength);
typedef HRESULT (*CreateVersionStringFromModule)(DWORD pidDebuggee, LPCWSTR szModuleName, LPWSTR pBuffer, DWORD cchBuffer, DWORD* pdwLength);
typedef HRESULT (*CreateDebuggingInterfaceFromVersion)(LPCWSTR szDebuggeeVersion, IUnknown ** ppCordb);

int main(int argc, const char **args)
{
#ifdef FEATURE_PAL
    printf("Hi, I'm a process %d\n", getpid());
#endif

#ifdef EXTERNAL_PAL
    void *palLib = load_pal();
    auto loadLibraryA = (LoadLibraryA)dlsym(palLib, "LoadLibraryA");
    auto getProcAddress = (GetProcAddress)dlsym(palLib, "GetProcAddress");
#else
#ifdef FEATURE_PAL
    PAL_InitializeDLL();
#endif
    auto getProcAddress = GetProcAddress;
    auto loadLibraryA = LoadLibraryA;
#endif    
    const char *shimName = MAKEDLLNAME_A("dbgshim");
    HMODULE shim_lib = loadLibraryA(shimName);
    if (shim_lib == nullptr)
    {
#ifdef FEATURE_PAL
        printf("%s not found", shimName);
#else
        int error = GetLastError();
        printf("%s not found 0x%04x", shimName, error);
#endif
        return 1;
    }

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
    printf("Enumerating CLRs in process %d\n", pid);

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

    hr = process->Stop(0);
    printf("Stop hr=%X\n", (int)hr);

    if (hr == 0) 
    {
	ICorDebugAppDomainEnum *domainEnum;
	hr = process->EnumerateAppDomains(&domainEnum);
	printf("EnumerateAppDomains hr=%X\n", (int)hr);

	ICorDebugAppDomain *domains[2];
	ULONG nDomains = 0;
	hr = domainEnum->Next(2, domains, &nDomains);
	printf("Next hr=%X\n", (int)hr);

	for (int i = 0; i < nDomains; i++) {
	    ICorDebugBreakpointEnum *bpEnum;
	    hr = domains[i]->EnumerateBreakpoints(&bpEnum);
	    printf("EnumerateBreakpoints hr=%X\n", (int)hr);

	    while (TRUE) {
		ICorDebugBreakpoint *bps[200];
		ULONG nBps = 0;
		hr = bpEnum->Next(200, bps, &nBps);
		printf("Next hr=%X\n", (int)hr);

		if (nBps == 0) {
		    break;
		}

		for (int b = 0; b < nBps; b++) {
		    hr = bps[b]->Activate(FALSE);
		    printf("bp Activate(FALSE) hr=%X\n", (int)hr);
		}
	    }
	}

	hr = process->Detach();
	printf("Detach hr=%X\n", (int)hr);
    }

    hr = pCordb->Terminate();
    printf("Cordebug Terminate hr=%X\n", (int)hr);

    printf("<press any key>");
    getchar();

    return 0;
}
