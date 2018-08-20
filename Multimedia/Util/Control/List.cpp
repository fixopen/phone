#include "List.h"

namespace Util {
    namespace Control {
        // List

        IMPLEMENT_DYNCREATE(List, CListView)

        List::List() {
        }

        List::~List() {
        }

        BEGIN_MESSAGE_MAP(List, CListView)
        END_MESSAGE_MAP()


        // List diagnostics

#ifdef _DEBUG
        void List::AssertValid() const {
            CListView::AssertValid();
        }

#ifndef _WIN32_WCE
        void List::Dump(CDumpContext& dc) const {
            CListView::Dump(dc);
        }
#endif
#endif //_DEBUG


        // List message handlers
    }
}
