//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
//

//
// ===========================================================================
// File: rpc.h
// 
// =========================================================================== 
// dummy rpc.h for PAL

#ifndef __RPC_H__
#define __RPC_H__

#include "palrt.h"

#define __declspec(x)

#define __RPCNDR_H_VERSION__
#define __RPC_STUB
#define __RPC_USER
#define __RPC_FAR

typedef void *LPSECURITY_ATTRIBUTES;

#define DECLSPEC_UUID(x) __declspec(uuid(x))
#define MIDL_INTERFACE(x)   struct DECLSPEC_UUID(x) __declspec(novtable)

#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8) \
    EXTERN_C const IID DECLSPEC_SELECTANY itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}

interface IRpcStubBuffer;
interface IRpcChannelBuffer;

typedef void* PRPC_MESSAGE;
typedef void* RPC_IF_HANDLE;

#define UNALIGNED

struct ITypeInfo;

#endif // __RPC_H__
