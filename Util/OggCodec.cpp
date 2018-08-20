#include "StdAfx.h"
#include "OggCodec.h"
#include "encrypt.h"
#include <string>
#include "..\MultimediaPhoneDlg.h"
#include "windows.h"

extern HANDLE g_hThreadExit;
extern "C" char *PKEY;
static int encode_count =0;
void OggCodec::ReSetOggCodec()
{
	nb_samples_enc = 0;
	total_samples_enc = 0;
	nb_encoded_enc = 0;
	outFile_enc = NULL;
	fout_enc = NULL;
	frame_size_enc = 0;
	vbr_enabled_enc = 0;
	vbr_max_enc = 0;
	abr_enabled_enc = 0;
	vad_enabled_enc = 0;
	dtx_enabled_enc = 0;
	nbBytes_enc = 0;
	mode_enc = NULL;
	modeID_enc = -1;
	st_enc = NULL;
	print_bitrate_enc = 0;
	rate_enc = 0;
	size_enc = 0;
	chan_enc = 1;
	fmt_enc = 16;
	//quality_enc = 8; //wzx 20110106
	vbr_quality_enc = -1;
	lsb_enc = 1;
	bytes_written_enc = 0;
	ret_enc = 0;
	result_enc = 0;
	id_enc = 0;
	nframes_enc = 1;
	complexity_enc = 3;
	speex_version_enc = NULL;
	comments_enc = NULL;
	comments_length_enc = 0;
	close_out_enc = 0;
	eos_enc = 0;
	bitrate_enc = 0;
	cumul_bits_enc = 0;
	enc_frames_enc = 0;
	wave_input_enc = 0;
	tmp_enc = 0;
	preprocess_enc = NULL;
	denoise_enabled_enc = 1;
	agc_enabled_enc = 0; //lxz 20111110
	lookahead_enc = 0;
	
	inFile_dec = NULL;
	fin_dec = NULL;
	close_in_dec = 0;
	granule_frame_size_dec = 0;
	packet_count_dec = 0;
	stream_init_dec = 0;
	page_granule_dec = 0;
	last_granule_dec = 0;
	skip_samples_dec = 0;
	page_nb_packets_dec = 0;
	enh_enabled_dec = 1;
	forceMode_dec = -1;
	audio_size_dec = 0;
	loss_percent_dec = -1;
	stereo_dec.balance = 1;
	stereo_dec.e_ratio = 0.5;
	stereo_dec.smooth_left = 1;
	stereo_dec.smooth_right = 1;
	stereo_dec.reserved1 = 0;
	stereo_dec.reserved2 = 0;
	channels_dec = 1;
	extra_headers_dec = 0;
	wav_format_dec = 1;
	speex_serialno_dec = -1;
	frame_size_dec = 0;
	st_dec = NULL;
	quiet_dec = 0;
	nframes_dec = 1;
	print_bitrate_dec = 0;
	eos_dec = 0;
	rate_dec = 0;
	lookahead_dec = 0;
	file_size = 0;

	OggBufferSize = INP_BUFFER_SIZE * 2;
	OggBufferLenght = 0;
	OggBufferPosition = 0;

	m_bDecodePause = false;
	m_bDecodeSeek = false;
	m_deleleFileFlag=false;
}

OggCodec::OggCodec()
{
	nb_samples_enc = 0;
	total_samples_enc = 0;
	nb_encoded_enc = 0;
	outFile_enc = NULL;
	fout_enc = NULL;
	frame_size_enc = 0;
	vbr_enabled_enc = 0;
	vbr_max_enc = 0;
	abr_enabled_enc = 0;
	vad_enabled_enc = 0;
	dtx_enabled_enc = 0;
	nbBytes_enc = 0;
	mode_enc = NULL;
	modeID_enc = -1;
	st_enc = NULL;
	print_bitrate_enc = 0;
	rate_enc = 0;
	size_enc = 0;
	chan_enc = 1;
	fmt_enc = 16;
	quality_enc = 8;
	vbr_quality_enc = -1;
	lsb_enc = 1;
	bytes_written_enc = 0;
	ret_enc = 0;
	result_enc = 0;
	id_enc = 0;
	nframes_enc = 1;
	complexity_enc = 3;
	speex_version_enc = NULL;
	comments_enc = NULL;
	comments_length_enc = 0;
	close_out_enc = 0;
	eos_enc = 0;
	bitrate_enc = 0;
	cumul_bits_enc = 0;
	enc_frames_enc = 0;
	wave_input_enc = 0;
	tmp_enc = 0;
	preprocess_enc = NULL;
	denoise_enabled_enc = 1;
	agc_enabled_enc = 1;
	lookahead_enc = 0;
	
	inFile_dec = NULL;
	fin_dec = NULL;
	close_in_dec = 0;
	granule_frame_size_dec = 0;
	packet_count_dec = 0;
	stream_init_dec = 0;
	page_granule_dec = 0;
	last_granule_dec = 0;
	skip_samples_dec = 0;
	page_nb_packets_dec = 0;
	enh_enabled_dec = 1;
	forceMode_dec = -1;
	audio_size_dec = 0;
	loss_percent_dec = -1;
	stereo_dec.balance = 1;
	stereo_dec.e_ratio = 0.5;
	stereo_dec.smooth_left = 1;
	stereo_dec.smooth_right = 1;
	stereo_dec.reserved1 = 0;
	stereo_dec.reserved2 = 0;
	channels_dec = 1;
	extra_headers_dec = 0;
	wav_format_dec = 1;
	speex_serialno_dec = -1;
	frame_size_dec = 0;
	st_dec = NULL;
	quiet_dec = 0;
	nframes_dec = 1;
	print_bitrate_dec = 0;
	eos_dec = 0;
	rate_dec = 0;
	lookahead_dec = 0;
	file_size = 0;

	OggBufferSize = INP_BUFFER_SIZE * 2;
	OggBufferLenght = 0;
	OggBufferPosition = 0;

	m_bDecodePause = false;
	m_bDecodeSeek = false;
	m_deleleFileFlag=false;
	InitializeCriticalSection(&csDecodeFile);
}

OggCodec::~OggCodec()
{
	StopEncode(true);
	StopDecode(true);
}

unsigned long OggCodec::Encode()
{
	while (true)
	{
		if (!OggQueue.empty())
		{
			break;
		}
		else if (bOggEncStop)
		{
			return 0;
		}
		Sleep(10);
	}

	//lxz 20110111 lxz
	encode_count = 0;
	std::string s_ = sEncFileName.substr(sEncFileName.size()-4);
	if(s_== ".spd")
	{
		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect())
		{
			std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
			if(strTemp=="")
			{
				char *key="";
				DoEncryptFile_((char *)sEncFileName.c_str(),key,1);	
			}
			else
			{
				char* key=(char *)strTemp.c_str();
				DoEncryptFile_((char *)sEncFileName.c_str(),key,0);
			}
		}
	}
	speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, (void*)&speex_version_enc);

	memset(vendor_string, 0, sizeof(vendor_string));

	_snprintf(vendor_string, sizeof(vendor_string), "Encoded with Speex %s", speex_version_enc);

	comment_init(&comments_enc, &comments_length_enc, vendor_string, sizeof(vendor_string));

	comment_add(&comments_enc, &comments_length_enc, NULL, comments, sizeof(comments)); 

	//	srand(time(NULL));
	if (ogg_stream_init(&os_enc, rand())==-1)
	{
		return 0;
	}
	
	wave_input_enc=1;

	lsb_enc=1;
		
	rate_enc = SAMPLESPERSEC;

	chan_enc = CHANNELS;

	fmt_enc = BITSPERSAMPLE;

	if (rate_enc>48000)
	{
		return 0;
	}
	else if (rate_enc>25000)
	{
		modeID_enc = SPEEX_MODEID_UWB;
	}
	else if (rate_enc>12500)
	{
		modeID_enc = SPEEX_MODEID_WB;
	}
	else if (rate_enc>=6000)
	{
		modeID_enc = SPEEX_MODEID_NB;
	}
	else 
	{
		return 0;
	}
	
	if (!mode_enc)
		mode_enc = speex_lib_get_mode (modeID_enc);
	speex_init_header(&header_enc, rate_enc, 1, mode_enc);
	header_enc.frames_per_packet=nframes_enc;
	header_enc.vbr=vbr_enabled_enc;
	header_enc.nb_channels = chan_enc;
	st_enc = speex_encoder_init(mode_enc);

	outFile_enc = (char*)sEncFileName.c_str();

	close_out_enc=1;
	speex_encoder_ctl(st_enc, SPEEX_GET_FRAME_SIZE, &frame_size_enc);
	speex_encoder_ctl(st_enc, SPEEX_SET_COMPLEXITY, &complexity_enc);
	speex_encoder_ctl(st_enc, SPEEX_SET_SAMPLING_RATE, &rate_enc);
	speex_encoder_ctl(st_enc, SPEEX_SET_QUALITY, &quality_enc);
	speex_encoder_ctl(st_enc, SPEEX_GET_LOOKAHEAD, &lookahead_enc);
	
	//lxz 20111110
	/*
 	preprocess_enc = speex_preprocess_state_init(frame_size_enc, rate_enc);
	int noise_suppress = -25;  //wzx 20110114
	int agc_level = 32000;
 	speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_DENOISE, &denoise_enabled_enc);
	speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noise_suppress);
    speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_AGC, &agc_enabled_enc);
	speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_AGC_LEVEL,&agc_level);
	*/

	int packet_size;
	op_enc.packet = (unsigned char *)speex_header_to_packet(&header_enc, &packet_size);
	op_enc.bytes = packet_size;
	op_enc.b_o_s = 1;
	op_enc.e_o_s = 0;
	op_enc.granulepos = 0;
	op_enc.packetno = 0;
	ogg_stream_packetin(&os_enc, &op_enc);
	free(op_enc.packet);
	
	while((result_enc = ogg_stream_flush(&os_enc, &og_enc)))
	{
		if(!result_enc) break;
		ret_enc = oe_write_page(&og_enc, fout_enc);
		if(ret_enc != og_enc.header_len + og_enc.body_len)
		{
            return 0;   
		}
		else
            bytes_written_enc += ret_enc;
		Sleep(0); //lxz20101220
	}
	
	op_enc.packet = (unsigned char *)comments_enc;
	op_enc.bytes = comments_length_enc;
	op_enc.b_o_s = 0;
	op_enc.e_o_s = 0;
	op_enc.granulepos = 0;
	op_enc.packetno = 1;
	ogg_stream_packetin(&os_enc, &op_enc);
	
	while((result_enc = ogg_stream_flush(&os_enc, &og_enc)))
	{
		if(!result_enc) break;
		ret_enc = oe_write_page(&og_enc, fout_enc);
		if(ret_enc != og_enc.header_len + og_enc.body_len)
		{
			return 0;
		}
		else
			bytes_written_enc += ret_enc;
		Sleep(0); //lxz20101220
	}
	
	free(comments_enc);
	
	speex_bits_init(&bits_enc);

	while (true)
	{
		if (!OggQueue.empty())
		{
			PcmData* m = OggQueue.front();
			if (OggBufferLenght + m->dataLength <= OggBufferSize)
			{
				memcpy(OggBuffer, OggBuffer + OggBufferPosition, (OggBufferLenght - OggBufferPosition) * 2);
				memcpy(OggBuffer + OggBufferLenght - OggBufferPosition, m->data, m->dataLength * 2);
				OggBufferLenght = OggBufferLenght - OggBufferPosition + m->dataLength;
				OggBufferPosition = 0;
				free(m->data);
				delete m;
				OggQueue.pop();
			}
		}
		else if (bOggEncStop)
		{
			break;
		}
		else
		{
			Sleep(10);
			continue;
		}

 		while (OggBufferPosition < OggBufferLenght)
		{
			Sleep(0); //lxz20101220
			id_enc++;						
			size_enc = OggBufferLenght - OggBufferPosition;
	
			int len = frame_size_enc;
			nb_samples_enc = size_enc < len ? size_enc : len;
			memcpy(input, OggBuffer + OggBufferPosition, nb_samples_enc * 2);
			OggBufferPosition += nb_samples_enc;

			if (OggQueue.empty() && bOggEncStop && (size_enc <= 0))
				op_enc.e_o_s = 1;
			else
				op_enc.e_o_s = 0;

			speex_encode_int(st_enc, (spx_int16_t*)input, &bits_enc);
			if ((id_enc+1)%nframes_enc!=0)
				continue;

			speex_bits_insert_terminator(&bits_enc);
			nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
			speex_bits_reset(&bits_enc);
			op_enc.packet = (unsigned char *)cbits;
			op_enc.bytes = nbBytes_enc;
			op_enc.b_o_s = 0;
			
			if (OggQueue.empty() && bOggEncStop && (size_enc <= 0))
				op_enc.e_o_s = 1;
			else
				op_enc.e_o_s = 0;
			op_enc.granulepos = (id_enc+1)*frame_size_enc-lookahead_enc;
			op_enc.packetno = 2+id_enc/nframes_enc;
			ogg_stream_packetin(&os_enc, &op_enc);
		
			while (ogg_stream_pageout(&os_enc,&og_enc))
			{
				ret_enc = oe_write_page(&og_enc, fout_enc);
				if(ret_enc != og_enc.header_len + og_enc.body_len)
				{
					return 0;
				}
				else
				{
					bytes_written_enc += ret_enc;
				}
			}
		}
	}

	if ((id_enc+1)%nframes_enc!=0)
	{
		while ((id_enc+1)%nframes_enc!=0)
		{
			id_enc++;
			speex_bits_pack(&bits_enc, 15, 5);
		}
		nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
		op_enc.packet = (unsigned char *)cbits;
		op_enc.bytes = nbBytes_enc;
		op_enc.b_o_s = 0;
		op_enc.e_o_s = 1;
		op_enc.granulepos = (id_enc+1)*frame_size_enc-lookahead_enc;
		if (op_enc.granulepos>total_samples_enc)
			op_enc.granulepos = total_samples_enc;
		
		op_enc.packetno = 2+id_enc/nframes_enc;
		ogg_stream_packetin(&os_enc, &op_enc);
	}
	
	while (ogg_stream_flush(&os_enc, &og_enc))
	{
		Sleep(0); //lxz20101220
		ret_enc = oe_write_page(&og_enc, fout_enc);
		if(ret_enc != og_enc.header_len + og_enc.body_len)
		{
			return 0;
		}
		else
			bytes_written_enc += ret_enc;
	}
	
	speex_encoder_destroy(st_enc);
	speex_bits_destroy(&bits_enc);
	ogg_stream_clear(&os_enc);
	
	if (!OggQueue.empty())
	{
		for (int i = 0; i < OggQueue.size(); ++i)
		{
			PcmData* m = OggQueue.front();
			free(m->data);
			delete m;
			OggQueue.pop();
		}
	}

	//lxz 20110111 lxz
// 	CTime tmstart = CTime::GetCurrentTime();
// 	TRACE(L"rename start %dm %ds", tmstart.GetMinute(), tmstart.GetSecond());
 	RenameFile();
// 	CTime tmend = CTime::GetCurrentTime();
// 	TRACE(L"rename end %dm %ds", tmend.GetMinute(), tmend.GetSecond());

	nb_samples_enc=0;
	total_samples_enc=0;
	nb_encoded_enc=0;
	frame_size_enc = 0;
	vbr_enabled_enc=0;
	vbr_max_enc=0;
	abr_enabled_enc=0;
	vad_enabled_enc=0;
	dtx_enabled_enc=0;
	nbBytes_enc=0;
	mode_enc=NULL;
	modeID_enc = -1;
	st_enc = NULL;
	print_bitrate_enc=0;
	rate_enc=0;
	size_enc;
	chan_enc=1;
	fmt_enc=16;
	quality_enc=8;
	vbr_quality_enc=-1;
	lsb_enc=1;
	bytes_written_enc=0;
	ret_enc = 0;
	result_enc = 0;
	id_enc=-1;
	nframes_enc=1;
	complexity_enc=3;
	speex_version_enc = NULL;
	comments_enc = NULL;
	comments_length_enc = 0;
	close_out_enc = 0;
	eos_enc = 0;
	bitrate_enc = 0;
	cumul_bits_enc = 0;
	enc_frames_enc = 0;
	wave_input_enc = 0;
	tmp_enc = 0;
	preprocess_enc = NULL;
	denoise_enabled_enc = 1;
	agc_enabled_enc = 1;		// turn on:1 turn off:2
	lookahead_enc = 0;

	OggBufferLenght = 0;
	OggBufferPosition = 0;

	SetEvent(g_hThreadExit);
	return 0;
}

//lxz 20101230

static int DoFileXOR_(FILE *fin, char *mima, int &i, char *ptr){
    char ch;
	char p;
	int j = 0;
    fread(&ch,1,1,fin);
    while(!feof(fin)){
        if(mima[i]==0)i=0;
		p = ch^mima[i];
		ptr[j++] = p;
        i++;
		if(j == 200)
			break;
		fread(&ch,1,1,fin);
    }
	return j;
}   

unsigned long OggCodec::Decode(SPD_File *spd)
{
	if(!spd->isEncry)
	{
		inFile_dec = (char*)sDecFileName.c_str();
		fin_dec = fopen(inFile_dec, "rb");
		if (!fin_dec)
		{
			return 0;
		}
	}
	else
	{
		fin_dec = spd->file;
	}
	

	//wzx 20101230
	fseek(fin_dec, 0, SEEK_END);
	file_size = ftell(fin_dec);
	if(!spd->isEncry || spd->flag == -1 )
	{
		fseek(fin_dec, 0, SEEK_SET);
	}
	else if(spd->flag != -1)
	{
		int len = 82 + spd->passwordlen;
		fseek(fin_dec, len, SEEK_SET);
	}
	m_count = 0;
	close_in_dec=1;
	ogg_sync_init(&oy_dec);	
	speex_bits_init(&bits_dec);
	while (!bOggDecStop)
	{	
		char *data;
		int j;
		int nb_read;
		EnterCriticalSection(&csDecodeFile);
		if (m_bDecodeSeek)
		{
			m_bDecodeSeek = false;
			ogg_sync_reset(&oy_dec);
		}
		data = ogg_sync_buffer(&oy_dec, 200);
		
		//add by qi 20111019
		TRACE(L"Decode_Encryr\n");

		if(!spd->isEncry || spd->flag == -1)
		{
			nb_read = fread(data, sizeof(char), 200, fin_dec);
		}
		else
		{
			nb_read = DoFileXOR_(fin_dec, spd->pKey, m_count, data);
		}
		LeaveCriticalSection(&csDecodeFile);
		ogg_sync_wrote(&oy_dec, nb_read);

		while (ogg_sync_pageout(&oy_dec, &og_dec)==1)
		{
			if (bOggDecStop)
				break;
			if (m_bDecodeSeek)
			{
				break;
			}
			
			int packet_no;
			if (stream_init_dec == 0)
			{
				ogg_stream_init(&os_dec, ogg_page_serialno(&og_dec));
				stream_init_dec = 1;
			}
			
			if (ogg_page_serialno(&og_dec) != os_dec.serialno)
			{
				ogg_stream_reset_serialno(&os_dec, ogg_page_serialno(&og_dec));
			}
			
			ogg_stream_pagein(&os_dec, &og_dec);
			page_granule_dec = ogg_page_granulepos(&og_dec);
			page_nb_packets_dec = ogg_page_packets(&og_dec);
			if (page_granule_dec>0 && frame_size_dec)
			{
				skip_samples_dec = frame_size_dec*(page_nb_packets_dec*granule_frame_size_dec*nframes_dec - (page_granule_dec-last_granule_dec))/granule_frame_size_dec;
				if (ogg_page_eos(&og_dec))
					skip_samples_dec = -skip_samples_dec;
			} 
			else
			{
				skip_samples_dec = 0;
			}
			last_granule_dec = page_granule_dec;
			packet_no=0;
			while (!eos_dec && ogg_stream_packetout(&os_dec, &op_dec) == 1)
			{
				if (bOggDecStop)
					break;
				if (m_bDecodeSeek)
				{

					break;
				}
				
				if (!memcmp(op_dec.packet, "Speex", 5))
				{
					speex_serialno_dec = os_dec.serialno;
				}

				if (speex_serialno_dec == -1 || os_dec.serialno != speex_serialno_dec)
					break;
				if (packet_count_dec==0)
				{
					st_dec = process_header(&op_dec, enh_enabled_dec, (spx_int32_t*)&frame_size_dec, &granule_frame_size_dec, (spx_int32_t*)&rate_dec, &nframes_dec, forceMode_dec, &channels_dec, &stereo_dec, &extra_headers_dec, 0);
					if (!st_dec)
						exit(1);
					speex_decoder_ctl(st_dec, SPEEX_GET_LOOKAHEAD, &lookahead_dec);
					if (!nframes_dec)
						nframes_dec=1;
					out_file_open(NULL, rate_dec, &channels_dec);					
				}
				else if (packet_count_dec==1)
				{
				}
				else if (packet_count_dec<=1+extra_headers_dec)
				{
					/* Ignore extra headers */
				} 
				else 
				{
					int lost=0;
					packet_no++;
					if (loss_percent_dec>0 && 100*((float)rand())/RAND_MAX<loss_percent_dec)
						lost=1;
					
					if (op_dec.e_o_s && os_dec.serialno == speex_serialno_dec)
						eos_dec=1;
					
					speex_bits_read_from(&bits_dec,\
						(char*)op_dec.packet, op_dec.bytes);
					for (j=0;j!=nframes_dec;j++)
					{
						if (bOggDecStop)
							break;
						if (m_bDecodeSeek)
						{
							break;
						}

						int ret;
						if (!lost)
							ret = speex_decode_int(st_dec, &bits_dec, (spx_int16_t*)output);
						else
							ret = speex_decode_int(st_dec, NULL, (spx_int16_t*)output);
						
						if (ret==-1)
							break;
						if (ret==-2)
						{
							break;
						}
						if (speex_bits_remaining(&bits_dec)<0)
						{
							break;
						}
						if (channels_dec==2)
							speex_decode_stereo_int((spx_int16_t*)output, frame_size_dec, &stereo_dec);
							
						memcpy(out, output, frame_size_dec*channels_dec * sizeof(short));

						int frame_offset = 0;
						int new_frame_size = frame_size_dec;

						if (packet_no == 1 && j==0 && skip_samples_dec > 0)
						{
							new_frame_size -= skip_samples_dec+lookahead_dec;
							frame_offset = skip_samples_dec+lookahead_dec;
						}
						if (packet_no == page_nb_packets_dec && skip_samples_dec < 0)
						{
							int packet_length = nframes_dec*frame_size_dec+skip_samples_dec+lookahead_dec;
							new_frame_size = packet_length - j*frame_size_dec;
							if (new_frame_size<0)
								new_frame_size = 0;
							if (new_frame_size>frame_size_dec)
								new_frame_size = frame_size_dec;
						}

						if (new_frame_size>0)
						{  
							if (m_bDecodeSeek)
							{
								break;
							}
							WIN_Play_Samples (out+frame_offset*channels_dec, sizeof(short) * new_frame_size*channels_dec);
							audio_size_dec+=sizeof(short)*new_frame_size*channels_dec;
						}
					}
				}

				packet_count_dec++;
			}
		}
		if (feof(fin_dec))
			break;
	}				
	
	if (st_dec)
	{
		speex_decoder_destroy(st_dec);
	}
	
	speex_bits_destroy(&bits_dec);
	if (stream_init_dec)
		ogg_stream_clear(&os_dec);
	ogg_sync_clear(&oy_dec);
	
	WIN_Audio_close ();
	
	if (close_in_dec)
		fclose(fin_dec);

	frame_size_dec=0;
	granule_frame_size_dec=0;
	st_dec=NULL;
	packet_count_dec=0;
	stream_init_dec = 0;
	page_granule_dec=0;+
	last_granule_dec=0;
	skip_samples_dec=0;
	page_nb_packets_dec = 0;
	enh_enabled_dec=1;
	nframes_dec=2;
	print_bitrate_dec=0;
	close_in_dec=0;
	eos_dec=0;
	forceMode_dec=-1;
	audio_size_dec=0;
	loss_percent_dec=-1;
	channels_dec=1;
	rate_dec=0;
	extra_headers_dec=0;
	wav_format_dec=1;
	lookahead_dec = 0;
	speex_serialno_dec = -1;
	frame_size_dec = 0;
	st_dec = NULL;
	quiet_dec = 0;
	nframes_dec = 1;
	print_bitrate_dec = 0;
	eos_dec = 0;
	rate_dec = 0;
	lookahead_dec = 0;

	file_size = 0;
	bOggDecStop = true;
// 	CloseHandle(hOggDecThread);
// 	hOggDecThread = NULL;
	return 0;
}

void OggCodec::PushPcmData(PcmData* pd)
{
	OggQueue.push(pd);
}

void OggCodec::SetQuality(unsigned int level)
{
	if (level >= 0 && level <= 10)
	{
		quality_enc = level;
	}
}

bool OggCodec::StartEncode(std::string encodeFileName, std::string comment)
{
	ReSetOggCodec();  //wangzhenxing20101220

	sEncFileName = encodeFileName;
	memset(comments, 0, sizeof(comments));
	if (!comment.empty())
	{
		int len = comment.size() < sizeof(comments) ? comment.size() : sizeof(comments) - 1;

		memcpy(comments, comment.c_str(), len);
	}

	bOggEncStop = false;
	ResetEvent(g_hThreadExit);

	hOggEncThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EncodeProcess, this, 0, NULL);

	if (hOggEncThread == INVALID_HANDLE_VALUE)
	{
		hOggEncThread = NULL;
		bOggEncStop = true;
		SetEvent(g_hThreadExit);

		return false;
	}
	
	return true;
}

void OggCodec::StopEncode(bool sync)
{		
	bOggEncStop = true;
	TRACE(L"bOggEncStop = true\r\n");
}

void OggCodec::RenameFile()
{
	if (bOggEncStop)
	{
		CloseHandle(hOggEncThread);
		hOggEncThread = NULL;
		//lxz 20110111 lxz
// 		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect())
// 		{
// 			std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
// 			int index=sEncFileName.find('.')+1;
// 			std::string aFileName=sEncFileName.substr((size_t)0,(size_t)index)+"spx";
// 			char *inFile=(char *)sEncFileName.c_str();
// 			char *outFile=(char *)aFileName.c_str();
//             if(strTemp=="")
// 			{
//                 char *key="";
// 				DoEncryptFile(inFile,outFile,key,1);
// 				DeleteFile((LPCTSTR)CString(inFile));
// 				TRY
// 				{
// 					CFile::Rename((LPCTSTR)CString(outFile),(LPCTSTR)CString(inFile));
// 				}
// 				
// 				CATCH(CFileException,e)
// 				{
//                   #ifdef _DEBUG
// 					afxDump << "File" << (LPCTSTR)CString(outFile) << "not found,cause =" << e->m_cause <<"\n";
//                   #endif
// 				}
//                 END_CATCH 
// 	
// 			}else
// 			{
// 				char* key=(char *)strTemp.c_str();
//                 DoEncryptFile(inFile,outFile,key,0);
// 				DeleteFile((LPCTSTR)CString(inFile));
// 				TRY
// 				{
// 					CFile::Rename((LPCTSTR)CString(outFile),(LPCTSTR)CString(inFile));
// 				}
// 				
// 				CATCH(CFileException,e)
// 				{
//                      #ifdef _DEBUG
// 						afxDump << "File" << (LPCTSTR)CString(outFile) << "not found,cause =" << e->m_cause <<"\n";
//                      #endif
// 				}
// 
//                 END_CATCH 
// 			}
// 			
// 		}	

		return;	
	}
}

bool OggCodec::StartDecode(std::string decodeFileName)
{
	int index=decodeFileName.find('.')+1;
	
	std::string Fileext=decodeFileName.substr((size_t)index,(size_t)decodeFileName.length()-index);
	//lxz 20101230
	mSpd.isEncry = FALSE;
	mSpd.file = NULL;
	mSpd.pKey = NULL;
	mSpd.flag = -1;

	if(Fileext=="spd")	
	{  
		std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->\
			m_pSettingDlg->m_pSetting->playRecordPassword();
		sDecFileName=decodeFileName.substr((size_t)0,(size_t)index)+"spx";    
		char *inFile=(char *)decodeFileName.c_str();
		char *outFile=(char *)sDecFileName.c_str();
		char * key=(char *)strTemp.c_str();
		mSpd.isEncry = TRUE;  //lxz 20101230
        
		if(strTemp=="")
		{
			 EncryInfo info;
			 info=DoDecryptFile_(inFile, "", 1);    //lxz 20101230
			 mSpd.file = info.file;
			 mSpd.pKey = info.pKey;
			 mSpd.flag = info.flag;
			 mSpd.passwordlen = info.passwordlen;
		}
		else
		{
            EncryInfo info;
			info=DoDecryptFile_(inFile,key,1);	//lxz 20101230
			mSpd.file = info.file;
			mSpd.pKey = info.pKey;
			mSpd.flag = info.flag;
			mSpd.passwordlen = info.passwordlen;
			if(info.flag == -1)
			{
				CString failTip(L"");
				if(gLangItem == 1)
					failTip = L"解密文件失败";
				else
					failTip = L"Decoding Failure!";
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pPlaySoundDlg->getM_sticSoundName()->SetWindowText(failTip);
				
				return false;
			}
			else
			{
				m_deleleFileFlag = true;
			}    
		}
	}
	else
	{
		sDecFileName=decodeFileName;
		m_deleleFileFlag=false;
	}
	
	bOggDecStop = false;
	hOggDecThread = CreateThread(\
		NULL, 0, (LPTHREAD_START_ROUTINE)DecodeProcess, this, 0, NULL);

	if (hOggDecThread == INVALID_HANDLE_VALUE)
	{
		hOggDecThread = NULL;
		bOggDecStop = true;
		return false;
	}

	return true;
}

void OggCodec::StopDecode(bool sync)
{
	if (m_bDecodePause)
	{
		PauseDecode(false);
	}
   //在播放中使用暂停，会转到这个地方
	if (bOggDecStop)
	{
		if(m_deleleFileFlag)
		{
			
			char * outFile=(char *)sDecFileName.c_str();
			
			DeleteFile((LPCTSTR)CString(outFile));
		}

		return;
	}

	bOggDecStop = true;

//在播放中没有使用暂停，直接退出，转到这里
	if (sync)
	{
// 		while (hOggDecThread != NULL)
// 		{
// 			Sleep(10);
// 		}
		WaitForSingleObject(hOggDecThread,5000);

		CloseHandle(hOggDecThread);

		hOggDecThread = NULL;

		if(m_deleleFileFlag)
		{	
			char * outFile=(char *)sDecFileName.c_str();
			
			DeleteFile((LPCTSTR)CString(outFile));
		}

	}
}
void OggCodec::PauseDecode(bool pause)
{
	m_bDecodePause = pause;

	if (m_bDecodePause)
	{
		SuspendThread(hOggDecThread);
	}
	else
	{
		ResumeThread(hOggDecThread);
	}
}

bool OggCodec::DecodeSeek(unsigned int second)
{
	bool b = false;

	if (!bOggDecStop && (fin_dec != NULL) && (file_size > 0))
	{
		int pos = 276;
		int step = 4107;//2 quality page length
		if (sDecFileName.find("HQ") != std::string::npos)
		{
			step = 4239;//8 quality page length //4  4332
			pos += (second / 2) * step;
		}
		else/* if (sDecFileName.find("ST") != std::string::npos)*/
		{
			step = 4107;//2 quality page length
			pos += (second / 5) * step;
		}
		
		EnterCriticalSection(&csDecodeFile);

		if(mSpd.isEncry && mSpd.flag != -1)		//lxz 20101231
		{
			int pos_ = pos;
			pos += (82+mSpd.passwordlen);
			m_count = (pos_%strlen(mSpd.pKey));
		}

		if (fseek(fin_dec, pos, SEEK_SET) == 0)
		{
			m_bDecodeSeek = true;

			b = true;
		}
		LeaveCriticalSection(&csDecodeFile);
	}
	return b;
}

unsigned long __stdcall OggCodec::EncodeProcess(void * param)
{
	return ((OggCodec*)param)->Encode();
}

//lxz 20101230
unsigned long __stdcall OggCodec::DecodeProcess(void * param)
{
	return ((OggCodec*)param)->Decode(&((OggCodec*)param)->mSpd);
}

void OggCodec::comment_init(char **comments, int* length, char *vendor_string, int vendor_string_length)
{
	int vendor_length=vendor_string_length;//strlen(vendor_string);

	int user_comment_list_length=0;

	int len=4+vendor_length+4;

	char *p=(char*)malloc(len);

	if(p==NULL)
	{
		return;
	}
	writeint(p, 0, vendor_length);

	memcpy(p+4, vendor_string, vendor_length);

	writeint(p, 4+vendor_length, user_comment_list_length);

	*length=len;

	*comments=p;
}

void OggCodec::comment_add(char **comments, int* length, char *tag, char *val, int val_length)
{
	char* p=*comments;
	int vendor_length=readint(p, 0);
	int user_comment_list_length=readint(p, 4+vendor_length);
	int tag_len=(tag?strlen(tag):0);
	int val_len=val_length;//strlen(val);
	int len=(*length)+4+tag_len+val_len;
	
	p=(char*)realloc(p, len);

	if(p==NULL)
	{
		return;
	}
	
	writeint(p, *length, tag_len+val_len);      /* length of comment */
	if(tag) memcpy(p+*length+4, tag, tag_len);  /* comment */
	memcpy(p+*length+4+tag_len, val, val_len);  /* comment */
	writeint(p, 4+vendor_length, user_comment_list_length+1);
	
	*comments=p;
	*length=len;
}
extern std::string g_sOutDialRecordName;
extern BOOL g_bRecordFileDel;

static int DoFileXOR__(char *mima, int &i, unsigned char *ptr, int len){
    char ch;
	char p;
	int len_ = len;
	unsigned char *ptr_ = ptr;
    while(len_){
        if(mima[i]==0)i=0;
		p = *ptr_^mima[i];
		*ptr_ = p;
        i++;
		ptr_++;
		len_--;
    }
	return len;
}  

int OggCodec::oe_write_page(ogg_page *page, FILE *fp)
{
	int written = 0;
	if(!g_sOutDialRecordName.empty())
	{
		sEncFileName = g_sOutDialRecordName;
		g_sOutDialRecordName = "";
	}

	//lxz 20110111 lxz
	outFile_enc = (char*)sEncFileName.c_str();
	fp = fopen(outFile_enc, "a+b");
	
	if (fp)
	{
		std::string s_ = sEncFileName.substr(sEncFileName.size()-4);
		if(s_== ".spd")
		{
			DoFileXOR__(PKEY, encode_count, page->header,page->header_len);
			written = fwrite(page->header,1,page->header_len, fp);
			DoFileXOR__(PKEY, encode_count, page->body,page->body_len);
			written += fwrite(page->body,1,page->body_len, fp);
			fflush(fp);
			fclose(fp);
		}
		else
		{
			written = fwrite(page->header,1,page->header_len, fp);
			written += fwrite(page->body,1,page->body_len, fp);
			fflush(fp);
			fclose(fp);
		}

	}
	else
	{	
		DWORD nError = GetLastError();
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		::PostMessage(main->m_hWnd, WM_RECORD_FULL, 0, 0);
		::PostMessage(main->m_pSoundDlg->m_pRecordSoundDlg->m_hWnd, WM_RECORD_FULL, 0, 0);
		::PostMessage(main->m_pTelephoneDlg->m_hWnd, WM_RECORD_FULL, 0, 0);
	}

	return written;
}

//-----------------------------------------------------------------------------------------------------------//

FILE* OggCodec::out_file_open(char *outFile, int rate, int *channels)
{
   FILE *fout=NULL;
   /*Open output file*/
   if (outFile == NULL || strlen(outFile)==0)
   {
	   unsigned int speex_channels = *channels;
	   if (Set_WIN_Params (INVALID_FILEDESC, rate, SAMPLE_SIZE, speex_channels))
	   {
		   return NULL;
	   }
   }
   else
   {
		fout = fopen(outFile, "wb");
		if (fout)
		{
			if (strcmp(outFile+strlen(outFile)-4,".wav")==0 || strcmp(outFile+strlen(outFile)-4,".WAV")==0)
			{
				write_wav_header(fout, rate, *channels, 0, 0);
			}
		}
   }

   return fout;
}


void* OggCodec::process_header(ogg_packet *op, spx_int32_t enh_enabled, spx_int32_t *frame_size, int *granule_frame_size, spx_int32_t *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo, int *extra_headers, int quiet)
{
   void *st;
   const SpeexMode *mode;
   SpeexHeader *header;
   int modeID;
   SpeexCallback callback;
      
   header = speex_packet_to_header((char*)op->packet, op->bytes);
   if (!header)
   {
      return NULL;
   }
   if (header->mode >= SPEEX_NB_MODES || header->mode<0)
   {
      free(header);
      return NULL;
   }
      
   modeID = header->mode;
   if (forceMode!=-1)
      modeID = forceMode;

   mode = speex_lib_get_mode (modeID);
   
   if (header->speex_version_id > 1)
   {
      free(header);
      return NULL;
   }

   if (mode->bitstream_version < header->mode_bitstream_version)
   {
      free(header);
      return NULL;
   }
   if (mode->bitstream_version > header->mode_bitstream_version) 
   {
      free(header);
      return NULL;
   }
   
   st = speex_decoder_init(mode);
   if (!st)
   {
      free(header);
      return NULL;
   }
   speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
   speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);
   *granule_frame_size = *frame_size;

   if (!(*channels==1))
   {
      callback.callback_id = SPEEX_INBAND_STEREO;
      callback.func = speex_std_stereo_request_handler;
      callback.data = stereo;
      speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
   }
   if (!*rate)
      *rate = header->rate;
   /* Adjust rate if --force-* options are used */
   if (forceMode!=-1)
   {
      if (header->mode < forceMode)
      {
         *rate <<= (forceMode - header->mode);
         *granule_frame_size >>= (forceMode - header->mode);
      }
      if (header->mode > forceMode)
      {
         *rate >>= (header->mode - forceMode);
         *granule_frame_size <<= (header->mode - forceMode);
      }
   }

   speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);
   *nframes = header->frames_per_packet;
   if (*channels==-1)
      *channels = header->nb_channels;
   *extra_headers = header->extra_headers;
   free(header);
   return st;
}

