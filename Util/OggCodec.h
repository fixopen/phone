#ifndef __OGGENCODER_H__
#define __OGGENCODER_H__

#include "../inc/speex.h"
//#include "../inc/speex_types.h"
// #include "../inc/speex_bits.h"
#include "../inc/speex_header.h"
#include "../inc/speex_stereo.h"
#include "../inc/speex_preprocess.h"
#include "../inc/speex_callbacks.h"
#include "../inc/ogg.h"
//#include "../inc/wav_io.h"
/* Copyright (C) 2002 Jean-Marc Valin 
   File: wav_io.h

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef WAV_IO_H
#define WAV_IO_H

#include <stdio.h>
#include "../inc/speex_types.h"

#if !defined(__LITTLE_ENDIAN__) && ( defined(WORDS_BIGENDIAN) || defined(__BIG_ENDIAN__) )
#define le_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#define be_short(s) ((short) (s))
#else
#define le_short(s) ((short) (s))
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#endif 

/** Convert little endian */
static inline spx_int32_t le_int(spx_int32_t i)
{
#if !defined(__LITTLE_ENDIAN__) && ( defined(WORDS_BIGENDIAN) || defined(__BIG_ENDIAN__) )
   spx_uint32_t ui, ret;
   ui = i;
   ret =  ui>>24;
   ret |= (ui>>8)&0x0000ff00;
   ret |= (ui<<8)&0x00ff0000;
   ret |= (ui<<24);
   return ret;
#else
   return i;
#endif
}

int read_wav_header(FILE *file, int *rate, int *channels, int *format, spx_int32_t *size);

void write_wav_header(FILE *file, int rate, int channels, int format, int size);

#endif

//#include "../inc/wave_out.h"
/* Copyright (c) 2002, John Edwards

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//    WAVE_OUT.H - Necessary stuff for WIN_AUDIO

#ifndef WAVE_OUT_H
#define WAVE_OUT_H

#include <stdio.h>
#include <windows.h>
#ifdef __MINGW32__
#include <mmsystem.h>
#endif

#define VERSION_STRING L"\n 0.7.0\n"

#define Cdecl               __cdecl
#define __attribute__(x)
#define sleep(__sec)        Sleep ((__sec) * 1000)
#define inline              __inline
#define restrict

//// constants /////////////////////////////////////////////////////

#define CD_SAMPLE_FREQ         44.1e3
#define SAMPLE_SIZE            16
#define SAMPLE_SIZE_STRING     ""
#define WINAUDIO_FD            ((FILE_T)-128)
#define FILE_T                 FILE*
#define INVALID_FILEDESC       NULL

//// Simple types //////////////////////////////////////////////////

typedef signed   int        Int;        // at least -32767...+32767, fast type
typedef unsigned int        Uint;       // at least 0...65535, fast type
typedef long double         Ldouble;    // most exact floating point format

//// procedures/functions //////////////////////////////////////////
// wave_out.c
Int        Set_WIN_Params             ( FILE_T dummyFile , Ldouble SampleFreq, Uint BitsPerSample, Uint Channels);
int        WIN_Play_Samples           ( const void* buff, size_t len );
int        WIN_Audio_close            ( void );

#endif /* WAVE_OUT_H */

#include <queue>

#define  WAVE_BUFFER_COUNT 2

#define  INP_BUFFER_SIZE 16000
#define  BITSPERSAMPLE   16
#define  SAMPLESPERSEC   8000
#define  CHANNELS        1

#define  MAX_FRAME_SIZE 2000
#define  MAX_FRAME_BYTES 2000

#define readint(buf, base) (((buf[base+3]<<24)&0xff000000)| \
	((buf[base+2]<<16)&0xff0000)| \
	((buf[base+1]<<8)&0xff00)| \
(buf[base]&0xff))
#define writeint(buf, base, val) do{ buf[base+3]=((val)>>24)&0xff; \
	buf[base+2]=((val)>>16)&0xff; \
	buf[base+1]=((val)>>8)&0xff; \
	buf[base]=(val)&0xff; \
}while(0)


class OggCodec
{
public:
	OggCodec();
	~OggCodec();

	struct PcmData 
	{
		short * data;
		long dataLength;
	};

	unsigned long Encode();
	unsigned long Decode();
	void PushPcmData(PcmData* pd);
	bool StartEncode(std::string encodeFileName, std::string comment);
	void StopEncode(bool sync = false);
	bool StartDecode(std::string decodeFileName);
	void StopDecode(bool sync = false);
	void PauseDecode(bool pause = true);
	void SetQuality(unsigned int level);
	bool DecodeSeek(unsigned int second);

	static unsigned long __stdcall EncodeProcess(void * param);
	static unsigned long __stdcall DecodeProcess(void * param);

	void comment_init(char **comments, int* length, char *vendor_string, int vendor_string_length);
	void comment_add(char **comments, int* length, char *tag, char *val, int val_length);
	int oe_write_page(ogg_page *page, FILE *fp);

	void* process_header(ogg_packet *op, spx_int32_t enh_enabled, spx_int32_t *frame_size, int *granule_frame_size, spx_int32_t *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo, int *extra_headers, int quiet);
	FILE* out_file_open(char *outFile, int rate, int *channels);

protected:
private:

	int nb_samples_enc;
	int total_samples_enc;
	int nb_encoded_enc;
	char *outFile_enc;
	FILE *fout_enc;
	short input[MAX_FRAME_SIZE];
	spx_int32_t frame_size_enc;
	spx_int32_t vbr_enabled_enc;
	spx_int32_t vbr_max_enc;
	int abr_enabled_enc;
	spx_int32_t vad_enabled_enc;
	spx_int32_t dtx_enabled_enc;
	int nbBytes_enc;
	const SpeexMode *mode_enc;
	int modeID_enc;
	void *st_enc;
	SpeexBits bits_enc;
	char cbits[MAX_FRAME_BYTES];
	int print_bitrate_enc;
	spx_int32_t rate_enc;
	spx_int32_t size_enc;
	int chan_enc;
	int fmt_enc;
	spx_int32_t quality_enc;
	float vbr_quality_enc;
	int lsb_enc;
	ogg_stream_state os_enc;
	ogg_stream_state so_enc;
	ogg_page 		 og_enc;
	ogg_packet 		 op_enc;
	int bytes_written_enc;
	int ret_enc;
	int result_enc;
	int id_enc;
	SpeexHeader header_enc;
	int nframes_enc;
	spx_int32_t complexity_enc;
	const char* speex_version_enc;
	char vendor_string[64];
	char *comments_enc;
	int comments_length_enc;
	//int close_in;
	int close_out_enc;
	int eos_enc;
	spx_int32_t bitrate_enc;
	double cumul_bits_enc;
	double enc_frames_enc;
//	char first_bytes[12];
	int wave_input_enc;
	spx_int32_t tmp_enc;
	SpeexPreprocessState *preprocess_enc;
	int denoise_enabled_enc;
	int agc_enabled_enc;
	spx_int32_t lookahead_enc;
	
	
	char *inFile_dec;
	FILE *fin_dec;
	short out[MAX_FRAME_SIZE];
	short output[MAX_FRAME_SIZE];
	int granule_frame_size_dec;
	int packet_count_dec;
	int stream_init_dec;
	ogg_int64_t page_granule_dec;
	ogg_int64_t last_granule_dec;
	int skip_samples_dec;
	int page_nb_packets_dec;
	ogg_sync_state oy_dec;
	int enh_enabled_dec;
	int forceMode_dec;
	int audio_size_dec;
	float loss_percent_dec;
	SpeexStereoState stereo_dec;
	int channels_dec;
	int extra_headers_dec;
	int wav_format_dec;
	int speex_serialno_dec;
	int frame_size_dec;
	void *st_dec;
	SpeexBits bits_dec;
	int quiet_dec;
	ogg_page       og_dec;
	ogg_packet     op_dec;
	ogg_stream_state os_dec;
	int nframes_dec;
	int close_in_dec;
	int print_bitrate_dec;
	int eos_dec;
	int rate_dec;
	int lookahead_dec;

	volatile bool bOggDecStop;
	HANDLE hOggDecThread;
	std::string sDecFileName;
	char comments[64];
		
	std::queue<PcmData*> OggQueue;
	HANDLE hOggEncThread;
	volatile bool bOggEncStop;
	short OggBuffer[INP_BUFFER_SIZE * 2];
	long OggBufferSize;
	long OggBufferLenght;
	long OggBufferPosition;
	std::string sEncFileName;

	bool m_bDecodePause;
	int file_size;
	CRITICAL_SECTION csDecodeFile;
	volatile bool m_bDecodeSeek;
};
#endif