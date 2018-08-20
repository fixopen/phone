// TransparentStatic.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "TransparentStatic.h"


// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)

CTransparentStatic::CTransparentStatic() {
}

CTransparentStatic::~CTransparentStatic() {
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
END_MESSAGE_MAP()



// CTransparentStatic message handlers


BOOL CTransparentStatic::OnEraseBkgnd(CDC* pDC) {
    return TRUE;
}

int CTransparentStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    //SetClassLong(m_hWnd, GCL_HBRBACKGROUND,(long)GetStockObject(NULL_BRUSH));
    return 0;
}