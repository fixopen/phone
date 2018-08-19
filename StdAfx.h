// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_)
#define AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#if defined(_WIN32_WCE) && (_WIN32_WCE >= 211) && (_AFXDLL)
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

int  Dprintf(const char *format, ...);
int  DMemprintf(char *title, DWORD &t, DWORD &a);
int DMemprintf(char *title);
void DWriteLog(char *tile, unsigned char *pData, int len);
void DWriteDat(char *tile, unsigned char *pData, int len);
//void Dprintf(CString s);

inline void _invalid_parameter_noinfo() {
}

#define for if(true) for
//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_)
