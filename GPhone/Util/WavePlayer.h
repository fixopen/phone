#ifndef __UTIL_WAVEPLAYER_H__
#define __UTIL_WAVEPLAYER_H__

#include <mmsystem.h>
#include <string>
#include "SmartPtr.h"
#include "WaveUtil.h"
#include "../Resource.h"

namespace Util {
    class WavePlayer : public CDialog {
        DECLARE_DYNAMIC(WavePlayer)

    public:
        static WavePlayer* const Instance();
        WavePlayer(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        enum { IDD = IDD_DIALOG_WAVE_PLAYER };

        bool const Start(std::wstring const& filename);
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
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        afx_msg LRESULT OnMM_WOM_OPEN(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnMM_WOM_DONE(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnMM_WOM_CLOSE(WPARAM wParam, LPARAM lParam);
        void (*notify_)();

        DECLARE_MESSAGE_MAP()
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
        OggCodec* oggCodec_;
    };
}

#endif //__UTIL_WAVEPLAYER_H__
