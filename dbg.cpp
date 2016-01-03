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
extern "C" DWORD GetLastError(VOID);
extern "C" DWORD ResumeThread(HANDLE hThread);

typedef struct _STARTUPINFOA {
    DWORD cb;
    LPSTR lpReserved;
    LPSTR lpDesktop;
    LPSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    LPBYTE lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

#define CREATE_SUSPENDED                  0x00000004

extern "C" BOOL CreateProcessA(
    LPCSTR lpApplicationName,
    LPSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation);

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

typedef VOID (*PSTARTUP_CALLBACK)(IUnknown *pCordb, PVOID parameter, HRESULT hr);
typedef HRESULT (*RegisterForRuntimeStartup)(DWORD dwProcessId, PSTARTUP_CALLBACK pfnCallback, PVOID parameter, PVOID *ppUnregisterToken);
typedef HRESULT (*UnregisterForRuntimeStartup)(PVOID pUnregisterToken);

typedef HRESULT (*EnumerateCLRs)(DWORD debuggeePID, HANDLE** ppHandleArrayOut, LPWSTR** ppStringArrayOut, DWORD* pdwArrayLengthOut);
typedef HRESULT (*CloseCLREnumeration)(HANDLE* pHandleArray, LPWSTR* pStringArray, DWORD dwArrayLength);
typedef HRESULT (*CreateVersionStringFromModule)(DWORD pidDebuggee, LPCWSTR szModuleName, LPWSTR pBuffer, DWORD cchBuffer, DWORD* pdwLength);
typedef HRESULT (*CreateDebuggingInterfaceFromVersion)(LPCWSTR szDebuggeeVersion, IUnknown ** ppCordb);

ICorDebugProcess *g_process = nullptr;
ICorDebug *g_pCordb = nullptr;
UnregisterForRuntimeStartup g_unregisterForRuntimeStartup = nullptr;
PVOID g_pUnregisterToken = nullptr;
bool g_unregisterInCallback = false;

void RuntimeStartupHandler(IUnknown *punk, PVOID parameter, HRESULT hr)
{
    if (hr == 0) 
    {
	int pid = (int)(intptr_t)parameter;
	g_pCordb = (ICorDebug *)punk;

        //hr = punk->QueryInterface(IID_ICorDebug, (void **)&pCordb);
        //printf("QueryInterface hr=%08x\n", (int)hr);

        hr = g_pCordb->Initialize();
        printf("Cordebug initialized hr=%08x\n", (int)hr);

        ManagedCallback *callback = new ManagedCallback();
        hr = g_pCordb->SetManagedHandler(callback);
        printf("SetManagedHandler hr=%08x\n", (int)hr);    

        printf("Attaching to pid %d\n", pid);
        hr = g_pCordb->DebugActiveProcess((DWORD)pid, FALSE, &g_process);
        printf("DebugActiveProcess hr=%08x\n", (int)hr);
    }
    else
    {
        printf("RuntimeStartupHandler FAILED hr=%08x\n", (int)hr);
    }

    if (g_unregisterInCallback)
    {
	hr = g_unregisterForRuntimeStartup(g_pUnregisterToken);
	printf("UnregisterForRuntimeStartup hr=%08x token %p\n", (int)hr, g_pUnregisterToken);
    }
}

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

    RegisterForRuntimeStartup registerForRuntimeStartup = (RegisterForRuntimeStartup)getProcAddress(shim_lib, "RegisterForRuntimeStartup");
    if (registerForRuntimeStartup == nullptr)
    {
        printf("RegisterForRuntimeStartup not found\n");
        return 1;
    }

    g_unregisterForRuntimeStartup = (UnregisterForRuntimeStartup)getProcAddress(shim_lib, "UnregisterForRuntimeStartup");
    if (g_unregisterForRuntimeStartup == nullptr)
    {
        printf("UnregisterForRuntimeStartup not found\n");
        return 1;
    }

    EnumerateCLRs enumerateCLRs = (EnumerateCLRs)getProcAddress(shim_lib, "EnumerateCLRs");
    if (enumerateCLRs == nullptr)
    {
        printf("EnumerateCLRs not found\n");
        return 1;
    }
    CloseCLREnumeration closeCLREnumeration = (CloseCLREnumeration)getProcAddress(shim_lib, "CloseCLREnumeration");
    if (closeCLREnumeration == nullptr)
    {
        printf("CloseCLREnumeration not found\n");
        return 1;
    }
    CreateVersionStringFromModule createVersionStringFromModule = (CreateVersionStringFromModule)getProcAddress(shim_lib, "CreateVersionStringFromModule");
    if (createVersionStringFromModule == nullptr)
    {
        printf("CreateVersionStringFromModule not found\n");
        return 1;
    }
    CreateDebuggingInterfaceFromVersion createDebuggingInterfaceFromVersion = (CreateDebuggingInterfaceFromVersion)getProcAddress(shim_lib, "CreateDebuggingInterfaceFromVersion");
    if (createDebuggingInterfaceFromVersion == nullptr)
    {
        printf("CreateDebuggingInterfaceFromVersion not found\n");
        return 1;
    }    

    bool newApi = true;
    bool launch = true;
    HRESULT hr = 0;
    int pid = 0;

    for (int i = 0; i < argc; i++)
    {
        if (i > 0)
        {
            if (strcmp(args[i], "-oldapi") == 0)
            {
		newApi = false;
            }
            else if (strcmp(args[i], "-unregincallback") == 0)
	    {
                g_unregisterInCallback = true;
	    }
	    else 
	    { 
		launch = false;
		pid = atoi(args[i]);
	    }
        }
    }    

    if (newApi) 
    {
        PROCESS_INFORMATION processInfo;

        if (launch)
        {
            STARTUPINFOA startupInfo;
        
            startupInfo.cb = sizeof(startupInfo);
            startupInfo.lpReserved = NULL;
            startupInfo.lpDesktop = NULL;
            startupInfo.lpTitle = NULL;
            startupInfo.dwFlags = 0;
            startupInfo.cbReserved2 = 0;
            startupInfo.lpReserved2 = NULL;
            startupInfo.hStdError = NULL;
            startupInfo.hStdInput = NULL;
            startupInfo.hStdOutput = NULL;
#ifdef WINDOWS
            #define PROGRAM_NAME "corerun.exe Hello.exe"
#else
            #define PROGRAM_NAME "corerun Hello.exe"
#endif
	    printf("Launching %s\n", PROGRAM_NAME);
            BOOL result = CreateProcessA(NULL, (LPSTR)PROGRAM_NAME, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo);
            if (!result)
            {
                printf("CreateProcessA(%s) FAILED %d\n", PROGRAM_NAME, GetLastError());
                return 1;
            }

            pid = processInfo.dwProcessId;
        }

        printf("RegisterForRuntimeStartup for process %d\n", pid);

        hr = registerForRuntimeStartup(pid, RuntimeStartupHandler, (PVOID)(intptr_t)pid, &g_pUnregisterToken);
        if (hr != 0)
        {
            printf("RegisterForRuntimeStartup FAILED hr=%08x\n", (int)hr);
            return 1;
        }
        
        if (launch)
        {
	    printf("ResumeThread for pid %d\n", pid);

            if (ResumeThread(processInfo.hThread) == (DWORD)-1)
            {
                printf("ResumeThread FAILED %d\n", GetLastError());
                return 1;
            }
        }
    }
    else 
    {
        printf("Enumerating CLRs in process %d\n", pid);

	if (launch)
        {
	    printf("Must have an pid with -oldapi\n");
	    return 1;	
	}

        HANDLE *handleArray;
        LPWSTR *stringArray;
        DWORD arrayLength;
        hr = enumerateCLRs(pid, &handleArray, &stringArray, &arrayLength);
        if (hr != 0 || arrayLength <= 0)
        {
            printf("EnumerateCLRs FAILED hr=%08x arrayLength=%d\n", (int)hr, arrayLength);
            return 1;
        }

        WCHAR verStr[1000];
        DWORD verLen;
        hr = createVersionStringFromModule(pid, stringArray[0], verStr, sizeof(verStr)/sizeof(verStr[0]), &verLen);
        if (hr != 0)
        {
            printf("CreateVersionStringFromModule FAILED hr=%08x\n", (int)hr);
            return 1;
        }

        ICorDebug *pCordb = nullptr;
        hr = createDebuggingInterfaceFromVersion(verStr, (IUnknown **)&pCordb);
        printf("CreateVersionStringFromModule hr=%08x pCordb=%p\n", (int)hr, pCordb);

        closeCLREnumeration(handleArray, stringArray, arrayLength);

        if (hr != 0 || pCordb == nullptr)
        {
            printf("Can't create ICorDebug!\n");
            return 1;
        }
        
        RuntimeStartupHandler(pCordb, (PVOID)(intptr_t)pid, 0);
    }

    printf("<press any key>");
    getchar();

    if (!g_unregisterInCallback)
    {
	hr = g_unregisterForRuntimeStartup(g_pUnregisterToken);
	printf("UnregisterForRuntimeStartup hr=%08x token %p\n", (int)hr, g_pUnregisterToken);
    }

    if (g_process != nullptr)
    {
	hr = g_process->Stop(0);
	printf("Stop hr=%08x\n", (int)hr);

	if (hr == 0) 
	{
	    ICorDebugAppDomainEnum *domainEnum;
	    hr = g_process->EnumerateAppDomains(&domainEnum);
	    printf("EnumerateAppDomains hr=%08x\n", (int)hr);

	    ICorDebugAppDomain *domains[2];
	    ULONG nDomains = 0;
	    hr = domainEnum->Next(2, domains, &nDomains);
	    printf("Next hr=%08x\n", (int)hr);

	    for (int i = 0; i < nDomains; i++) {
		ICorDebugBreakpointEnum *bpEnum;
		hr = domains[i]->EnumerateBreakpoints(&bpEnum);
		printf("EnumerateBreakpoints hr=%08x\n", (int)hr);

		while (TRUE) {
		    ICorDebugBreakpoint *bps[200];
		    ULONG nBps = 0;
		    hr = bpEnum->Next(200, bps, &nBps);
		    printf("Next hr=%08x\n", (int)hr);

		    if (nBps == 0) {
			break;
		    }

		    for (int b = 0; b < nBps; b++) {
			hr = bps[b]->Activate(FALSE);
			printf("bp Activate(FALSE) hr=%08x\n", (int)hr);
		    }
		}
	    }

	    hr = g_process->Detach();
	    printf("Detach hr=%08x\n", (int)hr);
	}
    }

    if (g_pCordb != nullptr)
    {
	hr = g_pCordb->Terminate();
	printf("Cordebug Terminate hr=%08x\n", (int)hr);
    }
    return 0;
}

