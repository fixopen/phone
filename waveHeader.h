#ifndef WAVEHEADER_H
#define WAVEHEADER_H
typedef enum __WAVEDEV_MSG__
{
	WAV_PLAYBACK_FLAG       = 0x10000000,
		WAV_SPEAKER_ONLY		= 0x10000001,
		WAV_LINEOUT_ONLY		= 0x10000002,
		WAV_HANDLE_ONLY			= 0x10000004,
		
		WAV_SNDEFFECT_POP		= 0x20000000,
		WAV_SNDEFFECT_3D		= 0x20000001,
		WAV_SNDEFFECT_CLASSIC	= 0x20000002,
		WAV_SNDEFFECT_JAZZ		= 0x20000003,
		
		WAV_RECORD_FLAG			= 0x30000000,
		WAV_LINEIN_MIC			= 0x30000001,
		WAV_LOCAL_MIC			= 0x30000002,
		WAV_LOCAL_HANDLE		= 0x30000004,
		
		WAV_SND_SOFTMUTE_ON		= 0x40000003,
		WAV_SND_SOFTMUTE_OFF	= 0x40000004,
		
		WAV_MIC_GAIN   			= 0x40000001
};

typedef int (__stdcall *SabtdInit)(void **ppSabtdStatus, int sampleRate, char *license);
typedef int (__stdcall *SabtdUnInit)(void *pSabtdStatus);
typedef int (__stdcall *SabtdSetParam)(void *pSabtdStatus, ParamType paramType, float paramValue);
typedef int (__stdcall *SabtdGetParam)(void *pSabtdStatus, ParamType paramType, float *pParamValue);
typedef int (__stdcall *SabtdReset)(void *pSabtdStatus);
typedef int (__stdcall *SabtdProcess)(void *pSabtdStatus, int *result, short *data, int length);

#endif