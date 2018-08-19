//#include "H263Decoder/convert.h"
#include <WinDef.h>
#include "H263CodecHard.h"

H263CoderHard::H263CoderHard()
{
	EncoderHasSeted = false;
	DecoderHasProbed = false;
	pdecoder=NULL;
	pencoder=NULL;
	CurrentFrame = 0;

}
H263CoderHard::~H263CoderHard()
{

}
bool H263CoderHard::EncoderSetParam(Para *pPara)
{
	H263EncoderPara.uiWidth = pPara->uiWidth;
	H263EncoderPara.uiHeight = pPara->uiHeight;
	if((H263EncoderPara.uiWidth == 176) && (H263EncoderPara.uiHeight == 144))
	{
		H263EncoderPara.uiBitrate_kbps = pPara->uiBitrate_kbps;
		H263EncoderPara.uiFramerate	=	pPara->uiFramerate;
		H263EncoderPara.uiGOPNum = pPara->uiGOPNum;
		H263EncoderPara.uiQuantInter = pPara->uiQuantInter;
		H263EncoderPara.uiQuantIntra = pPara->uiQuantIntra;
		EncoderHasSeted = true;
	}
	return EncoderHasSeted;
}
bool H263CoderHard::EncoderHardInit()
{
	bool ret = false;
	// Initialize the compressor
	if(EncoderHasSeted)
	{
		// Initialize table for RGB to YUV conversion
//		InitLookupTable();
		// Initialize the compressor
		pencoder =(char *)SsbSipMPEG4EncodeInit(SSBSIPMFCENC_H263,H263EncoderPara.uiWidth,H263EncoderPara.uiHeight,H263EncoderPara.uiFramerate,H263EncoderPara.uiBitrate_kbps,H263EncoderPara.uiGOPNum);
		if (pencoder) 
		{
				
			if(!SsbSipMPEG4EncodeExe(pencoder))
			{
				pEncoderBuff_in =(unsigned char *) SsbSipMPEG4EncodeGetInBuf(pencoder,80000);
				if(pEncoderBuff_in)
				{
					CurrentFrame = 0;
					ret = true;
					return ret;
				}

			}
			SsbSipMPEG4EncodeDeInit(pencoder);
			pencoder = NULL;		
		}	
	}
	return ret;
}
bool H263CoderHard::EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize)
{
	bool ret = false;
	//Compress the data...to h263
	memcpy(pEncoderBuff_in,pYuv420Data,H263EncoderPara.uiWidth*H263EncoderPara.uiHeight+H263EncoderPara.uiWidth*H263EncoderPara.uiHeight/2);
	int retvalue;
	//bool	I_frame = true;
	//if(H263EncoderPara.uiGOPNum == 0)
	//{
	//	if(CurrentFrame)
	//		I_frame = false;
	//}
	//else
	//{
	//	if(CurrentFrame%H263EncoderPara.uiGOPNum != 0)
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

	retvalue = SsbSipMPEG4EncodeExe(pencoder);
	if(retvalue == SSBSIP_MPEG4_ENC_RET_OK)
	{
		long size;
		pEncoderBuff_out =(unsigned char *) SsbSipMPEG4EncodeGetOutBuf(pencoder,&size);
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
void H263CoderHard::EncoderHardDeInit()
{
	if(EncoderHasSeted)
	{
		if(pencoder)
		{
			SsbSipMPEG4EncodeDeInit(pencoder);
			pencoder = NULL;
		}
	CurrentFrame = 0;
	EncoderHasSeted = false;
	}
}

bool H263CoderHard::DecoderHardInit()
{
	bool	ret = false;
	//Initialize conversion table for YUV420 to RGB
//	InitConvertTable();
	// Initialize decompressor
	pdecoder = (char *)SsbSipMPEG4DecodeInit();
	if(pdecoder)
	{
		pDecoderBuff_in =(unsigned char *) SsbSipMPEG4DecodeGetInBuf(pdecoder, 80000);
		if(pDecoderBuff_in)
		{
			ret = true;
			return ret;  
		}
		SsbSipMPEG4DecodeDeInit(pdecoder);
		pdecoder = NULL;	
	}
	return ret;
}
bool H263CoderHard::DecodeHard(unsigned char *pH263Data,int H263DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens)
{
	int retvalue;
	bool ret = false;

	memcpy(pDecoderBuff_in,pH263Data,H263DataSize);
	if(!DecoderHasProbed)
	{

		retvalue = SsbSipMPEG4DecodeExe(pdecoder,H263DataSize);
		if(!retvalue)
		{
			SSBSIP_MPEG4_STREAM_INFO strinfo_;
			retvalue = SsbSipMPEG4DecodeGetConfig(pdecoder,MPEG4_DEC_GETCONF_STREAMINFO,&strinfo_);
			if(!retvalue)
			{
				H263DecoderPara.uiWidth = strinfo_.width;
				H263DecoderPara.uiHeight = strinfo_.height;
				ret = true;
				DecoderHasProbed = true;
			}
			else
				return ret;
  		}
		else
			return ret;
	}

	retvalue = SsbSipMPEG4DecodeExe(pdecoder,H263DataSize);		
	if(!retvalue)
	{
		long size;
		pDecoderBuff_out =(unsigned char *) SsbSipMPEG4DecodeGetOutBuf(pdecoder,&size);
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
bool H263CoderHard::DecoderGetParam(Para *pPara)
{
	if(DecoderHasProbed)
	{
		pPara->uiWidth = H263DecoderPara.uiWidth;
		pPara->uiHeight = H263DecoderPara.uiHeight;
		pPara->uiBitrate_kbps = H263DecoderPara.uiBitrate_kbps;
		pPara->uiFramerate = H263DecoderPara.uiFramerate;
		pPara->uiGOPNum = H263DecoderPara.uiGOPNum;
		pPara->uiQuantInter = H263DecoderPara.uiQuantInter;
		pPara->uiQuantIntra = H263DecoderPara.uiQuantIntra;
	}
	return DecoderHasProbed;
}
void H263CoderHard::DecoderHardDeInit()
{
	if(DecoderHasProbed)
	{
		// close decoder ......
		if(pdecoder)
		{
			SsbSipMPEG4DecodeDeInit(pdecoder);
			pdecoder = NULL;
		}
	}
	DecoderHasProbed = false;
}