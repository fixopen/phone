#pragma once


// List view
#include <afxcview.h>

class List : public CListView {
    DECLARE_DYNCREATE(List)

protected:
    List();           // protected constructor used by dynamic creation
    virtual ~List();

public:
#ifdef _DEBUG
    virtual void AssertValid() const;
#ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
    DECLARE_MESSAGE_MAP()
};


