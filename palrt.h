
/******************* HRESULTs *********************************************/

#ifdef RC_INVOKED
#define _HRESULT_TYPEDEF_(_sc) _sc
#else // RC_INVOKED
#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#endif // RC_INVOKED

#define S_OK                             _HRESULT_TYPEDEF_(0x00000000L)
#define S_FALSE                          _HRESULT_TYPEDEF_(0x00000001L)

#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)
#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFFL)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)
#define E_HANDLE                         _HRESULT_TYPEDEF_(0x80070006L)
#define E_ABORT                          _HRESULT_TYPEDEF_(0x80004004L)
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)
#define E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80070005L)
#define E_PENDING                        _HRESULT_TYPEDEF_(0x8000000AL)

#define DISP_E_PARAMNOTFOUND             _HRESULT_TYPEDEF_(0x80020004L)
#define DISP_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80020005L)
#define DISP_E_BADVARTYPE                _HRESULT_TYPEDEF_(0x80020008L)
#define DISP_E_OVERFLOW                  _HRESULT_TYPEDEF_(0x8002000AL)
#define DISP_E_DIVBYZERO                 _HRESULT_TYPEDEF_(0x80020012L)

#define CLASS_E_CLASSNOTAVAILABLE        _HRESULT_TYPEDEF_(0x80040111L)
#define CLASS_E_NOAGGREGATION            _HRESULT_TYPEDEF_(0x80040110L)

#define CO_E_CLASSSTRING                 _HRESULT_TYPEDEF_(0x800401F3L)

#define URL_E_INVALID_SYNTAX             _HRESULT_TYPEDEF_(0x80041001L)
#define MK_E_SYNTAX                      _HRESULT_TYPEDEF_(0x800401E4L)

#define STG_E_INVALIDFUNCTION            _HRESULT_TYPEDEF_(0x80030001L)
#define STG_E_FILENOTFOUND               _HRESULT_TYPEDEF_(0x80030002L)
#define STG_E_PATHNOTFOUND               _HRESULT_TYPEDEF_(0x80030003L)
#define STG_E_WRITEFAULT                 _HRESULT_TYPEDEF_(0x8003001DL)
#define STG_E_FILEALREADYEXISTS          _HRESULT_TYPEDEF_(0x80030050L)
#define STG_E_ABNORMALAPIEXIT            _HRESULT_TYPEDEF_(0x800300FAL)

#define NTE_BAD_UID                      _HRESULT_TYPEDEF_(0x80090001L)
#define NTE_BAD_HASH                     _HRESULT_TYPEDEF_(0x80090002L)
#define NTE_BAD_KEY                      _HRESULT_TYPEDEF_(0x80090003L)
#define NTE_BAD_LEN                      _HRESULT_TYPEDEF_(0x80090004L)
#define NTE_BAD_DATA                     _HRESULT_TYPEDEF_(0x80090005L)
#define NTE_BAD_SIGNATURE                _HRESULT_TYPEDEF_(0x80090006L)
#define NTE_BAD_VER                      _HRESULT_TYPEDEF_(0x80090007L)
#define NTE_BAD_ALGID                    _HRESULT_TYPEDEF_(0x80090008L)
#define NTE_BAD_FLAGS                    _HRESULT_TYPEDEF_(0x80090009L)
#define NTE_BAD_TYPE                     _HRESULT_TYPEDEF_(0x8009000AL)
#define NTE_BAD_KEY_STATE                _HRESULT_TYPEDEF_(0x8009000BL)
#define NTE_BAD_HASH_STATE               _HRESULT_TYPEDEF_(0x8009000CL)
#define NTE_NO_KEY                       _HRESULT_TYPEDEF_(0x8009000DL)
#define NTE_NO_MEMORY                    _HRESULT_TYPEDEF_(0x8009000EL)
#define NTE_SIGNATURE_FILE_BAD           _HRESULT_TYPEDEF_(0x8009001CL)
#define NTE_FAIL                         _HRESULT_TYPEDEF_(0x80090020L)

#define CRYPT_E_HASH_VALUE               _HRESULT_TYPEDEF_(0x80091007L)

#define TYPE_E_SIZETOOBIG                _HRESULT_TYPEDEF_(0x800288C5L)
#define TYPE_E_DUPLICATEID               _HRESULT_TYPEDEF_(0x800288C6L)

#define STD_CTL_SCODE(n) MAKE_SCODE(SEVERITY_ERROR, FACILITY_CONTROL, n)
#define CTL_E_OVERFLOW                  STD_CTL_SCODE(6)
#define CTL_E_OUTOFMEMORY               STD_CTL_SCODE(7)
#define CTL_E_DIVISIONBYZERO            STD_CTL_SCODE(11)
#define CTL_E_OUTOFSTACKSPACE           STD_CTL_SCODE(28)
#define CTL_E_FILENOTFOUND              STD_CTL_SCODE(53)
#define CTL_E_DEVICEIOERROR             STD_CTL_SCODE(57)
#define CTL_E_PERMISSIONDENIED          STD_CTL_SCODE(70)
#define CTL_E_PATHFILEACCESSERROR       STD_CTL_SCODE(75)
#define CTL_E_PATHNOTFOUND              STD_CTL_SCODE(76)

#define INET_E_CANNOT_CONNECT            _HRESULT_TYPEDEF_(0x800C0004L)
#define INET_E_RESOURCE_NOT_FOUND        _HRESULT_TYPEDEF_(0x800C0005L)
#define INET_E_OBJECT_NOT_FOUND          _HRESULT_TYPEDEF_(0x800C0006L)
#define INET_E_DATA_NOT_AVAILABLE        _HRESULT_TYPEDEF_(0x800C0007L)
#define INET_E_DOWNLOAD_FAILURE          _HRESULT_TYPEDEF_(0x800C0008L)
#define INET_E_CONNECTION_TIMEOUT        _HRESULT_TYPEDEF_(0x800C000BL)
#define INET_E_UNKNOWN_PROTOCOL          _HRESULT_TYPEDEF_(0x800C000DL)

#define DBG_PRINTEXCEPTION_C             _HRESULT_TYPEDEF_(0x40010006L)

//************************** end of HRESULTS ***************************    

#define interface struct

#define STDMETHODCALLTYPE    __stdcall
#define STDMETHODVCALLTYPE   __cdecl

#define STDAPICALLTYPE       __stdcall
#define STDAPIVCALLTYPE      __cdecl

#define STDMETHODIMP         HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)  type STDMETHODCALLTYPE

#define STDMETHODIMPV        HRESULT STDMETHODVCALLTYPE
#define STDMETHODIMPV_(type) type STDMETHODVCALLTYPE

#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method

#define STDMETHODV(method)       virtual HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method) virtual type STDMETHODVCALLTYPE method

#define STDAPI               EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type)        EXTERN_C type STDAPICALLTYPE

#define STDAPIV              EXTERN_C HRESULT STDAPIVCALLTYPE
#define STDAPIV_(type)       EXTERN_C type STDAPIVCALLTYPE

#define PURE                    = 0
#define THIS_
#define THIS                void

#if _MSC_VER
#define DECLSPEC_NOVTABLE   __declspec(novtable)
#define DECLSPEC_IMPORT     __declspec(dllimport)
#define DECLSPEC_SELECTANY  __declspec(selectany)
#elif defined(__GNUC__)
#define DECLSPEC_NOVTABLE
#define DECLSPEC_IMPORT     
#define DECLSPEC_SELECTANY  __attribute__((weak))
#else
#define DECLSPEC_NOVTABLE
#define DECLSPEC_IMPORT
#define DECLSPEC_SELECTANY
#endif

#define DECLARE_INTERFACE(iface)    interface DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface DECLSPEC_NOVTABLE iface : public baseiface

#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID *
#endif

EXTERN_C const GUID GUID_NULL;

typedef GUID *LPGUID;
typedef const GUID FAR *LPCGUID;

#ifdef __cplusplus
extern "C++" {
inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
    { return !memcmp(&rguid1, &rguid2, sizeof(GUID)); }
inline int operator==(REFGUID guidOne, REFGUID guidOther)
    { return IsEqualGUID(guidOne,guidOther); }
inline int operator!=(REFGUID guidOne, REFGUID guidOther)
    { return !IsEqualGUID(guidOne,guidOther); }
};
#endif // __cplusplus

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID FAR name

typedef GUID IID;
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID *
#endif
#define IID_NULL GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)

#define __IID_DEFINED__

typedef GUID CLSID;
#define CLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const CLSID &
#else
#define REFCLSID const CLSID *
#endif
#define CLSID_NULL GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)

typedef UINT_PTR WPARAM;
typedef LONG_PTR LRESULT;

typedef LONG SCODE;


typedef union _ULARGE_INTEGER {
    struct {
#if BIGENDIAN
        DWORD HighPart;
        DWORD LowPart;
#else
        DWORD LowPart;
        DWORD HighPart;
#endif
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

/******************* HRESULT types ****************************************/

#define FACILITY_WINDOWS                 8
#define FACILITY_URT                     19
#define FACILITY_UMI                     22
#define FACILITY_SXS                     23
#define FACILITY_STORAGE                 3
#define FACILITY_SSPI                    9
#define FACILITY_SCARD                   16
#define FACILITY_SETUPAPI                15
#define FACILITY_SECURITY                9
#define FACILITY_RPC                     1
#define FACILITY_WIN32                   7
#define FACILITY_CONTROL                 10
#define FACILITY_NULL                    0
#define FACILITY_MSMQ                    14
#define FACILITY_MEDIASERVER             13
#define FACILITY_INTERNET                12
#define FACILITY_ITF                     4
#define FACILITY_DPLAY                   21
#define FACILITY_DISPATCH                2
#define FACILITY_COMPLUS                 17
#define FACILITY_CERT                    11
#define FACILITY_ACS                     20
#define FACILITY_AAF                     18

#define NO_ERROR 0L

#define SEVERITY_SUCCESS    0
#define SEVERITY_ERROR      1

#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)
#define IS_ERROR(Status) ((ULONG)(Status) >> 31 == SEVERITY_ERROR) // diff from win32
#define HRESULT_CODE(hr)    ((hr) & 0xFFFF)
#define SCODE_CODE(sc)      ((sc) & 0xFFFF)
#define HRESULT_FACILITY(hr)  (((hr) >> 16) & 0x1fff)
#define SCODE_FACILITY(sc)    (((sc) >> 16) & 0x1fff)
#define HRESULT_SEVERITY(hr)  (((hr) >> 31) & 0x1)
#define SCODE_SEVERITY(sc)    (((sc) >> 31) & 0x1)

// both macros diff from Win32
#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((ULONG)(sev)<<31) | ((ULONG)(fac)<<16) | ((ULONG)(code))) )
#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((ULONG)(sev)<<31) | ((ULONG)(fac)<<16) | ((LONG)(code))) )

#define FACILITY_NT_BIT                 0x10000000
#define HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))
#define __HRESULT_FROM_WIN32(x) HRESULT_FROM_WIN32(x)

#define HRESULT_FROM_NT(x)      ((HRESULT) ((x) | FACILITY_NT_BIT))

/******************* OLE, BSTR, VARIANT *************************/

STDAPI_(LPVOID) CoTaskMemAlloc(SIZE_T cb);
STDAPI_(void) CoTaskMemFree(LPVOID pv);

typedef SHORT VARIANT_BOOL;
#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

typedef WCHAR OLECHAR;
typedef OLECHAR* LPOLESTR;
typedef const OLECHAR* LPCOLESTR;

typedef WCHAR *BSTR;

STDAPI_(BSTR) SysAllocString(const OLECHAR*);
STDAPI_(BSTR) SysAllocStringLen(const OLECHAR*, UINT);
STDAPI_(BSTR) SysAllocStringByteLen(const char *, UINT);
STDAPI_(void) SysFreeString(BSTR);
STDAPI_(UINT) SysStringLen(BSTR);
STDAPI_(UINT) SysStringByteLen(BSTR);

typedef double DATE;

typedef union tagCY {
    struct {
#if BIGENDIAN
        LONG    Hi;
        ULONG   Lo;
#else
        ULONG   Lo;
        LONG    Hi;
#endif
    } u;
    LONGLONG int64;
} CY, *LPCY;

typedef CY CURRENCY;

typedef struct tagDEC {
    // Decimal.cs treats the first two shorts as one long
    // And they seriable the data so we need to little endian
    // seriliazation
    // The wReserved overlaps with Variant's vt member
#if BIGENDIAN
    union {
        struct {
            BYTE sign;
            BYTE scale;
        } u;
        USHORT signscale;
    } u;
    USHORT wReserved;
#else
    USHORT wReserved;
    union {
        struct {
            BYTE scale;
            BYTE sign;
        } u;
        USHORT signscale;
    } u;
#endif
    ULONG Hi32;
    union {
        struct {
            ULONG Lo32;
            ULONG Mid32;
        } v;
        ULONGLONG Lo64;
    } v;
} DECIMAL, *LPDECIMAL;

#define DECIMAL_NEG ((BYTE)0x80)
#define DECIMAL_SCALE(dec)       ((dec).u.u.scale)
#define DECIMAL_SIGN(dec)        ((dec).u.u.sign)
#define DECIMAL_SIGNSCALE(dec)   ((dec).u.signscale)
#define DECIMAL_LO32(dec)        ((dec).v.v.Lo32)
#define DECIMAL_MID32(dec)       ((dec).v.v.Mid32)
#define DECIMAL_HI32(dec)        ((dec).Hi32)
#define DECIMAL_LO64_GET(dec)    ((dec).v.Lo64)
#define DECIMAL_LO64_SET(dec,value)   {(dec).v.Lo64 = value; }

#define DECIMAL_SETZERO(dec) {DECIMAL_LO32(dec) = 0; DECIMAL_MID32(dec) = 0; DECIMAL_HI32(dec) = 0; DECIMAL_SIGNSCALE(dec) = 0;}

typedef struct tagBLOB {
    ULONG cbSize;
    BYTE *pBlobData;
} BLOB, *LPBLOB;

interface IStream;
interface IRecordInfo;

typedef unsigned short VARTYPE;

enum VARENUM {
    VT_EMPTY    = 0,
    VT_NULL = 1,
    VT_I2   = 2,
    VT_I4   = 3,
    VT_R4   = 4,
    VT_R8   = 5,
    VT_CY   = 6,
    VT_DATE = 7,
    VT_BSTR = 8,
    VT_DISPATCH = 9,
    VT_ERROR    = 10,
    VT_BOOL = 11,
    VT_VARIANT  = 12,
    VT_UNKNOWN  = 13,
    VT_DECIMAL  = 14,
    VT_I1   = 16,
    VT_UI1  = 17,
    VT_UI2  = 18,
    VT_UI4  = 19,
    VT_I8   = 20,
    VT_UI8  = 21,
    VT_INT  = 22,
    VT_UINT = 23,
    VT_VOID = 24,
    VT_HRESULT  = 25,
    VT_PTR  = 26,
    VT_SAFEARRAY    = 27,
    VT_CARRAY   = 28,
    VT_USERDEFINED  = 29,
    VT_LPSTR    = 30,
    VT_LPWSTR   = 31,
    VT_RECORD   = 36,

    VT_FILETIME        = 64,
    VT_BLOB            = 65,
    VT_STREAM          = 66,
    VT_STORAGE         = 67,
    VT_STREAMED_OBJECT = 68,
    VT_STORED_OBJECT   = 69,
    VT_BLOB_OBJECT     = 70,
    VT_CF              = 71,
    VT_CLSID           = 72,

    VT_VECTOR   = 0x1000,
    VT_ARRAY    = 0x2000,
    VT_BYREF    = 0x4000,
    VT_TYPEMASK = 0xfff,
};

typedef struct tagVARIANT VARIANT, *LPVARIANT;

struct tagVARIANT
    {
    union
        {
        struct
            {
#if BIGENDIAN
            // We need to make sure vt overlaps with DECIMAL's wReserved.
            // See the DECIMAL type for details.
            WORD wReserved1;
            VARTYPE vt;
#else
            VARTYPE vt;
            WORD wReserved1;
#endif
            WORD wReserved2;
            WORD wReserved3;
            union
                {
                LONGLONG llVal;
                LONG lVal;
                BYTE bVal;
                SHORT iVal;
                FLOAT fltVal;
                DOUBLE dblVal;
                VARIANT_BOOL boolVal;
                SCODE scode;
                CY cyVal;
                DATE date;
                BSTR bstrVal;
                interface IUnknown *punkVal;
                BYTE *pbVal;
                SHORT *piVal;
                LONG *plVal;
                LONGLONG *pllVal;
                FLOAT *pfltVal;
                DOUBLE *pdblVal;
                VARIANT_BOOL *pboolVal;
                SCODE *pscode;
                CY *pcyVal;
                DATE *pdate;
                BSTR *pbstrVal;
                interface IUnknown **ppunkVal;
                VARIANT *pvarVal;
                PVOID byref;
                CHAR cVal;
                USHORT uiVal;
                ULONG ulVal;
                ULONGLONG ullVal;
                INT intVal;
                UINT uintVal;
                DECIMAL *pdecVal;
                CHAR *pcVal;
                USHORT *puiVal;
                ULONG *pulVal;
                ULONGLONG *pullVal;
                INT *pintVal;
                UINT *puintVal;
                struct __tagBRECORD
                    {
                    PVOID pvRecord;
                    interface IRecordInfo *pRecInfo;
                    } brecVal;
                } n3;
            } n2;
        DECIMAL decVal;
        } n1;
    };

typedef VARIANT VARIANTARG, *LPVARIANTARG;

STDAPI_(void) VariantInit(VARIANT * pvarg);
STDAPI_(HRESULT) VariantClear(VARIANT * pvarg);

#define V_VT(X)         ((X)->n1.n2.vt)
#define V_UNION(X, Y)   ((X)->n1.n2.n3.Y)
#define V_RECORDINFO(X) ((X)->n1.n2.n3.brecVal.pRecInfo)
#define V_RECORD(X)     ((X)->n1.n2.n3.brecVal.pvRecord)

#define V_UI1(X)         V_UNION(X, bVal)
#define V_UI1REF(X)      V_UNION(X, pbVal)
#define V_I2(X)          V_UNION(X, iVal)
#define V_I2REF(X)       V_UNION(X, piVal)
#define V_I4(X)          V_UNION(X, lVal)
#define V_I4REF(X)       V_UNION(X, plVal)
#define V_I8(X)          V_UNION(X, llVal)
#define V_I8REF(X)       V_UNION(X, pllVal)
#define V_R4(X)          V_UNION(X, fltVal)
#define V_R4REF(X)       V_UNION(X, pfltVal)
#define V_R8(X)          V_UNION(X, dblVal)
#define V_R8REF(X)       V_UNION(X, pdblVal)
#define V_I1(X)          V_UNION(X, cVal)
#define V_I1REF(X)       V_UNION(X, pcVal)
#define V_UI2(X)         V_UNION(X, uiVal)
#define V_UI2REF(X)      V_UNION(X, puiVal)
#define V_UI4(X)         V_UNION(X, ulVal)
#define V_UI4REF(X)      V_UNION(X, pulVal)
#define V_UI8(X)         V_UNION(X, ullVal)
#define V_UI8REF(X)      V_UNION(X, pullVal)
#define V_INT(X)         V_UNION(X, intVal)
#define V_INTREF(X)      V_UNION(X, pintVal)
#define V_UINT(X)        V_UNION(X, uintVal)
#define V_UINTREF(X)     V_UNION(X, puintVal)

#ifdef _WIN64
#define V_INT_PTR(X)        V_UNION(X, llVal)
#define V_UINT_PTR(X)       V_UNION(X, ullVal)
#define V_INT_PTRREF(X)     V_UNION(X, pllVal)
#define V_UINT_PTRREF(X)    V_UNION(X, pullVal)
#else
#define V_INT_PTR(X)        V_UNION(X, lVal)
#define V_UINT_PTR(X)       V_UNION(X, ulVal)
#define V_INT_PTRREF(X)     V_UNION(X, plVal)
#define V_UINT_PTRREF(X)    V_UNION(X, pulVal)
#endif

#define V_CY(X)          V_UNION(X, cyVal)
#define V_CYREF(X)       V_UNION(X, pcyVal)
#define V_DATE(X)        V_UNION(X, date)
#define V_DATEREF(X)     V_UNION(X, pdate)
#define V_BSTR(X)        V_UNION(X, bstrVal)
#define V_BSTRREF(X)     V_UNION(X, pbstrVal)
#define V_UNKNOWN(X)     V_UNION(X, punkVal)
#define V_UNKNOWNREF(X)  V_UNION(X, ppunkVal)
#define V_VARIANTREF(X)  V_UNION(X, pvarVal)
#define V_ERROR(X)       V_UNION(X, scode)
#define V_ERRORREF(X)    V_UNION(X, pscode)
#define V_BOOL(X)        V_UNION(X, boolVal)
#define V_BOOLREF(X)     V_UNION(X, pboolVal)
#define V_BYREF(X)       V_UNION(X, byref)

#define V_DECIMAL(X)     ((X)->n1.decVal)
#define V_DECIMALREF(X)    V_UNION(X, pdecVal)

#define V_ISBYREF(X)     (V_VT(X)&VT_BYREF)

STDAPI CreateStreamOnHGlobal(PVOID hGlobal, BOOL fDeleteOnRelease, interface IStream** ppstm);

#define STGM_DIRECT             0x00000000L

#define STGM_READ               0x00000000L
#define STGM_WRITE              0x00000001L
#define STGM_READWRITE          0x00000002L

#define STGM_SHARE_DENY_NONE    0x00000040L
#define STGM_SHARE_DENY_READ    0x00000030L
#define STGM_SHARE_DENY_WRITE   0x00000020L
#define STGM_SHARE_EXCLUSIVE    0x00000010L

#define STGM_DELETEONRELEASE    0x04000000L

#define STGM_CREATE             0x00001000L
#define STGM_CONVERT            0x00020000L
#define STGM_FAILIFTHERE        0x00000000L

#define STGM_NOSNAPSHOT         0x00200000L

STDAPI IIDFromString(LPOLESTR lpsz, IID* lpiid);
STDAPI_(int) StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax); 

STDAPI CoCreateGuid(OUT GUID * pguid);

#define STARTF_USESTDHANDLES       0x00000100

typedef struct _STARTUPINFOW {
    DWORD cb;
    LPWSTR lpReserved_PAL_Undefined;
    LPWSTR lpDesktop_PAL_Undefined;
    LPWSTR lpTitle_PAL_Undefined;
    DWORD dwX_PAL_Undefined;
    DWORD dwY_PAL_Undefined;
    DWORD dwXSize_PAL_Undefined;
    DWORD dwYSize_PAL_Undefined;
    DWORD dwXCountChars_PAL_Undefined;
    DWORD dwYCountChars_PAL_Undefined;
    DWORD dwFillAttribute_PAL_Undefined;
    DWORD dwFlags;
    WORD wShowWindow_PAL_Undefined;
    WORD cbReserved2_PAL_Undefined;
    LPBYTE lpReserved2_PAL_Undefined;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef STARTUPINFOW STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;

#define CREATE_NEW_CONSOLE          0x00000010
#define NORMAL_PRIORITY_CLASS             0x00000020

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId_PAL_Undefined;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;