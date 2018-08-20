// URLEdit.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "URLEdit.h"

namespace Control {
    // URLEdit

    IMPLEMENT_DYNAMIC(URLEdit, CEdit)

        URLEdit::URLEdit()
    {

    }

    URLEdit::~URLEdit()
    {
    }


    BEGIN_MESSAGE_MAP(URLEdit, CEdit)
        ON_WM_CHAR()
    END_MESSAGE_MAP()



    // URLEdit message handlers

    void URLEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        if ((nChar < 48 || nChar > 57) && nChar != 45 && nChar != 43)
        {
            //AfxMessageBox( "you must type the number or the char '-' or the char '+'" );
            return;
        }
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}
