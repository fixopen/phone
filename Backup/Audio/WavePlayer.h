#ifndef __UTIL_AUDIO_WAVEPLAYER_H__
#define __UTIL_AUDIO_WAVEPLAYER_H__

#include <windef.h>
#include <mmsystem.h>
#include <string>
#include "WaveUtil.h"
#include "../SmartPtr.h"
//#include "../../Resource.h"

//OggFileReader->SpeexDecode->Device
namespace Util {
    namespace Audio {
        class WavePlayer/* : public CDialog*/ {
            //DECLARE_DYNAMIC(WavePlayer)

        public:
            static WavePlayer* const Instance();
            WavePlayer(/*CWnd* pParent = NULL*/);   // standard constructor

            // Dialog Data
            //enum { IDD = IDD_DIALOG_WAVE_PLAYER };

            void SetDecode(SpeexCodec* const speexCodec) {
                speexCodec_ = speexCodec;
            }
            void (*EatData)(void const* const data, size_t const length);
            void InsertData(void const* const data, size_t const length);
            bool const Start(std::wstring const& filename = L"");
            bool const Pause();
            bool const Resume();
            bool const Stop();
            void SetStopNotify(void (*notify)()) {
                notify_ = notify;
            }
            bool const IsPlaying() const {
                return isPlaying_;
            }
        protected:
            //virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
            //afx_msg LRESULT OnMM_WOM_OPEN(WPARAM wParam, LPARAM lParam);
            //afx_msg LRESULT OnMM_WOM_DONE(WPARAM wParam, LPARAM lParam);
            //afx_msg LRESULT OnMM_WOM_CLOSE(WPARAM wParam, LPARAM lParam);
            void (*notify_)();

            //DECLARE_MESSAGE_MAP()
        private:
            HWAVEOUT waveOut_;
            unsigned char* buffer_;
            PWAVEHDR waveHeader_;
            WAVEFORMATEX waveFormat_;
            bool isPlaying_;
            bool isLeaveWord_;
            volatile bool isEnding_;
            int dataCount_;
            DWORD dataLength_;
            //OggCodec* oggCodec_;
            SpeexCodec* speexCodec_;
            CRITICAL_SECTION criticalSection_;
        };
    }
}

#endif //__UTIL_AUDIO_WAVEPLAYER_H__
