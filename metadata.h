IMetaDataImport2 *get_metadata(ICorDebugModule *module)
{
    IUnknown *unkMetadata;
    IMetaDataImport2 *metadata;
    module->GetMetaDataInterface(IID_IMetaDataImport2, (IUnknown **)&unkMetadata);
    unkMetadata->QueryInterface(IID_IMetaDataImport2, (void**)&metadata);

    return metadata;    
}

void print_class(ICorDebugClass *type)
{
    ICorDebugModule *module = NULL;
    type->GetModule(&module);

    mdTypeDef tkType;
    type->GetToken(&tkType);
    auto metadata = get_metadata(module);

    WCHAR type_name[200];
    ULONG len;
    DWORD flags;
    mdToken baseType;
    metadata->GetTypeDefProps(tkType,
            type_name,
            sizeof(type_name) / sizeof(type_name[0]),
            &len,
            &flags,
            &baseType
        );    

    printf("%s", to_ascii(type_name));
}

void print_function(ICorDebugFunction *func)
{
    ICorDebugModule *module = NULL;
    func->GetModule(&module);
    mdMethodDef tkMethod;
    mdTypeDef tkType;
    func->GetToken(&tkMethod);
    auto metadata = get_metadata(module);

    WCHAR fname[200], type_name[200];
    ULONG len;
    DWORD flags;

    metadata->GetMethodProps(tkMethod,
        &tkType, 
        fname,
        sizeof(fname) / sizeof(fname[0]),
        &len,
        NULL, //pdwAttr
        NULL, //sig blob
        NULL, //sig blob count
        NULL, //RVA 
        &flags 
    );

    mdToken baseType;
    metadata->GetTypeDefProps(tkType,
            type_name,
            sizeof(type_name) / sizeof(type_name[0]),
            &len,
            &flags,
            &baseType
        );    

    printf("%s.%s\n", to_ascii(type_name), to_ascii(fname));
}


void print_callstack(ICorDebugThread *thread)
{
    ICorDebugChain *chain = NULL;
    thread->GetActiveChain(&chain);
    ICorDebugFrameEnum *frameEnum = NULL;
    chain->EnumerateFrames(&frameEnum);
    ICorDebugFrame *frames[100];
    ULONG frameCount = 0;
    frameEnum->Next(sizeof(frames)/sizeof(frames[0]),
                    frames,
                    &frameCount);
    for (int i = 0; i < frameCount; i++)
    {
        printf("#%d: ", i);
        ICorDebugFunction *func = NULL;
        frames[i]->GetFunction(&func);
        if (func) 
        {
            print_function(func);
        }
    }
}


void print_all_callstakcs(ICorDebugController *controller)
{
    ICorDebugThreadEnum *threadsEnum;
    controller->EnumerateThreads(&threadsEnum);
    ICorDebugThread *threads[100];
    ULONG count;
    threadsEnum->Next(100, threads, &count);
    for (int i = 0; i < count; i++) 
    {
        printf("Thread #%d\n", i);
        print_callstack(threads[i]);
    }
}

void enum_types(ICorDebugModule *module)
{
    auto metadata = get_metadata(module);

    mdTypeDef typeTokens[3000];
    ULONG typesCount;
    HCORENUM typeEnum = NULL;
    metadata->EnumTypeDefs(&typeEnum, typeTokens, sizeof(typeTokens) / sizeof(typeTokens[0]), &typesCount);

    printf("Types found: %d\n", typesCount);
    for (int i = 0; i < typesCount; i++)
    {
        auto tkType = typeTokens[i];
        WCHAR name[200];
        ULONG len;
        DWORD flags;
        mdToken baseType;
        metadata->GetTypeDefProps(tkType,
                name,
                sizeof(name) / sizeof(name[0]),
                &len,
                &flags,
                &baseType
            );

        printf("\t\ttype: %s\n", to_ascii(name));

        HCORENUM methodEnum = NULL;
        mdMethodDef methodTokens[3000];
        ULONG methodCount;        
        metadata->EnumMethods(&methodEnum, tkType, methodTokens, sizeof(methodTokens) / sizeof(methodTokens[0]) ,&methodCount);

        for (int j = 0; j < methodCount; j++)
        {
            metadata->GetMethodProps(methodTokens[j],
                NULL, //class
                name,
                sizeof(name) / sizeof(name[0]),
                &len,
                NULL, //pdwAttr
                NULL, //sig blob
                NULL, //sig blob count
                NULL, //RVA 
                &flags 
            );

            printf("\t\t\tmethod: %s\n", to_ascii(name));

            
            {
                printf("\tBreakpoint being created\n");
                ICorDebugFunction *mainFunc = NULL;
                module->GetFunctionFromToken(methodTokens[j], &mainFunc);

                ICorDebugFunctionBreakpoint *bp = NULL;
                HRESULT hr = mainFunc->CreateBreakpoint(&bp);
                if (hr == 0)
                {
                    hr = bp->Activate(TRUE);
                    printf("\tActivate(TRUE) breakpoint %08x\n", hr);

                    hr = bp->Activate(FALSE);
                    printf("\tActivate(FALSE) breakpoint %08x\n", hr);

                }
                else 
                {
                    printf("\tCreate breakpoint %s FAILED %08x\n", (char *)name, hr);
                }
            }
        }
    }
}
