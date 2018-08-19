// NewSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NewSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewSocket

CNewSocket::CNewSocket()
{
}

CNewSocket::~CNewSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CNewSocket, CCeSocket)
	//{{AFX_MSG_MAP(CNewSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CNewSocket member functions

void CNewSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CCeSocket::OnAccept(nErrorCode);
}

void CNewSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

//	char buf[1024*16] = {0};
//	int len = Receive(buf, 1024*16);
/*
	FILE *fl = fopen("/flashdrv/123.dat", "w+b");
	if(fl)
	{
		fwrite(buf, sizeof(char), len, fl);
		fclose(fl);
	}
*/
//	CCeSocket::OnReceive(nErrorCode);

}

int CNewSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CCeSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CNewSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CCeSocket::Send(lpBuf, nBufLen, nFlags);
}
