#include "ImageViewer.h"
#include "../Image/ImageOp.h"

namespace Util {
    namespace Control {
        // ImageViewer

        IMPLEMENT_DYNCREATE(ImageViewer, CView)

        ImageViewer::ImageViewer() {
        }

        ImageViewer::~ImageViewer() {
        }

        BEGIN_MESSAGE_MAP(ImageViewer, CView)
        END_MESSAGE_MAP()


        // ImageViewer drawing

        void ImageViewer::OnDraw(CDC* pDC) {
            CDocument* pDoc = GetDocument();
            // TODO: add draw code here
            //Util::ImageOp::GetImage(filename_);
        }


        // ImageViewer diagnostics

#ifdef _DEBUG
        void ImageViewer::AssertValid() const {
            CView::AssertValid();
        }

#ifndef _WIN32_WCE
        void ImageViewer::Dump(CDumpContext& dc) const {
            CView::Dump(dc);
        }
#endif
#endif //_DEBUG


        // ImageViewer message handlers
    }
}
