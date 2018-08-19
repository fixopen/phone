//#include "H263Decoder/convert.h"
#include <windef.h>
#include "H264CodecHard.h"

H264CoderHard::H264CoderHard()
{
	EncoderHasSeted = false;
	DecoderHasProbed = false;
	pdecoder=NULL;
	pencoder=NULL;
	CurrentFrame = 0;

}
H264CoderHard::~H264CoderHard()
{

}
bool H264CoderHard::EncoderSetParam(Para *pPara)
{
	H264EncoderPara.uiWidth = pPara->uiWidth;
	H264EncoderPara.uiHeight = pPara->uiHeight;
	if((H264EncoderPara.uiWidth == 176) && (H264EncoderPara.uiHeight == 144))
	{
		H264EncoderPara.uiBitrate_kbps = pPara->uiBitrate_kbps;
		H264EncoderPara.uiFramerate	=	pPara->uiFramerate;
		H264EncoderPara.uiGOPNum = pPara->uiGOPNum;
		H264EncoderPara.uiQuantInter = pPara->uiQuantInter;
		H264EncoderPara.uiQuantIntra = pPara->uiQuantIntra;
		EncoderHasSeted = true;
	}
	return EncoderHasSeted;
}
bool H264CoderHard::EncoderHardInit()
{
	bool ret = false;
	// Initialize the compressor
	if(EncoderHasSeted)
	{
		// Initialize table for RGB to YUV conversion
//		InitLookupTable();
		// Initialize the compressor
		pencoder =(char *)SsbSipH264EncodeInit(H264EncoderPara.uiWidth,H264EncoderPara.uiHeight,H264EncoderPara.uiFramerate,H264EncoderPara.uiBitrate_kbps,H264EncoderPara.uiGOPNum);
		if (pencoder) 
		{
				
			if(!SsbSipH264EncodeExe(pencoder))
			{
				pEncoderBuff_in =(unsigned char *) SsbSipH264EncodeGetInBuf(pencoder,80000);
				if(pEncoderBuff_in)
				{
					CurrentFrame = 0;
					ret = true;
					return ret;
				}

			}
			SsbSipH264EncodeDeInit(pencoder);
			pencoder = NULL;		
		}	
	}
	return ret;
}
bool H264CoderHard::EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize)
{
	bool ret = false;
	//Compress the data...to h263
	memcpy(pEncoderBuff_in,pYuv420Data,H264EncoderPara.uiWidth*H264EncoderPara.uiHeight+H264EncoderPara.uiWidth*H264EncoderPara.uiHeight/2);
	int retvalue;
	//bool	I_frame = true;
	//if(H264EncoderPara.uiGOPNum == 0)
	//{
	//	if(CurrentFrame)
	//		I_frame = false;
	//}
	//else
	//{
	//	if(CurrentFrame%H264EncoderPara.uiGOPNum != 0)
	//			I_frame = false;
	//}

	//if(I_frame)//I-FRAME
	//{
	//	unsigned int value[2];
	//	value[0] = MPEG4_ENC_PIC_OPT_IDR;
	//	value[1] = 1;
	//	ret = SsbSipMPEG4EncodeSetConfig(pencoder,H264_ENC_SETCONF_CUR_PIC_OPT, value);
	//	if(ret != SSBSIP_H264_DEC_RET_OK)
	//	{
	//		H264CoderDeInit();
	//		return 7; 
	//	}
	//}
	//else//P-FRAME
	//{
	//		unsigned int value[2];
	//		value[0] = ENC_PIC_OPT_RECOVERY;
	//		value[1] = 1;
	//		ret = SsbSipMPEG4EncodeSetConfig(pencoder,H264_ENC_SETCONF_CUR_PIC_OPT, value);
	//		if(ret != SSBSIP_H264_DEC_RET_OK)
	//	{
	//		H264CoderDeInit();
	//		return 9; 

	//	}
	//}

	retvalue = SsbSipH264EncodeExe(pencoder);
	if(retvalue == SSBSIP_H264_ENC_RET_OK)
	{
		long size;
		pEncoderBuff_out =(unsigned char *) SsbSipH264EncodeGetOutBuf(pencoder,&size);
		if(pEncoderBuff_out)
		{
			if(size <= OutBuffSize)
			{
				memcpy(pOutBuffer,pEncoderBuff_out,size);
				*pEncodedSize = size;
				ret = true;
			}					
		}				
	}

	return ret;
}
void H264CoderHard::EncoderHardDeInit()
{
	if(EncoderHasSeted)
	{
		if(pencoder)
		{
			SsbSipH264EncodeDeInit(pencoder);
			pencoder = NULL;
		}
	CurrentFrame = 0;
	EncoderHasSeted = false;
	}
}

bool H264CoderHard::DecoderHardInit()
{
	bool	ret = false;
	//Initialize conversion table for YUV420 to RGB
//	InitConvertTable();
	// Initialize decompressor
	pdecoder = (char *)SsbSipH264DecodeInit();
	if(pdecoder)
	{
		pDecoderBuff_in =(unsigned char *) SsbSipH264DecodeGetInBuf(pdecoder, 80000);
		if(pDecoderBuff_in)
		{
			ret = true;
			return ret;  
		}
		SsbSipH264DecodeDeInit(pdecoder);
		pdecoder = NULL;	
	}
	return ret;
}
bool H264CoderHard::DecodeHard(unsigned char *pH264Data,int H264DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens)
{
	int retvalue;
	bool ret = false;

	memcpy(pDecoderBuff_in,pH264Data,H264DataSize);
	if(!DecoderHasProbed)
	{

		retvalue = SsbSipH264DecodeExe(pdecoder,H264DataSize);
		if(!retvalue)
		{
			SSBSIP_H264_STREAM_INFO strinfo_;
			retvalue = SsbSipH264DecodeGetConfig(pdecoder,H264_DEC_GETCONF_STREAMINFO,&strinfo_);
			if(!retvalue)
			{
				H264DecoderPara.uiWidth = strinfo_.width;
				H264DecoderPara.uiHeight = strinfo_.height;
				ret = true;
				DecoderHasProbed = true;
			}
			else
				return ret;
  		}
		else
			return ret;
	}

	retvalue = SsbSipH264DecodeExe(pdecoder,H264DataSize);		
	if(!retvalue)
	{
		long size;
		pDecoderBuff_out =(unsigned char *) SsbSipH264DecodeGetOutBuf(pdecoder,&size);
		if(pDecoderBuff_out)
		{
			if(size <= *pDecodedLens)
			{
				memcpy(pOutYuv420Data,pDecoderBuff_out,size);				
				*pDecodedLens = size;
				ret = true;
			}
		}
		else
			return false;
	}
	else
		return false;
	return ret;
}
bool H264CoderHard::DecoderGetParam(Para *pPara)
{
	if(DecoderHasProbed)
	{
		pPara->uiWidth = H264DecoderPara.uiWidth;
		pPara->uiHeight = H264DecoderPara.uiHeight;
		pPara->uiBitrate_kbps = H264DecoderPara.uiBitrate_kbps;
		pPara->uiFramerate = H264DecoderPara.uiFramerate;
		pPara->uiGOPNum = H264DecoderPara.uiGOPNum;
		pPara->uiQuantInter = H264DecoderPara.uiQuantInter;
		pPara->uiQuantIntra = H264DecoderPara.uiQuantIntra;
	}
	return DecoderHasProbed;
}
void H264CoderHard::DecoderHardDeInit()
{
	if(DecoderHasProbed)
	{
		// close decoder ......
		if(pdecoder)
		{
			SsbSipH264DecodeDeInit(pdecoder);
			pdecoder = NULL;
		}
	}
	DecoderHasProbed = false;
}