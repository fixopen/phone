#pragma once

namespace Control {
    // URLEdit

    class URLEdit : public CEdit
    {
        DECLARE_DYNAMIC(URLEdit)

    public:
        URLEdit();
        virtual ~URLEdit();

    protected:
        DECLARE_MESSAGE_MAP()
        afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    };
}

