#include "StdAfx.h"
#include "Convert.h"
#include "pjmedia\\port.h"

#include "VideoCodecs/H263CodecHard.h"
#include "VideoCodecs/H264CodecHard.h"
#include "VideoCodecs/MP4CodecHard.h"

#include "VideoSubSysFor6410.h"

#define CAM_WIDTH 176
#define CAM_HEIGHT 144

static unsigned char LocalRgb24Buff[CAM_WIDTH*CAM_HEIGHT*3];
static unsigned char RemoteRgb24Buff[CAM_WIDTH*CAM_HEIGHT*3];

extern	VideoSubSysFor6410	*pMyVideoSubSys;

static	unsigned char YUV420_Received[352*288+352*288/2];
static	bool	HasCaptured = false;

static	unsigned char EncodedFrameBuff[352*288+352*288/2];
static	unsigned char BeforeEncodedFrameBuff[352*288+352*288/2];
static  const unsigned int EncodeFrameBuffLen = 352*288+352*288/2;
static	long EncodedFrameSize  = 0;
static	pjmedia_frame EncodedFrame;

static	unsigned char ReceivedFrameBuff[352*288+352*288/2];
static	long	ReceivedFrameLen = 0;

static  Para	DecodePara;



 void MyCamCallBack(BYTE* pdata, long len)
{
	memcpy(BeforeEncodedFrameBuff,pdata,len);
	HasCaptured = true;
		//if (pMyVideoSubSys->pMyLocalDisplay)
		//{
		//	::ConvertYUV2RGB(EncodedFrameBuff,\
		//					EncodedFrameBuff + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight \
		//									 +pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight /2,\
		//					EncodedFrameBuff + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight,\
		//					LocalRgb24Buff,\
		//					pMyVideoSubSys->MyCamPara_.uiWidth,\
		//					pMyVideoSubSys->MyCamPara_.uiHeight
		//					);
		//	pMyVideoSubSys->pMyLocalDisplay->ShowPic(pMyVideoSubSys->MyCamPara_.uiWidth,pMyVideoSubSys->MyCamPara_.uiHeight,LocalRgb24Buff,false);
		//	//pMyVideoSubSys->pMyRemoteDisplay->ShowPic(pMyVideoSubSys->MyCamPara_.uiWidth,pMyVideoSubSys->MyCamPara_.uiHeight,LocalRgb24Buff,false);
		//}
	if (pMyVideoSubSys)
	{
		if (pMyVideoSubSys->pMedia_port_info && pMyVideoSubSys->put_frame_video_data)
		{
			pMyVideoSubSys->pMyVideoCodec->EncodeHard(pdata,EncodedFrameBuff,EncodeFrameBuffLen,(int *)&EncodedFrameSize);

			pjmedia_port *port = (pjmedia_port*)pMyVideoSubSys->pMedia_port_info;
			port->info.encoding_name = pj_str("H263");
			port->info.pt = 34;
			port->info.type = PJMEDIA_TYPE_VIDEO;
			port->info.video_height = pMyVideoSubSys->MyCamPara_.uiHeight;
			port->info.video_width = pMyVideoSubSys->MyCamPara_.uiWidth;
			port->info.name = pj_str("video");		

			EncodedFrame.type = PJMEDIA_FRAME_TYPE_VIDEO;
			EncodedFrame.buf = EncodedFrameBuff;
			EncodedFrame.size = EncodedFrameSize;
			//memcpy(&(EncodedFrame.media_param),&mediaParam,sizeof(MEDIAPARAM));
			//1103 qi 本地显示
			//{
			//    static int	DecodedFrameLen = 80000;
			//	static  Para	DecPara;
			//	static unsigned char YUVReceived[352*288+352*288/2];
			//	static unsigned char RemoteRgbBuff[CAM_WIDTH*CAM_HEIGHT*3];
			//	static int Width_Dec;
			//	static int Height_Dec;
			//	pMyVideoSubSys->pMyVideoCodec->DecodeHard(EncodedFrameBuff,EncodedFrameSize,YUVReceived,&DecodedFrameLen);
			//	pMyVideoSubSys->pMyVideoCodec->DecoderGetParam(&DecPara);
			//	Width_Dec = DecPara.uiWidth;
			//	Height_Dec = DecPara.uiHeight;

			//	::ConvertYUV2RGB(YUVReceived,\
			//		YUVReceived + Width_Dec * Height_Dec,\
			//		YUVReceived + Width_Dec * Height_Dec \
			//		+ Width_Dec * Height_Dec / 2,\
			//		RemoteRgbBuff,\
			//		Width_Dec,\
			//		Height_Dec
			//		);
			//	pMyVideoSubSys->pMyRemoteDisplay->ShowPic(Width_Dec,Height_Dec,RemoteRgbBuff,false);
			//}
			pMyVideoSubSys->put_frame_video_data(port,&EncodedFrame);
		}
		if (pMyVideoSubSys->pMyLocalDisplay)
		{
			//::ConvertYUV2RGB(pdata,\
			//				pdata + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight \
			//								 +pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight /2,\
			//				pdata + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight,\
			//				LocalRgb24Buff,\
			//				pMyVideoSubSys->MyCamPara_.uiWidth,\
			//				pMyVideoSubSys->MyCamPara_.uiHeight
			//				);

			::ConvertYUV2RGB(BeforeEncodedFrameBuff,\
							BeforeEncodedFrameBuff + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight \
											 +pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight /2,\
							BeforeEncodedFrameBuff + pMyVideoSubSys->MyCamPara_.uiWidth * pMyVideoSubSys->MyCamPara_.uiHeight,\
							LocalRgb24Buff,\
							pMyVideoSubSys->MyCamPara_.uiWidth,\
							pMyVideoSubSys->MyCamPara_.uiHeight
							);
			pMyVideoSubSys->pMyLocalDisplay->ShowPic(pMyVideoSubSys->MyCamPara_.uiWidth,pMyVideoSubSys->MyCamPara_.uiHeight,LocalRgb24Buff,false);
			//pMyVideoSubSys->pMyRemoteDisplay->ShowPic(pMyVideoSubSys->MyCamPara_.uiWidth,pMyVideoSubSys->MyCamPara_.uiHeight,LocalRgb24Buff,false);
		}
		
	}

}

VideoSubSysFor6410::VideoSubSysFor6410(void)
{
    InitLookupTable();
	InitConvertTable();

	pMyLocalDisplay		= new Display;
	//if(pMyLocalDisplay)
	//	pMyLocalDisplay->DisplayInit();
	pMyRemoteDisplay	= new Display;
	//if(pMyRemoteDisplay)
	//	pMyRemoteDisplay->DisplayInit();
	pMyVideoCodec		= NULL;
	pMyCamera			= new LocalCamera6410;
	::pMyVideoSubSys		= this;
}

VideoSubSysFor6410::~VideoSubSysFor6410(void)
{
	if(pMyLocalDisplay)
	{
		pMyLocalDisplay->DisplayDeInit();
		delete pMyLocalDisplay;
	}
	if (pMyRemoteDisplay)
	{
		pMyRemoteDisplay->DisplayDeInit();
		delete pMyRemoteDisplay;
	}
	if (pMyCamera)
	{
		pMyCamera->CloseCamera();
		delete pMyCamera;
	}
	::pMyVideoSubSys		= NULL;
}
pj_status_t VideoSubSysFor6410::InitCodec(int nPt,int nWidth,int nHeight)
{	
	PayLoadType_Sdp = nPt;
	Width_Sdp = nWidth;
	Height_Sdp = nHeight;
	switch(PayLoadType_Sdp)
	{
		case 34:
			//pMyVideoCodec =(VideoCodec *) new H263CoderHard;
			pMyVideoCodec =(VideoCodec *) new MP4CoderHard;
			//pMyVideoCodec =(VideoCodec *) new H264CoderHard;
			Para MyH263Para;
			MyH263Para.uiWidth = MyCamPara_.uiWidth;
			MyH263Para.uiHeight = MyCamPara_.uiHeight;
			MyH263Para.uiBitrate_kbps = 90000;
			MyH263Para.uiFramerate = 20;
			MyH263Para.uiQuantInter = 8;
			MyH263Para.uiQuantIntra = 8;
			MyH263Para.uiGOPNum = 20;
			pMyVideoCodec->EncoderSetParam(&MyH263Para);
			pMyVideoCodec->EncoderHardInit();
			pMyVideoCodec->DecoderHardInit();
			break;
		default:
			break;
	}

	return PJ_SUCCESS;
}
pj_status_t VideoSubSysFor6410::UninitCodec()
{
	if (pMyVideoCodec)
	{
		pMyVideoCodec->EncoderHardDeInit();
		pMyVideoCodec->DecoderHardDeInit();
		delete pMyVideoCodec;
	}
	return PJ_SUCCESS;
}

pj_status_t VideoSubSysFor6410::pjmedia_video_open_camera()
{
	MyCamPara_.uiWidth = CAM_WIDTH;
	MyCamPara_.uiHeight = CAM_HEIGHT;
	MyCamPara_.uiFrameRate = 20;
	MyCamPara_.SampleCallBack =(MANAGEDCALLBACKPROC) MyCamCallBack;
	pMyCamera->SetPara(&MyCamPara_);
	pMyCamera->OpenCamera();
	return PJ_SUCCESS;
}
pj_status_t VideoSubSysFor6410::pjmedia_video_close_camera()
{
	pMyCamera->CloseCamera();
	pMyCamera = NULL;
	return PJ_SUCCESS;
}
pj_status_t VideoSubSysFor6410::pjmedia_video_dev_count()
{
	return PJ_SUCCESS;
}
pj_status_t VideoSubSysFor6410::pjmedia_video_query_frame_start()
{
	pMyCamera->StartCapture();
	return PJ_SUCCESS;
}
pj_status_t VideoSubSysFor6410::pjmedia_video_query_frame_stop()
{
	pMyCamera->StopCapture();
	return PJ_SUCCESS;
}
void VideoSubSysFor6410::SetLocaLVideoHandle(HWND hShowLocalVideo)
{
	m_hShowLocalVideo = hShowLocalVideo;
	pMyLocalDisplay->DisplayInit();
	pMyLocalDisplay->SetPara(hShowLocalVideo);

}

void VideoSubSysFor6410::SetRemoteVideoHandle(HWND hShowRemoteVideo)
{
	//m_hShowRemoteVideo = hShowRemoteVideo;
	pMyRemoteDisplay->DisplayInit();
	pMyRemoteDisplay->SetPara(hShowRemoteVideo);
	m_hShowRemoteVideo = hShowRemoteVideo;

}

pj_status_t VideoSubSysFor6410::pjmedia_video_query_frame_attach(put_frame_video_callback pfv,void* param)
{
	pMedia_port_info = param;
	put_frame_video_data = pfv;
	return PJ_SUCCESS;
}


pj_status_t VideoSubSysFor6410::pjmedia_video_rtp_data_to_jitterbuffer(pj_uint8_t *pBuffer,pj_ssize_t payload_len,rtpParam * rtp_param)
{
	static bool	bIsFirstFrame = true;

	switch (rtp_param->payload)
	{
	case 34:
		/* 获取MEDIAPARAM结构信息 */
		//nMediaParam = sizeof(LY_DWORD);			

		//	memcpy(&(mediaParam.byResolution), pVideoBuf, sizeof(char));
		//	memcpy(&(mediaParam.byKeyFrame), pVideoBuf+sizeof(char), sizeof(char));

		if(rtp_param->marker != 1)
		{
			memcpy(ReceivedFrameBuff + ReceivedFrameLen,pBuffer,rtp_param->len);
			ReceivedFrameLen += rtp_param->len;
		}
		else
		{
			if(bIsFirstFrame)
			{
				memcpy(ReceivedFrameBuff + ReceivedFrameLen,pBuffer,rtp_param->len);
				ReceivedFrameLen += rtp_param->len;
				bIsFirstFrame = false;
			}
			else
			{
				memcpy(ReceivedFrameBuff + ReceivedFrameLen,pBuffer,rtp_param->len);
				ReceivedFrameLen = rtp_param->len;
				bIsFirstFrame = false;
			}
		}
		if(!bIsFirstFrame)
		{
			if (pMyVideoSubSys->pMyRemoteDisplay && pMyVideoSubSys->m_hShowRemoteVideo)
			{
				int	DecodedFrameLen = 80000;
				pMyVideoCodec->DecodeHard(ReceivedFrameBuff,ReceivedFrameLen,YUV420_Received,&DecodedFrameLen);
				pMyVideoCodec->DecoderGetParam(&DecodePara);
				Width_Dec = DecodePara.uiWidth;
				Height_Dec = DecodePara.uiHeight;

				::ConvertYUV2RGB(YUV420_Received,\
					YUV420_Received + Width_Dec * Height_Dec,\
					YUV420_Received + Width_Dec * Height_Dec \
					+ Width_Dec * Height_Dec / 2,\
					RemoteRgb24Buff,\
					Width_Dec,\
					Height_Dec
					);
				pMyVideoSubSys->pMyRemoteDisplay->ShowPic(Width_Dec,Height_Dec,RemoteRgb24Buff,false);
			}
			bIsFirstFrame = true;
			ReceivedFrameLen = 0;			
		}			
		break;
	default:
		break;
	}
	return PJ_SUCCESS;
}