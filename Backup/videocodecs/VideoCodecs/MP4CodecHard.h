#ifndef	_MP4CodecHard_
#define	_MP4CodecHard_

#include "H264_MP4_Decoder_encoder/MfcDrvParams.h"
#include "H264_MP4_Decoder_encoder/SsbSipMpeg4Encode.h"
#include "H264_MP4_Decoder_encoder/SsbSipMpeg4Decode.h"

#include "CodecPara.h"
#include "VideoCodec.h"
//struct	Para
//{
//	unsigned int uiWidth;
//	unsigned int uiHeight;
//	unsigned int uiFramerate;
//	unsigned int uiBitrate_kbps;
//	unsigned int uiQuantIntra;
//	unsigned int uiQuantInter;
//    unsigned int uiGOPNum;
//};

class MP4CoderHard : VideoCodec
{
public:
	MP4CoderHard(void);
	virtual ~MP4CoderHard(void);
	virtual bool EncoderSetParam(Para *pPara);
	virtual bool EncoderHardInit();
	virtual bool EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize);
	virtual void EncoderHardDeInit();

	virtual bool DecoderHardInit();
	virtual bool DecodeHard(unsigned char *pMP4Data,int MP4DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens);
	virtual bool DecoderGetParam(Para *pPara);
	virtual void DecoderHardDeInit();
	virtual bool CurrentIFrame(void);
private:
	Para MP4EncoderPara;
	Para MP4DecoderPara;
	bool EncoderHasSeted;
	bool DecoderHasProbed;
	char *pdecoder;
	char *pencoder;
	unsigned char *pEncoderBuff_in;
	unsigned char *pEncoderBuff_out;
	unsigned char *pDecoderBuff_in;
	unsigned char *pDecoderBuff_out;
	int CurrentFrame;
	bool IsIFrame;
};
#endif