#include <windef.h>
#include <string.h>
//#include "H263Decoder/convert.h"
#include "MP4CodecHard.h"

MP4CoderHard::MP4CoderHard()
{
	EncoderHasSeted = false;
	DecoderHasProbed = false;
	pdecoder=NULL;
	pencoder=NULL;
	CurrentFrame = 0; 
	IsIFrame = true;

}
MP4CoderHard::~MP4CoderHard()
{

}
bool MP4CoderHard::EncoderSetParam(Para *pPara)
{
	MP4EncoderPara.uiWidth = pPara->uiWidth;
	MP4EncoderPara.uiHeight = pPara->uiHeight;
	//if((MP4EncoderPara.uiWidth == 176) && (MP4EncoderPara.uiHeight == 144))
	//{
		MP4EncoderPara.uiBitrate_kbps = pPara->uiBitrate_kbps;
		MP4EncoderPara.uiFramerate	=	pPara->uiFramerate;
		MP4EncoderPara.uiGOPNum = pPara->uiGOPNum;
		MP4EncoderPara.uiQuantInter = pPara->uiQuantInter;
		MP4EncoderPara.uiQuantIntra = pPara->uiQuantIntra;
		EncoderHasSeted = true;
	/*}*/
	return EncoderHasSeted;
}
bool MP4CoderHard::EncoderHardInit()
{
	bool ret = false;
	// Initialize the compressor
	if(EncoderHasSeted)
	{
		// Initialize table for RGB to YUV conversion
		//InitLookupTable();
		// Initialize the compressor
		pencoder =(char *)SsbSipMPEG4EncodeInit(SSBSIPMFCENC_MPEG4,MP4EncoderPara.uiWidth,MP4EncoderPara.uiHeight,MP4EncoderPara.uiFramerate,MP4EncoderPara.uiBitrate_kbps,MP4EncoderPara.uiGOPNum);
		if (pencoder) 
		{
				
			if(!SsbSipMPEG4EncodeExe(pencoder))
			{
				pEncoderBuff_in =(unsigned char *) SsbSipMPEG4EncodeGetInBuf(pencoder,80000);
				if(pEncoderBuff_in)
				{
					CurrentFrame = 0;
					IsIFrame = true;
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
bool MP4CoderHard::EncodeHard(unsigned char *pYuv420Data,unsigned char *pOutBuffer ,int OutBuffSize,int *pEncodedSize)
{
	bool ret = false;
	//Compress the data...to h263
	memcpy(pEncoderBuff_in,pYuv420Data,MP4EncoderPara.uiWidth*MP4EncoderPara.uiHeight+MP4EncoderPara.uiWidth*MP4EncoderPara.uiHeight/2);
	int retvalue;

	retvalue = SsbSipMPEG4EncodeExe(pencoder);
	if(retvalue == SSBSIP_MPEG4_ENC_RET_OK)
	{		
		if(!MP4EncoderPara.uiGOPNum&&(CurrentFrame !=0))
		{
			IsIFrame = false;
		}
		else if((MP4EncoderPara.uiGOPNum == 1))
		{
			IsIFrame = true;
		}
		else
		{
			if(CurrentFrame % MP4EncoderPara.uiGOPNum == 0)
			{
				IsIFrame = true;
			}
			else
			{
				IsIFrame = false;			
			}
		}
		CurrentFrame++;		
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
bool MP4CoderHard::CurrentIFrame()
{
	return IsIFrame;
}
void MP4CoderHard::EncoderHardDeInit()
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

bool MP4CoderHard::DecoderHardInit()
{
	bool	ret = false;
	//Initialize conversion table for YUV420 to RGB
	//InitConvertTable();
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
bool MP4CoderHard::DecodeHard(unsigned char *pMP4Data,int MP4DataSize,unsigned char *pOutYuv420Data,int *pDecodedLens)
{
	int retvalue;
	bool ret = false;

	memcpy(pDecoderBuff_in,pMP4Data,MP4DataSize);
	if(!DecoderHasProbed)
	{

		retvalue = SsbSipMPEG4DecodeExe(pdecoder,MP4DataSize);
		if(!retvalue)
		{
			SSBSIP_MPEG4_STREAM_INFO strinfo_;
			retvalue = SsbSipMPEG4DecodeGetConfig(pdecoder,MPEG4_DEC_GETCONF_STREAMINFO,&strinfo_);
			if(!retvalue)
			{
				MP4DecoderPara.uiWidth = strinfo_.width;
				MP4DecoderPara.uiHeight = strinfo_.height;
				ret = true;
				DecoderHasProbed = true;
			}
			else
				return ret;
  		}
		else
			return ret;
	}

	retvalue = SsbSipMPEG4DecodeExe(pdecoder,MP4DataSize);		
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
bool MP4CoderHard::DecoderGetParam(Para *pPara)
{
	if(DecoderHasProbed)
	{
		pPara->uiWidth = MP4DecoderPara.uiWidth;
		pPara->uiHeight = MP4DecoderPara.uiHeight;
		pPara->uiBitrate_kbps = MP4DecoderPara.uiBitrate_kbps;
		pPara->uiFramerate = MP4DecoderPara.uiFramerate;
		pPara->uiGOPNum = MP4DecoderPara.uiGOPNum;
		pPara->uiQuantInter = MP4DecoderPara.uiQuantInter;
		pPara->uiQuantIntra = MP4DecoderPara.uiQuantIntra;
	}
	return DecoderHasProbed;
}
void MP4CoderHard::DecoderHardDeInit()
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