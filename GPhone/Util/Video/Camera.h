#ifndef __UTIL_VIDEO_CAMERA_H__
#define __UTIL_VIDEO_CAMERA_H__

namespace Util {
    namespace Video {
        typedef void (__stdcall *CameraDataProc)(
        unsigned char const * const data,
        unsigned int const length);

        struct CameraParameters {
            CameraParameters(CameraDataProc const callBack = 0)
            : width(176)
            , height(144)
            , frameRate(20)
            , sampleCallBack(callBack) {
            }
            int width;
            int height;
            unsigned int frameRate;
            CameraDataProc sampleCallBack;
        };

        class Camera {
        public:
            virtual ~Camera() = 0 {
            }
            bool const SetParameters(CameraParameters const & params) {
                parameters_ = params;
                return true;
            }
            virtual bool const Open() = 0;
            virtual bool const Start() = 0;
            virtual bool const Stop() = 0;
            virtual bool const Close() = 0;
        private:
            CameraParameters parameters_;
        };
    }
}

#endif //__UTIL_VIDEO_CAMERA_H__
