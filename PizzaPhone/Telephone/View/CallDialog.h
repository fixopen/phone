#ifndef __TELEPHONE_VIEW_CALLDIALOG_H__
#define __TELEPHONE_VIEW_CALLDIALOG_H__

#include <map>

#include "../../Resource.h" //for IDD_DIALOG_CALL
#include "../Data/CallInfo.h" //for CallInfo
#include "../Logical/CallId.h" //for CallId
#include "../../Control/ImageButton.h" //for ImageButton

namespace Telephone {
    namespace View {
        enum MessageCode {
            UM_TEL_DIAL = (WM_USER + 1000),
            UM_TEL_PICKUP_TIMEOUT,
            UM_TEL_DIAL_END,
            UM_TEL_CALLID,
            UM_TEL_CONNECT,
            UM_TEL_START_RECORD,
            UM_TEL_RING,
            UM_TEL_CALLID_TIMEOUT,
            UM_TEL_SUB_DIAL,
            UM_TEL_STOP_RECORD,
            UM_TEL_REMOTE_HANGUP,
            UM_TEL_CONNECT_TIMEOUT,
            UM_TEL_REMOTE_BUSY,
            UM_TEL_CALLIN_TIMEOUT,
            UM_TEL_DIAL_TIMEOUT,
            UM_TEL_RECORD_TIMEOUT,
            UM_TEL_SOFTDIAL,
            UM_TEL_PICKUP,
            UM_TEL_HANGUP,
            UM_TEL_STORAGE_FULL,
            UM_TEL_MAX_DURATION_OVER,
        };

        // CallDialog dialog

        class CallDialog : public CDialog {
            DECLARE_DYNAMIC(CallDialog)

        public:
            CallDialog(CWnd* pParent = NULL);   // standard constructor
            virtual ~CallDialog();

            // Dialog Data
            enum { IDD = IDD_DIALOG_CALL };

        protected:
            virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

            DECLARE_MESSAGE_MAP()
        private:
            std::map<int, int> timers_;
            CWnd* listener_;
            bool hasCallId_;
            CFont font_;
            Control::ImageButton closeButton_;
            Control::ImageButton hangupButton_;
            Control::ImageButton hiddenButton_;
            Control::ImageButton rejectButton_;
            Control::ImageButton listenButton_;
            Control::ImageButton callButton_;
            Control::ImageButton voiceMessagesButton_;
        public:
            HWND const GetLocalDisplay() const;
            HWND const GetRemoteDisplay() const;
            void SetNumber(std::wstring const& number);
            void SetName(std::wstring const& name);
            void SetAddress(std::wstring const& address);
            void SetStartTime(Util::Timestamp const& time);
            void SetRecordStartTime(Util::Timestamp const& time);
            void ClearDuration();
            void SetListener(CWnd* listener) {
                listener_ = listener;
            }
            afx_msg void OnDestroy();
            afx_msg void OnBnClickedButtonClose();
        protected:
            virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        public:
            afx_msg void OnBnClickedButtonReject();
            afx_msg void OnBnClickedButtonListen();
            afx_msg void OnBnClickedButtonHangup();
            afx_msg void OnBnClickedButtonHidden();
            afx_msg void OnBnClickedButtonNewContact();
            afx_msg void OnBnClickedButtonMute();
            afx_msg void OnBnClickedButtonStartRecord();
            afx_msg void OnBnClickedButtonStopRecord();
            void dial_(wchar_t const keyCode);
            void pickupTimeout_();
            void callId_(Telephone::Logical::CALLID_INFO* callId);
            void connect_(void);
            void startRecord_(void);
            void ring_(void);
            void callIdTimeout_(void);
            void subDial_(wchar_t const keyCode);
            void stopRecord_(void);
            void remoteHangup_(void);
            void connectTimeout_(void);
            void remoteBusy_(void);
            void callinTimeout_(void);
            void dialTimeout_(void);
            void recordTimeout_(void);
            void softDial_(std::wstring const& number);
            void pickup_(void);
            void hangup_();
            afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
            afx_msg void OnBnClickedButtonVoiceMessages();
            afx_msg void OnTimer(UINT_PTR nIDEvent);
            afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
            afx_msg BOOL OnEraseBkgnd(CDC* pDC);
            virtual BOOL OnInitDialog();
        };

        extern CallDialog* const callForm();
    }
}

#endif //__TELEPHONE_VIEW_CALLDIALOG_H__
