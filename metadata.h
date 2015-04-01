void enum_types(ICorDebugModule *module)
{
    IUnknown *unkMetadata;
    IMetaDataImport2 *metadata;
    module->GetMetaDataInterface(IID_IMetaDataImport2, (IUnknown **)&unkMetadata);
    unkMetadata->QueryInterface(IID_IMetaDataImport2, (void**)&metadata);

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

            if (strcmp((char *)name, "Main") == 0)
            {
                printf("\tBreakpoint being created\n");
                ICorDebugFunction *mainFunc = NULL;
                module->GetFunctionFromToken(methodTokens[j], &mainFunc);
                ICorDebugFunctionBreakpoint *bp = NULL;
                mainFunc->CreateBreakpoint(&bp);
                bp->Activate(TRUE);

            }
        }


    }
}