#ifndef  _VideoCodec_
#define	 _VideoCodec_

#include "CodecPara.h"

class VideoCodec
{
public:
	VideoCodec(void){};
	virtual ~VideoCodec(void){};
	virtual bool EncoderSetParam(Para *pPara) = 0;
	virtual bool EncoderHardInit() = 0;
	virtual bool EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize) = 0;
	virtual void EncoderHardDeInit() = 0;

	virtual bool DecoderHardInit() = 0;
	virtual bool DecodeHard(unsigned char *pData,int DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens) = 0;
	virtual bool DecoderGetParam(Para *pPara) = 0;
	virtual void DecoderHardDeInit() = 0;
};
#endif