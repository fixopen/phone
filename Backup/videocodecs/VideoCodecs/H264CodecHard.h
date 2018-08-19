#ifndef	_H264CodecHard_
#define	_H264CodecHard_

#include "H264_MP4_Decoder_encoder/MfcDrvParams.h"
//#include "H264_MP4_Decoder_encoder/SsbSipLogMsg.h"
#include "H264_MP4_Decoder_encoder/SsbSipH264Encode.h"
#include "H264_MP4_Decoder_encoder/SsbSipH264Decode.h"

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

class H264CoderHard : VideoCodec
{
public:
	H264CoderHard(void);
	virtual ~H264CoderHard(void);
	virtual bool EncoderSetParam(Para *pPara);
	virtual bool EncoderHardInit();
	virtual bool EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize);
	virtual void EncoderHardDeInit();

	virtual bool DecoderHardInit();
	virtual bool DecodeHard(unsigned char *pData,int DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens);
	virtual bool DecoderGetParam(Para *pPara);
	virtual void DecoderHardDeInit();
private:
	Para H264EncoderPara;
	Para H264DecoderPara;
	bool EncoderHasSeted;
	bool DecoderHasProbed;
	char *pdecoder;
	char *pencoder;
	unsigned char *pEncoderBuff_in;
	unsigned char *pEncoderBuff_out;
	unsigned char *pDecoderBuff_in;
	unsigned char *pDecoderBuff_out;
	unsigned int CurrentFrame;
};
#endif