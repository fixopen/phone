#include "BusyToneDetectThread.h"
#include "Telephone.h"

namespace Telephone {
    namespace Logical {
        int const BusyToneDetectThread::run() {
            prepare_();
            while (!quit_) {
                if (active_) {
                    detect_();
                    active_ = false;
                    Sleep(0);
                } else {
                    Sleep(100);
                }
            }
            finally_();
            return 0;
        }

        void BusyToneDetectThread::prepare_() {
            detectLib_ = LoadLibrary(L"/FlashDrv/res_dat/SABTD_CEDLL.dll");
            if (detectLib_ != NULL) {
                btd_init = (SabtdInit)GetProcAddress(detectLib_, L"SABTD_Init");
                btd_uninit = (SabtdUnInit)GetProcAddress(detectLib_, L"SABTD_UnInit");
                btd_set_param = (SabtdSetParam)GetProcAddress(detectLib_, L"SABTD_SetParam");
                btd_get_param = (SabtdGetParam)GetProcAddress(detectLib_, L"SABTD_GetParam");
                btd_reset = (SabtdReset)GetProcAddress(detectLib_, L"SABTD_Reset");
                btd_process = (SabtdProcess)GetProcAddress(detectLib_, L"SABTD_Process");

                if (btd_init && btd_uninit && btd_set_param && btd_get_param && btd_reset && btd_process) {
                    int initResult = btd_init(&detectStruct_, SAMPLESPERSEC, "/FlashDrv/res_dat/sa_license.dat");
                    if ((initResult == -1000) || (initResult < 0 || detectStruct_ == NULL)) {
                        btd_init = NULL;
                        btd_uninit = NULL;
                        btd_set_param = NULL;
                        btd_get_param = NULL;
                        btd_reset = NULL;
                        btd_process = NULL;
                    } else {
                        if (initResult == SARTC_NORMAL) {
                            btd_set_param(detectStruct_, SABTD_PARAMTYPE_ENERGY, 1.0);
                            btd_set_param(detectStruct_, SABTD_PARAMTYPE_FRONT_NOTTONE, 0);
                            btd_set_param(detectStruct_, SABTD_PARAMTYPE_BUSYTONE_NUM, 3);
                            btd_set_param(detectStruct_, SABTD_PARAMTYPE_SILENCE_LENGTH, 5);
                        }
                    }
                }
            }
        }
        void BusyToneDetectThread::finally_() {
            if (btd_reset != NULL) {
                btd_reset(detectStruct_);
            }
            //EnterCriticalSection(&mutex_);
            //int s = detectQueue_.size();
            //for (int i = 0; i < s; ++i) {
            //    Util::OggCodec::PcmData* m = detectQueue_.front();
            //    //free(m->data);
            //    //delete m;
            //    detectQueue_.pop();
            //}
            //detectQueue_.clear();
            //LeaveCriticalSection(&mutex_);
            //CloseHandle(detectThread);
            //detectThread = NULL;
            //return 0;
            FreeLibrary(detectLib_);
        }
        void BusyToneDetectThread::detect_() {
            while (!isVoiceStop_) {
                EnterCriticalSection(&mutex_);
                bool isEmpty = detectQueue_.empty();
                LeaveCriticalSection(&mutex_);
                if (!isEmpty) {
                    //Copy the voice data
                    EnterCriticalSection(&mutex_);
                    Util::OggCodec::PcmData* m = detectQueue_.front();
                    memset(voiceData_, maxVoiceDataLength * sizeof(short), 0);
                    memcpy(voiceData_, m->data, m->dataLength * sizeof(short));
                    voiceLength_ = m->dataLength;
                    //free(m->data);
                    //delete m;
                    detectQueue_.pop();
                    LeaveCriticalSection(&mutex_);

                    //detect the voice data
                    int toneDetected = 0;
                    if (btd_process != NULL) {
                        if (btd_process(detectStruct_, &toneDetected, voiceData_, voiceLength_) < 0) {
                            if (btd_reset != NULL) {
                                btd_reset(detectStruct_);
                            }
                        }
                    }
                    if ((toneDetected == SABTD_TONE_DETECTED) /*|| (toneDetected == SABTD_SILENCE_DETECTED)*/) {  //¾²ÒôÆÁ±Î
                        if (btd_reset != NULL) {
                            btd_reset(detectStruct_);
                        }
                        isVoiceStop_ = true;
                        Telephone::Instance()->fireEvent(Telephone::eRemoteHangup, 0);
                    }
                }
                Sleep(0);
            }
        }
    }
}
