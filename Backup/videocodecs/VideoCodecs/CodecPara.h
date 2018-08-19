#ifndef __UTIL_VIDEO_CODECPARA_H__
#define __UTIL_VIDEO_CODECPARA_H__

namespace Util {
    namespace Video {
        struct Para {
            unsigned int uiWidth;
            unsigned int uiHeight;
            unsigned int uiFramerate;
            unsigned int uiBitrate_kbps;
            unsigned int uiQuantIntra;
            unsigned int uiQuantInter;
            unsigned int uiGOPNum;
        };
    }
}

#endif //__UTIL_VIDEO_CODECPARA_H__
