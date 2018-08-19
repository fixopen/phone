#if !defined(AFX_NEWSOCKET_H__BDEE503B_0FD4_40D7_9279_F199E961E08E__INCLUDED_)
#define AFX_NEWSOCKET_H__BDEE503B_0FD4_40D7_9279_F199E961E08E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewSocket.h : header file
//

//#include <wcesock.h>
#include <afxsock.h>
/////////////////////////////////////////////////////////////////////////////
// CNewSocket command target

class CNewSocket : public CSocket/*CCeSocket*/ {
    // Attributes
public:

    // Operations
public:
    CNewSocket();
    virtual ~CNewSocket();

    // Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewSocket)
    public:
    virtual void OnAccept(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
    virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CNewSocket)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    // Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSOCKET_H__BDEE503B_0FD4_40D7_9279_F199E961E08E__INCLUDED_)
