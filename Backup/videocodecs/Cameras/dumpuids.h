//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//
//--------------------------------------------------------------------------;
#pragma once
#include <strmif.h>
// The CLSID used by the dump filter
// {41F3D573-C28F-4a50-A3E3-160226E56EC9}
DEFINE_GUID(CLSID_NullRender, 
0x41f3d573, 0xc28f, 0x4a50, 0xa3, 0xe3, 0x16, 0x2, 0x26, 0xe5, 0x6e, 0xc9);


// ISampleGrabber interface definition
#ifdef __cplusplus
extern "C" {
#endif 
	// {C85F4AAF-69D8-4d86-B6B8-2E9FADE7E7A3}
	DEFINE_GUID(IID_INullRender, 
	0xc85f4aaf, 0x69d8, 0x4d86, 0xb6, 0xb8, 0x2e, 0x9f, 0xad, 0xe7, 0xe7, 0xa3);

	DECLARE_INTERFACE_(INullRender,IFileSinkFilter) {
	//	STDMETHOD(RegisterCallback)(MANAGEDCALLBACKPROC callback) PURE;
		    // Write data streams to a file
	    STDMETHOD (Write)(PBYTE pbData,LONG lData) PURE;
	};
#ifdef __cplusplus
}
#endif
// {34BA2A3F-640B-412e-8E25-1A24EF1F3E42}

