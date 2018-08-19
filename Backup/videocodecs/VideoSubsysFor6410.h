#pragma once
#include "videodev.h"
#include "videoparam.h"

#include "Displays/Display.h"
#include "VideoCodecs/VideoCodec.h"
#include "Cameras/LocalCamera6410.h"

class VideoSubSysFor6410
{
public:
	VideoSubSysFor6410(void);
public:
	~VideoSubSysFor6410(void);
public:
	pj_status_t InitCodec(int nPt,int nWidth,int nHeight);
	pj_status_t UninitCodec();

	pj_status_t pjmedia_video_query_frame_attach(put_frame_video_callback pfv,void* param);

	void SetRemoteVideoHandle(HWND hShowRemoteVideo);
	void SetLocaLVideoHandle(HWND hShowLocalVideo);
	pj_status_t pjmedia_video_rtp_data_to_jitterbuffer(pj_uint8_t *pBuffer,pj_ssize_t payload_len,rtpParam * rtp_param);
	pj_status_t pjmedia_video_open_camera();
	pj_status_t pjmedia_video_close_camera();
	pj_status_t pjmedia_video_dev_count(void);
	pj_status_t pjmedia_video_query_frame_start();
	pj_status_t pjmedia_video_query_frame_stop();

private:
	put_frame_video_callback put_frame_video_data;

private:

	Display		*pMyLocalDisplay;
	Display		*pMyRemoteDisplay;
	VideoCodec	*pMyVideoCodec;
	LocalCamera6410	*pMyCamera;

	int			PayLoadType_Sdp;
	int			Width_Sdp;
	int			Height_Sdp;

	int			Width_Dec;
	int			Height_Dec;

	param		MyCamPara_;
	friend		void MyCamCallBack(BYTE* pdata, long len);

	void		*pMedia_port_info;
	HWND m_hShowRemoteVideo; 
	HWND m_hShowLocalVideo; 
	MEDIAPARAM mediaParam;
};
