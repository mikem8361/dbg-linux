#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
//#include <stdint.h>
#include "no_sal2.h"
#include "pal_mstypes.h"
#include "cordebug.h"



typedef HRESULT (*CreateCordbObject)(int iDebuggerVersion, ICorDebug **ppCordb);

int main(int argc, const char **args)
{
    printf("Hi, I'm a process %d\n", getpid());

    void *dac_lib = dlopen("libmscordaccore.so", RTLD_LAZY | RTLD_GLOBAL);
    if (dac_lib == nullptr) 
    {
        printf("Bummer DAC\n");
        return 1;
    }
    else
    {
        printf("dac %p\n", dac_lib);
    }

    void *dbi_lib = dlopen("libmscordbi.so", RTLD_LAZY | RTLD_GLOBAL);
    if (dbi_lib == nullptr) 
    {
        printf("Bummer\n");
        return 1;
    }
    else
    {
        printf("dbi %p\n", dbi_lib);
    }

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

    printf("Cordebug created hr=%d pCordb=%p\n", (int)hr, pCordb);

    hr = pCordb->Initialize();
    printf("Cordebug initialized hr=%d\n", (int)hr);

    getchar();

    return 0;
}