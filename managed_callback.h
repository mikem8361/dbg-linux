#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}



MIDL_DEFINE_GUID(IID, IID_ICorDebugManagedCallback,0x3d6f5f60,0x7538,0x11d3,0x8d,0x5b,0x00,0x10,0x4b,0x35,0xe7,0xef);


MIDL_DEFINE_GUID(IID, IID_ICorDebugManagedCallback3,0x264EA0FC,0x2591,0x49AA,0x86,0x8E,0x83,0x5E,0x65,0x15,0x32,0x3F);


MIDL_DEFINE_GUID(IID, IID_ICorDebugManagedCallback2,0x250E5EEA,0xDB5C,0x4C76,0xB6,0xF3,0x8C,0x46,0xF1,0x2E,0x32,0x03);

MIDL_DEFINE_GUID(IID, IID_ICorDebugFunctionBreakpoint,0xCC7BCAE9,0x8A68,0x11d2,0x98,0x3C,0x00,0x00,0xF8,0x08,0x34,0x2D);

MIDL_DEFINE_GUID(IID, IID_ICorDebugILFrame4,0xAD914A30,0xC6D1,0x4AC5,0x9C,0x5E,0x57,0x7F,0x3B,0xAA,0x8A,0x45);

MIDL_DEFINE_GUID(IID, IID_ICorDebugAssembly2,0x426d1f9e,0x6dd4,0x44c8,0xae,0xc7,0x26,0xcd,0xba,0xf4,0xe3,0x98);

#define DebugBreak() asm("int 3");
#define PrintWhereIAm() printf("Callback called: %s\n", __func__);

class ManagedCallback : public ICorDebugManagedCallback, ICorDebugManagedCallback2
{
    ICorDebugProcess *m_pProcess;


public:
    ManagedCallback()
        :m_pProcess(nullptr)
    {
    }

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        void **ppvObj) 
    {
        if (!ppvObj)
            return E_INVALIDARG;

        *ppvObj = NULL;
        if (/* riid == IID_IUnknown || */ riid == IID_ICorDebugManagedCallback
             || riid == IID_ICorDebugManagedCallback2)
        {
            // Increment the reference count and return the pointer.
            *ppvObj = (LPVOID)this;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }
        
    virtual ULONG STDMETHODCALLTYPE AddRef( void) { return 1; }
        
    virtual ULONG STDMETHODCALLTYPE Release( void) { return 1; }

    // ICorDebugManagedCallback
    virtual HRESULT STDMETHODCALLTYPE Breakpoint( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugBreakpoint *pBreakpoint)
    {
	static int bpsHit = 0;

        ICorDebugFunctionBreakpoint *fbp = NULL;
        pBreakpoint->QueryInterface(IID_ICorDebugFunctionBreakpoint, (void**)&fbp);
        ICorDebugFunction *function = NULL;
        int hr = fbp->GetFunction(&function);
        if (function == NULL || pThread == NULL) {
	    printf("Breakpoint notification %x NULL function %p or pThread %p", hr, function, pThread);
        }
	else {
            printf("Breakpoint at \n");
       	    print_callstack(pThread);
	}

	if (++bpsHit > 100)
        {
	    pBreakpoint->Activate(FALSE);
            printf("Deactivating BP after %d hits\n", bpsHit);
        }
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE StepComplete( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugStepper *pStepper,
        /* [in] */ CorDebugStepReason reason)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE Break( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *thread)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE Exception( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ BOOL unhandled)    
    {
        PrintWhereIAm(); 
        printf("unhandled %s\n", unhandled ? "true" : "false");
        print_all_callstakcs(m_pProcess);
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE EvalComplete( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugEval *pEval)
    { 
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE EvalException( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugEval *pEval)    
    { 
        PrintWhereIAm(); 
        
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE CreateProcess( 
        /* [in] */ ICorDebugProcess *pProcess)    
    {
        PrintWhereIAm();
        m_pProcess = pProcess;
        m_pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE ExitProcess( 
        /* [in] */ ICorDebugProcess *pProcess)    
    {
        PrintWhereIAm(); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE CreateThread( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *thread)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE ExitThread( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *thread)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE LoadModule( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugModule *pModule)    
    {
        PrintWhereIAm(); 
        WCHAR name[100];
        ULONG32 len;
        pModule->EnableJITDebugging(TRUE, FALSE);
        pModule->GetName(100, &len, name);
        printf("\tModule name: %s\n", to_ascii(name)); 
        if (strstr((const char *)name, "mscorlib") == NULL)
        {
            enum_types(pModule);
            pModule->EnableClassLoadCallbacks(TRUE);
        }
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE UnloadModule( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugModule *pModule)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE LoadClass( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugClass *c)    
    {
        PrintWhereIAm(); 
        printf("\tclass: ");
        print_class(c);
        printf("\n");
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE UnloadClass( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugClass *c)    
    {
        PrintWhereIAm(); 
        printf("\tclass");
        print_class(c);
        printf("\n");        
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE DebuggerError( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ HRESULT errorHR,
        /* [in] */ DWORD errorCode)    
    {
        PrintWhereIAm(); 
        pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE LogMessage( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ LONG lLevel,
        /* [in] */ WCHAR *pLogSwitchName,
        /* [in] */ WCHAR *pMessage)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE LogSwitch( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ LONG lLevel,
        /* [in] */ ULONG ulReason,
        /* [in] */ WCHAR *pLogSwitchName,
        /* [in] */ WCHAR *pParentName)
        {
            PrintWhereIAm();
            printf("\tSwitch name: %s\n", to_ascii(pLogSwitchName));
            pAppDomain->Continue(FALSE);  
            return S_OK; 
        }
    
    virtual HRESULT STDMETHODCALLTYPE CreateAppDomain( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ ICorDebugAppDomain *pAppDomain)
        {
            PrintWhereIAm(); 
            pAppDomain->Attach();
            pAppDomain->Continue(FALSE); 
            return S_OK; 
        }
    
    virtual HRESULT STDMETHODCALLTYPE ExitAppDomain( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ ICorDebugAppDomain *pAppDomain)
        {
            PrintWhereIAm(); 
            pAppDomain->Continue(FALSE); 
            return S_OK; 
        }
    
    virtual HRESULT STDMETHODCALLTYPE LoadAssembly( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugAssembly *pAssembly)    
    {
        PrintWhereIAm(); 
        {
            BOOL t;
            ICorDebugAssembly2 *a2;
            pAssembly->QueryInterface(IID_ICorDebugAssembly2, (void **)&a2);
            a2->IsFullyTrusted(&t);
        }
        WCHAR name[100];
        ULONG32 len;
        pAssembly->GetName(100, &len, name);
        printf("\tAssembly name: %s\n", to_ascii(name));
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE UnloadAssembly( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugAssembly *pAssembly)
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE ControlCTrap( 
        /* [in] */ ICorDebugProcess *pProcess)
    {
        PrintWhereIAm(); 
        pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE NameChange( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread)
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE UpdateModuleSymbols( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugModule *pModule,
        /* [in] */ IStream *pSymbolStream)
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE EditAndContinueRemap( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugFunction *pFunction,
        /* [in] */ BOOL fAccurate)    
    { 
        PrintWhereIAm(); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE BreakpointSetError( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugBreakpoint *pBreakpoint,
        /* [in] */ DWORD dwError)    
    { 
        PrintWhereIAm(); 
        return S_OK; 
    }
    

    // ICorDebugManagedCallback2
    virtual HRESULT STDMETHODCALLTYPE FunctionRemapOpportunity( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugFunction *pOldFunction,
        /* [in] */ ICorDebugFunction *pNewFunction,
        /* [in] */ ULONG32 oldILOffset)    
    { 
        PrintWhereIAm(); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE CreateConnection( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ CONNID dwConnectionId,
        /* [in] */ WCHAR *pConnName)
    {
        PrintWhereIAm(); 
        pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE ChangeConnection( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ CONNID dwConnectionId)
    {
        PrintWhereIAm(); 
        pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE DestroyConnection( 
        /* [in] */ ICorDebugProcess *pProcess,
        /* [in] */ CONNID dwConnectionId)
    {
        PrintWhereIAm(); 
        pProcess->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE Exception( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugFrame *pFrame,
        /* [in] */ ULONG32 nOffset,
        /* [in] */ CorDebugExceptionCallbackType dwEventType,
        /* [in] */ DWORD dwFlags)    
    {
        PrintWhereIAm();
        printf("\tFlags = %x\n dwEventType=%x", dwFlags, (DWORD)dwEventType);
        print_callstack(pThread);
        pAppDomain->Continue(FALSE);  
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE ExceptionUnwind( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ CorDebugExceptionUnwindCallbackType dwEventType,
        /* [in] */ DWORD dwFlags)    
    {
        PrintWhereIAm(); 
        pAppDomain->Continue(FALSE); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE FunctionRemapComplete( 
        /* [in] */ ICorDebugAppDomain *pAppDomain,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugFunction *pFunction)    
    { 
        PrintWhereIAm(); 
        return S_OK; 
    }
    
    virtual HRESULT STDMETHODCALLTYPE MDANotification( 
        /* [in] */ ICorDebugController *pController,
        /* [in] */ ICorDebugThread *pThread,
        /* [in] */ ICorDebugMDA *pMDA)    
    { 
        PrintWhereIAm(); 
        return S_OK; 
    }
    

};
