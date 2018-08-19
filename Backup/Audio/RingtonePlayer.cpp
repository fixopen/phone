#include "RingtonePlayer.h"

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Strmiids.lib")

namespace Util {
    namespace Audio {
        RingtonePlayer* const RingtonePlayer::Instance() {
            static RingtonePlayer* result = 0;
            if (!result) {
                result = new RingtonePlayer();
                result->start();
            }
            return result;
        }

        int const RingtonePlayer::run() {
            while (!isQuit_) {
                if (isActive_) {
                    if (mediaControl_
                        && graphBuilder_
                        && basicAudio_
                        && mediaEvent_
                        && (filename_ != L"")
                        && loopCount_ ) { //循环播放
                            int loopCount = 0;
                            while (loopCount < loopCount_) {
                                long evCode;
                                mediaEvent_->WaitForCompletion(5, &evCode);
                                switch (evCode) {
                        case EC_COMPLETE:
                            ++loopCount;
                            mediaPosition_->put_CurrentPosition(0);
                            mediaControl_->Run();
                            break;
                        case EC_ERRORABORT:
                            break;
                        case EC_USERABORT:
                            break;
                        case 0: //not complete
                            break;
                        default:
                            break;
                                }
                                if (!isActive_) {
                                    goto endOfPlay;
                                }
                            }
endOfPlay:
                            Stop();
                    }
                    isActive_ = false;
                } else {
                    Sleep(50);
                }
            }
            return 0;
        }

        RingtonePlayer::RingtonePlayer()
            : isQuit_(false)
            , isActive_(false)
            , graphBuilder_(0)
            , mediaControl_(0)
            , mediaPosition_(0)
            , basicAudio_(0)
            , mediaEvent_(0)
            , loopCount_(0xFF) {
                //startThread_();
                CoInitialize(NULL); 
                init_();
        }

        RingtonePlayer::~RingtonePlayer() {
            release_();
            willStop();
            Stop();
            //stopThread_();
            CoUninitialize();
        }
#if 0
        void RingtonePlayer::threadFunction_() {
            while (!RingtonePlayer::Instance()->isQuit_) {
                if (RingtonePlayer::Instance()->isActive_) {
                    if (RingtonePlayer::Instance()->mediaControl_
                        && RingtonePlayer::Instance()->graphBuilder_
                        && RingtonePlayer::Instance()->basicAudio_
                        && RingtonePlayer::Instance()->mediaEvent_
                        && (RingtonePlayer::Instance()->filename_ != L"")
                        && RingtonePlayer::Instance()->loopCount_ ) { //循环播放
                            long evCode;
                            RingtonePlayer::Instance()->mediaEvent_->WaitForCompletion(5, &evCode);
                            if (evCode != 0) {
                                --RingtonePlayer::Instance()->loopCount_;
                                if (RingtonePlayer::Instance()->loopCount_) {
                                    RingtonePlayer::Instance()->mediaPosition_->put_CurrentPosition(0);
                                    RingtonePlayer::Instance()->mediaControl_->Run();
                                } else {
                                    RingtonePlayer::Instance()->Stop();
                                }
                            }
                    }
                } else {
                    Sleep(50);
                }
            }
        }

        void RingtonePlayer::startThread_() {
            CoInitialize(NULL); 
            playThread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFunction_, NULL, 0, NULL);
        }

        void RingtonePlayer::stopThread_() {
            CloseHandle(playThread_);
        }
#endif
        void RingtonePlayer::init_() {
            //以下程序每次播放MP3(或其他格式)文件时调用
            //创建一个Filter Graph manager
            CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graphBuilder_);
            graphBuilder_->QueryInterface(IID_IMediaControl, (void**)&mediaControl_);
            graphBuilder_->QueryInterface(IID_IMediaPosition, (void**)&mediaPosition_);

            //得到音频相关接口
            graphBuilder_->QueryInterface(IID_IBasicAudio, (void**)&basicAudio_);
            graphBuilder_->QueryInterface(IID_IMediaEvent, (void**)&mediaEvent_);
        }

        void RingtonePlayer::release_() {
            //释放对象
            if (mediaControl_) {
                mediaControl_->Stop();
            }
            if (mediaEvent_) {
                mediaEvent_->Release();
                mediaEvent_ = NULL;
            }
            if (basicAudio_) {
                basicAudio_->Release();
                basicAudio_ = NULL;
            }
            if (mediaPosition_) {
                mediaPosition_->Release();
                mediaPosition_ = NULL;
            }
            if (mediaControl_) {
                mediaControl_->Release();
                mediaControl_ = NULL;
            }
            if (graphBuilder_) {
                graphBuilder_->Release();
                graphBuilder_ = NULL;
            }
        }

        void RingtonePlayer::Start(std::wstring const& filename, int loopCount) {
            filename_ = filename;
            if (filename_ == L"") {
                Stop();
            } else {
                ::Sleep(50);
                ::EnterCriticalSection(&criticalSection_);
                loopCount_ = loopCount;
                //release_();
                //init_();
                ::LeaveCriticalSection(&criticalSection_);
                isActive_ = true;
                if (graphBuilder_ && mediaControl_) {
                    //连接播放指定文件所需的filter
                    graphBuilder_->RenderFile(filename_.c_str(), NULL);
                    //开始播放指定的文件
                    mediaControl_->Run();
                    if (basicAudio_) {
                        waveOutSetVolume(NULL, volume_);
                    }
                }
                playing_ = true;
            }
        }

        void RingtonePlayer::Stop(bool const release) {
            ::EnterCriticalSection(&criticalSection_);
            isActive_ = false;
            mediaControl_->Stop();
            if (playing_) {
                filename_ = L"";
                //if (release) {
                //    release_();
                //}
                if (stopNotify_) {
                    stopNotify_();
                }
                playing_ = false;
            }
            stopNotify_ = 0;
            ::LeaveCriticalSection(&criticalSection_);
        }

        void RingtonePlayer::Pause() {
            if (mediaControl_) {
                mediaControl_->Pause();
            }
        }

        void RingtonePlayer::Resume() {
            if (mediaControl_) {
                mediaControl_->Run();
            }
        }

        //-8000 ~ 0
        void RingtonePlayer::SetVolume(int const value) {
            int v = value;
            if (v <= -8000) {
                v = -8000;
            } else if (v > 0) {
                v = 0;
            }
            volume_ = v;
            if (basicAudio_) {
                basicAudio_->put_Volume(volume_);
            }
        }

        void RingtonePlayer::DecreaseVolume(int const offset) {
            if (basicAudio_) {
                volume_ -= offset;
                if (volume_ < MIN_SOUND) {
                    volume_ = MIN_SOUND;
                }
                basicAudio_->put_Volume(volume_); 
            }
        }

        void RingtonePlayer::IncreaseVolume(int const offset) {
            if (basicAudio_) {
                volume_ += offset;
                if (volume_ > MAX_SOUND) {
                    volume_ = MAX_SOUND;
                }
                basicAudio_->put_Volume(volume_);
            }
        }

        void RingtonePlayer::SetMinVolume() {
            if (basicAudio_) {
                volume_ = MIN_SOUND;
                basicAudio_->put_Volume(volume_); 
            }
        }

        void RingtonePlayer::SetMaxVolume() {
            if (basicAudio_) {
                volume_ = MAX_SOUND;
                basicAudio_->put_Volume(volume_); 
            }
        }

        void RingtonePlayer::SetDefaultVolume() {
            if (basicAudio_) {
                volume_ = DEFUALT_SOUND_VALUME;
                basicAudio_->put_Volume(volume_);
            }
        }
    }
}
