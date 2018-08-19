#include "SpeexDecoder.h"

#include <stdlib.h>

/*
void speex_encoder_ctl(void *encoder, int request, void *ptr);

void speex_decoder_ctl(void *encoder, int request, void *ptr); 

The different values of request allowed are (note that some only apply to the encoder or the decoder):

SPEEX_SET_ENH**
Set perceptual enhancer to on (1) or off (0) (integer) 
SPEEX_GET_ENH**
Get perceptual enhancer status (integer) 
SPEEX_GET_FRAME_SIZE
Get the number of samples per frame for the current mode (integer) 
SPEEX_SET_QUALITY*
Set the encoder speech quality (integer 0 to 10) 
SPEEX_GET_QUALITY*
Get the current encoder speech quality (integer 0 to 10) 
SPEEX_SET_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_GET_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_SET_LOW_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_GET_LOW_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_SET_HIGH_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_GET_HIGH_MODE*$ \dagger$
Use the source, Luke! 
SPEEX_SET_VBR*
Set variable bit-rate (VBR) to on (1) or off (0) (integer) 
SPEEX_GET_VBR*
Get variable bit-rate (VBR) status (integer) 
SPEEX_SET_VBR_QUALITY*
Set the encoder VBR speech quality (float 0 to 10) 
SPEEX_GET_VBR_QUALITY*
Get the current encoder VBR speech quality (float 0 to 10) 
SPEEX_SET_COMPLEXITY*
Set the CPU resources allowed for the encoder (integer 1 to 10) 
SPEEX_GET_COMPLEXITY*
Get the CPU resources allowed for the encoder (integer 1 to 10) 
SPEEX_SET_BITRATE*
Set the bit-rate to use to the closest value not exceeding the parameter (integer in bps) 
SPEEX_GET_BITRATE
Get the current bit-rate in use (integer in bps) 
SPEEX_SET_SAMPLING_RATE
Set real sampling rate (integer in Hz) 
SPEEX_GET_SAMPLING_RATE
Get real sampling rate (integer in Hz) 
SPEEX_RESET_STATE
Reset the encoder/decoder state to its original state (zeros all memories) 
SPEEX_SET_VAD*
Set voice activity detection (VAD) to on (1) or off (0) (integer) 
SPEEX_GET_VAD*
Get voice activity detection (VAD) status (integer) 
SPEEX_SET_DTX*
Set discontinuous transmission (DTX) to on (1) or off (0) (integer) 
SPEEX_GET_DTX*
Get discontinuous transmission (DTX) status (integer) 
SPEEX_SET_ABR*
Set average bit-rate (ABR) to a value n in bits per second (integer in bps) 
SPEEX_GET_ABR*
Get average bit-rate (ABR) setting (integer in bps) 
SPEEX_SET_PLC_TUNING*
Tell the encoder to optimize encoding for a certain percentage of packet loss (integer in percent) 
SPEEX_GET_PLC_TUNING*
Get the current tuning of the encoder for PLC (integer in percent) 
*
applies only to the encoder 
**
applies only to the decoder 
$ \dagger$
normally only used internally 
*/
/*
void speex_mode_query(SpeexMode *mode, int request, void *ptr);
The admissible values for request are (unless otherwise note, the values are returned through ptr):

SPEEX_MODE_FRAME_SIZE
Get the frame size (in samples) for the mode 
SPEEX_SUBMODE_BITRATE
Get the bit-rate for a submode number specified through ptr (integer in bps). 
*/
/*
Preprocessor options

SPEEX_PREPROCESS_SET_DENOISE
Turns denoising on(1) or off(2) (integer) 
SPEEX_PREPROCESS_GET_DENOISE
Get denoising status (integer) 
SPEEX_PREPROCESS_SET_AGC
Turns automatic gain control (AGC) on(1) or off(2) (integer) 
SPEEX_PREPROCESS_GET_AGC
Get AGC status (integer) 
SPEEX_PREPROCESS_SET_VAD
Turns voice activity detector (VAD) on(1) or off(2) (integer) 
SPEEX_PREPROCESS_GET_VAD
Get VAD status (integer) 
SPEEX_PREPROCESS_SET_AGC_LEVEL

SPEEX_PREPROCESS_GET_AGC_LEVEL

SPEEX_PREPROCESS_SET_DEREVERB
Turns reverberation removal on(1) or off(2) (integer) 
SPEEX_PREPROCESS_GET_DEREVERB
Get reverberation removal status (integer) 
SPEEX_PREPROCESS_SET_DEREVERB_LEVEL

SPEEX_PREPROCESS_GET_DEREVERB_LEVEL

SPEEX_PREPROCESS_SET_DEREVERB_DECAY

SPEEX_PREPROCESS_GET_DEREVERB_DECAY

SPEEX_PREPROCESS_SET_PROB_START

SPEEX_PREPROCESS_GET_PROB_START

SPEEX_PREPROCESS_SET_PROB_CONTINUE

SPEEX_PREPROCESS_GET_PROB_CONTINUE

SPEEX_PREPROCESS_SET_NOISE_SUPPRESS
Set maximum attenuation of the noise in dB (negative number) 
SPEEX_PREPROCESS_GET_NOISE_SUPPRESS
Get maximum attenuation of the noise in dB (negative number) 
SPEEX_PREPROCESS_SET_ECHO_SUPPRESS
Set maximum attenuation of the residual echo in dB (negative number) 
SPEEX_PREPROCESS_GET_ECHO_SUPPRESS
Set maximum attenuation of the residual echo in dB (negative number) 
SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE
Set maximum attenuation of the echo in dB when near end is active (negative number) 
SPEEX_PREPROCESS_GET_ECHO_SUPPRESS_ACTIVE
Set maximum attenuation of the echo in dB when near end is active (negative number) 
SPEEX_PREPROCESS_SET_ECHO_STATE
Set the associated echo canceller for residual echo suppression (NULL for no residual echo suppression) 
SPEEX_PREPROCESS_GET_ECHO_STATE
Get the associated echo canceller 
*/
/*
Table 1: In-band signalling codes
Code    Size (bits)     Content
0   1   Asks decoder to set perceptual enhancement off (0) or on(1)
1   1   Asks (if 1) the encoder to be less ``agressive'' due to high packet loss
2   4   Asks encoder to switch to mode N
3   4   Asks encoder to switch to mode N for low-band
4   4   Asks encoder to switch to mode N for high-band
5   4   Asks encoder to switch to quality N for VBR
6   4   Request acknowloedge (0=no, 1=all, 2=only for in-band data)
7   4   Asks encoder to set CBR (0), VAD(1), DTX(3), VBR(5), VBR+DTX(7)
8   8   Transmit (8-bit) character to the other end
9   8   Intensity stereo information
10  16  Announce maximum bit-rate acceptable (N in bytes/second)
11  16  reserved
12  32  Acknowledge receiving packet N
13  32  reserved
14  64  reserved
15  64  reserved
*/
/*
Table 2: Ogg/Speex header packet
Field                   Type    Size
speex_string            char[]  8
speex_version           char[]  20
speex_version_id        int     4
header_size             int     4
rate                    int     4
mode                    int     4
mode_bitstream_version  int     4
nb_channels             int     4
bitrate                 int     4
frame_size              int     4
vbr                     int     4
frames_per_packet       int     4
extra_headers           int     4
reserved1               int     4
reserved2               int     4
*/
namespace Util {
    namespace Audio {
        SpeexDecoder::SpeexDecoder()
        : dec_state_(0)
        , file_(0) {
            memset(version_, 0, 256);
            speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, (void*)version_);

            //SpeexPreprocessState *preprocess_state = speex_preprocess_state_init(frame_size, sampling_rate);
            //speex_preprocess_ctl(preprocess_state, request, ptr);
            //while (true) {
            //    speex_preprocess_run(preprocess_state, audio_frame);
            //    speex_preprocess_estimate_update(preprocess_state, audio_frame);
            //}
            //speex_preprocess_state_destroy(preprocess_state);


            //SpeexEchoState *echo_state = speex_echo_state_init(frame_size, filter_length);
            //speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_ECHO_STATE, echo_state);
            //while (true) {
            //    write_to_soundcard(echo_frame, frame_size);
            //    read_from_soundcard(input_frame, frame_size);
            //    speex_echo_cancellation(echo_state, input_frame, echo_frame, output_frame);
            //    speex_echo_capture(echo_state, input_frame, output_frame);
            //    speex_echo_playback(echo_state, echo_frame);
            //}
            //speex_echo_state_reset(echo_state);
            //speex_echo_state_destroy(echo_state);


            //echo_diagnostic('aec_rec.sw', 'aec_play.sw', 'aec_diagnostic.sw', 1024);
        }

        SpeexDecoder::~SpeexDecoder() {
        }

        void SpeexDecoder::InitDecode(SpeexMode const & mode, size_t const frameSize, int const enh) {
            SpeexBits bits_;
            speex_bits_init(&bits_);
            /*void **/dec_state_ = speex_decoder_init(&mode); //speex_nb_mode, speex_wb_mode, speex_uwb_mode
            speex_decoder_ctl(dec_state_, SPEEX_GET_FRAME_SIZE, const_cast<size_t*>(&frameSize));
            speex_decoder_ctl(dec_state_, SPEEX_SET_ENH, const_cast<int*>(&enh));
        }
        std::wstring const SpeexDecoder::Decode(char* in, size_t const length) {
            speex_bits_read_from(&bits_, in, length);
            spx_int16_t* out = static_cast<spx_int16_t*>(malloc(1000));
            speex_decode_int(dec_state_, &bits_, out);
            return std::wstring(); //(static_cast<wchar_t*>(out), length);
        }

        void SpeexDecoder::UninitDecode() {
            speex_decoder_destroy(dec_state_);
            speex_bits_destroy(&bits_);
        }
    }
}

#if 0
#include <speex/speex.h>
#include <stdio.h>

/*The frame size in hardcoded for this sample code but it doesn't have to be*/
#define FRAME_SIZE 160
int main(int argc, char **argv)
{
    char *inFile;
    FILE *fin;
    short in[FRAME_SIZE];
    float input[FRAME_SIZE];
    char cbits[200];
    int nbBytes;
    /*Holds the state of the encoder*/
    void *state;
    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    int i, tmp;

    /*Create a new encoder state in narrowband mode*/
    state = speex_encoder_init(&speex_nb_mode);

    /*Set the quality to 8 (15 kbps)*/
    tmp=8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

    inFile = argv[1];
    fin = fopen(inFile, "r");

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);
    while (1)
    {
        /*Read a 16 bits/sample audio frame*/
        fread(in, sizeof(short), FRAME_SIZE, fin);
        if (feof(fin))
            break;
        /*Copy the 16 bits values to float so Speex can work on them*/
        for (i=0;i<FRAME_SIZE;i++)
            input[i]=in[i];

        /*Flush all the bits in the struct so we can encode a new frame*/
        speex_bits_reset(&bits);

        /*Encode the frame*/
        speex_encode(state, input, &bits);
        /*Copy the bits to an array of char that can be written*/
        nbBytes = speex_bits_write(&bits, cbits, 200);

        /*Write the size of the frame first. This is what sampledec expects but
        it's likely to be different in your own application*/
        fwrite(&nbBytes, sizeof(int), 1, stdout);
        /*Write the compressed data*/
        fwrite(cbits, 1, nbBytes, stdout);

    }

    /*Destroy the encoder state*/
    speex_encoder_destroy(state);
    /*Destroy the bit-packing struct*/
    speex_bits_destroy(&bits);
    fclose(fin);
    return 0;
}

#include <speex/speex.h>
#include <stdio.h>

/*The frame size in hardcoded for this sample code but it doesn't have to be*/
#define FRAME_SIZE 160
int main(int argc, char **argv)
{
    char *outFile;
    FILE *fout;
    /*Holds the audio that will be written to file (16 bits per sample)*/
    short out[FRAME_SIZE];
    /*Speex handle samples as float, so we need an array of floats*/
    float output[FRAME_SIZE];
    char cbits[200];
    int nbBytes;
    /*Holds the state of the decoder*/
    void *state;
    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    int i, tmp;

    /*Create a new decoder state in narrowband mode*/
    state = speex_decoder_init(&speex_nb_mode);

    /*Set the perceptual enhancement on*/
    tmp=1;
    speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

    outFile = argv[1];
    fout = fopen(outFile, "w");

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);
    while (1)
    {
        /*Read the size encoded by sampleenc, this part will likely be 
        different in your application*/
        fread(&nbBytes, sizeof(int), 1, stdin);
        fprintf (stderr, "nbBytes: %d\n", nbBytes);
        if (feof(stdin))
            break;

        /*Read the "packet" encoded by sampleenc*/
        fread(cbits, 1, nbBytes, stdin);
        /*Copy the data into the bit-stream struct*/
        speex_bits_read_from(&bits, cbits, nbBytes);

        /*Decode the data*/
        speex_decode(state, &bits, output);

        /*Copy from float to short (16 bits) for output*/
        for (i=0;i<FRAME_SIZE;i++)
            out[i]=output[i];

        /*Write the decoded audio to file*/
        fwrite(out, sizeof(short), FRAME_SIZE, fout);
    }

    /*Destroy the decoder state*/
    speex_decoder_destroy(state);
    /*Destroy the bit-stream truct*/
    speex_bits_destroy(&bits);
    fclose(fout);
    return 0;
}





while (true) {
    if (!OggQueue.empty()) {
        break;
    } else if (bOggEncStop) {
        return 0;
    }
    Sleep(10);
}
speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, (void*)&speex_version_enc);
memset(vendor_string, 0, sizeof(vendor_string));
_snprintf(vendor_string, sizeof(vendor_string), "Encoded with Speex %s", speex_version_enc);
comment_init(&comments_enc, &comments_length_enc, vendor_string, sizeof(vendor_string));
comment_add(&comments_enc, &comments_length_enc, NULL, comments, sizeof(comments)); 
//srand(time(NULL));
if (ogg_stream_init(&os_enc, rand())==-1) {
    return 0;
}
wave_input_enc=1;
lsb_enc=1;
rate_enc = SAMPLESPERSEC;
chan_enc = CHANNELS;
fmt_enc = BITSPERSAMPLE;
if (rate_enc>48000) {
    return 0;
} else if (rate_enc>25000) {
    modeID_enc = SPEEX_MODEID_UWB;
} else if (rate_enc>12500) {
    modeID_enc = SPEEX_MODEID_WB;
} else if (rate_enc>=6000) {
    modeID_enc = SPEEX_MODEID_NB;
} else {
    return 0;
}
if (!mode_enc) {
    mode_enc = speex_lib_get_mode (modeID_enc);
}
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
int packet_size;
op_enc.packet = (unsigned char *)speex_header_to_packet(&header_enc, &packet_size);
op_enc.bytes = packet_size;
op_enc.b_o_s = 1;
op_enc.e_o_s = 0;
op_enc.granulepos = 0;
op_enc.packetno = 0;
ogg_stream_packetin(&os_enc, &op_enc);
free(op_enc.packet);
while((result_enc = ogg_stream_flush(&os_enc, &og_enc))) {
    if (!result_enc) {
        break;
    }
    ret_enc = oe_write_page(&og_enc, fout_enc);
    if (ret_enc != og_enc.header_len + og_enc.body_len) {
        return 0;
    } else {
        bytes_written_enc += ret_enc;
    }
}
op_enc.packet = (unsigned char *)comments_enc;
op_enc.bytes = comments_length_enc;
op_enc.b_o_s = 0;
op_enc.e_o_s = 0;
op_enc.granulepos = 0;
op_enc.packetno = 1;
ogg_stream_packetin(&os_enc, &op_enc);
while ((result_enc = ogg_stream_flush(&os_enc, &og_enc))) {
    if (!result_enc) {
        break;
    }
    ret_enc = oe_write_page(&og_enc, fout_enc);
    if (ret_enc != og_enc.header_len + og_enc.body_len) {
        return 0;
    } else {
        bytes_written_enc += ret_enc;
    }
}
free(comments_enc);
speex_bits_init(&bits_enc);
while (true) {
    if (!OggQueue.empty()) {
        PcmData* m = OggQueue.front();
        if (OggBufferLenght + m->dataLength <= OggBufferSize) {
            memcpy(OggBuffer, OggBuffer + OggBufferPosition, (OggBufferLenght - OggBufferPosition) * 2);
            memcpy(OggBuffer + OggBufferLenght - OggBufferPosition, m->data, m->dataLength * 2);
            OggBufferLenght = OggBufferLenght - OggBufferPosition + m->dataLength;
            OggBufferPosition = 0;
            free(m->data);
            delete m;
            OggQueue.pop();
        }
    } else if (bOggEncStop) {
        break;
    } else {
        Sleep(10);
        continue;
    }
    while (OggBufferPosition < OggBufferLenght) {
        ++id_enc;                       
        size_enc = OggBufferLenght - OggBufferPosition;

        int len = frame_size_enc;
        nb_samples_enc = size_enc < len ? size_enc : len;
        memcpy(input, OggBuffer + OggBufferPosition, nb_samples_enc * 2);
        OggBufferPosition += nb_samples_enc;

        if (OggQueue.empty() && bOggEncStop && (size_enc <= 0)) {
            op_enc.e_o_s = 1;
        } else {
            op_enc.e_o_s = 0;
        }
        speex_encode_int(st_enc, (spx_int16_t*)input, &bits_enc);
        if ((id_enc+1)%nframes_enc!=0) {
            continue;
        }
        speex_bits_insert_terminator(&bits_enc);
        nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
        speex_bits_reset(&bits_enc);
        op_enc.packet = (unsigned char *)cbits;
        op_enc.bytes = nbBytes_enc;
        op_enc.b_o_s = 0;
        if (OggQueue.empty() && bOggEncStop && (size_enc <= 0)) {
            op_enc.e_o_s = 1;
        } else {
            op_enc.e_o_s = 0;
        }
        op_enc.granulepos = (id_enc+1)*frame_size_enc-lookahead_enc;
        op_enc.packetno = 2+id_enc/nframes_enc;
        ogg_stream_packetin(&os_enc, &op_enc);
        while (ogg_stream_pageout(&os_enc,&og_enc)) {
            ret_enc = oe_write_page(&og_enc, fout_enc);
            if(ret_enc != og_enc.header_len + og_enc.body_len) {
                return 0;
            } else {
                bytes_written_enc += ret_enc;
            }
        }
    }
}
if ((id_enc+1)%nframes_enc!=0) {
    while ((id_enc+1)%nframes_enc!=0) {
        ++id_enc;
        speex_bits_pack(&bits_enc, 15, 5);
    }
    nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
    op_enc.packet = (unsigned char *)cbits;
    op_enc.bytes = nbBytes_enc;
    op_enc.b_o_s = 0;
    op_enc.e_o_s = 1;
    op_enc.granulepos = (id_enc+1)*frame_size_enc-lookahead_enc;
    if (op_enc.granulepos>total_samples_enc) {
        op_enc.granulepos = total_samples_enc;
    }
    op_enc.packetno = 2+id_enc/nframes_enc;
    ogg_stream_packetin(&os_enc, &op_enc);
}
while (ogg_stream_flush(&os_enc, &og_enc)) {
    ret_enc = oe_write_page(&og_enc, fout_enc);
    if(ret_enc != og_enc.header_len + og_enc.body_len) {
        return 0;
    } else {
        bytes_written_enc += ret_enc;
    }
}
speex_encoder_destroy(st_enc);
speex_bits_destroy(&bits_enc);
ogg_stream_clear(&os_enc);
if (!OggQueue.empty()) {
    for (size_t i = 0; i < OggQueue.size(); ++i) {
        PcmData* m = OggQueue.front();
        free(m->data);
        delete m;
        OggQueue.pop();
    }
}
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
agc_enabled_enc = 0;
lookahead_enc = 0;
OggBufferLenght = 0;
OggBufferPosition = 0;



inFile_dec = (char*)sDecFileName.c_str();
fin_dec = fopen(inFile_dec, "rb");
if (!fin_dec) {
    return 0;
}
fseek(fin_dec, 0, SEEK_END);
file_size = ftell(fin_dec);
fseek(fin_dec, 0, SEEK_SET);
close_in_dec=1;
ogg_sync_init(&oy_dec); 
speex_bits_init(&bits_dec);
while (!bOggDecStop) {
    char *data;
    //int i;
    int j;
    int nb_read;
    EnterCriticalSection(&csDecodeFile);
    if (m_bDecodeSeek) {
        m_bDecodeSeek = false;
        ogg_sync_reset(&oy_dec);
    }       
    data = ogg_sync_buffer(&oy_dec, 200);
    nb_read = fread(data, sizeof(char), 200, fin_dec);
    LeaveCriticalSection(&csDecodeFile);
    ogg_sync_wrote(&oy_dec, nb_read);
    while (ogg_sync_pageout(&oy_dec, &og_dec)==1) {
        if (bOggDecStop) {
            break;
        }
        if (m_bDecodeSeek) {
            break;
        }
        int packet_no;
        if (stream_init_dec == 0) {
            ogg_stream_init(&os_dec, ogg_page_serialno(&og_dec));
            stream_init_dec = 1;
        }
        if (ogg_page_serialno(&og_dec) != os_dec.serialno) {
            ogg_stream_reset_serialno(&os_dec, ogg_page_serialno(&og_dec));
        }
        ogg_stream_pagein(&os_dec, &og_dec);
        page_granule_dec = ogg_page_granulepos(&og_dec);
        page_nb_packets_dec = ogg_page_packets(&og_dec);
        if (page_granule_dec>0 && frame_size_dec) {
            skip_samples_dec = frame_size_dec * (page_nb_packets_dec * granule_frame_size_dec * nframes_dec - (page_granule_dec - last_granule_dec)) / granule_frame_size_dec;
            if (ogg_page_eos(&og_dec)) {
                skip_samples_dec = -skip_samples_dec;
            }
        } else {
            skip_samples_dec = 0;
        }
        last_granule_dec = page_granule_dec;
        packet_no = 0;
        while (!eos_dec && ogg_stream_packetout(&os_dec, &op_dec) == 1) {
            if (bOggDecStop) {
                break;
            }
            if (m_bDecodeSeek) {
                break;
            }
            if (!memcmp(op_dec.packet, "Speex", 5)) {
                speex_serialno_dec = os_dec.serialno;
            }
            if (speex_serialno_dec == -1 || os_dec.serialno != speex_serialno_dec) {
                break;
            }
            if (packet_count_dec==0) {
                st_dec = process_header(&op_dec, enh_enabled_dec, (spx_int32_t*)&frame_size_dec, &granule_frame_size_dec, (spx_int32_t*)&rate_dec, &nframes_dec, forceMode_dec, &channels_dec, &stereo_dec, &extra_headers_dec, 0);
                if (!st_dec) {
                    exit(1);
                }
                speex_decoder_ctl(st_dec, SPEEX_GET_LOOKAHEAD, &lookahead_dec);
                if (!nframes_dec) {
                    nframes_dec=1;
                }
                out_file_open(NULL, rate_dec, &channels_dec);                   
            } else if (packet_count_dec==1) {
                //
            } else if (packet_count_dec<=1+extra_headers_dec) {
                // Ignore extra headers
            } else {
                int lost = 0;
                ++packet_no;
                if (loss_percent_dec > 0 && 100 * ((float)rand()) / RAND_MAX < loss_percent_dec) {
                    lost = 1;
                }
                if (op_dec.e_o_s && os_dec.serialno == speex_serialno_dec) {
                    eos_dec = 1;
                }
                speex_bits_read_from(&bits_dec, (char*)op_dec.packet, op_dec.bytes);
                for (j = 0;j != nframes_dec; ++j) {
                    if (bOggDecStop) {
                        break;
                    }
                    if (m_bDecodeSeek) {
                        break;
                    }
                    int ret;
                    if (!lost) {
                        ret = speex_decode_int(st_dec, &bits_dec, (spx_int16_t*)output);
                    } else {
                        ret = speex_decode_int(st_dec, NULL, (spx_int16_t*)output);
                    }
                    if (ret == -1) {
                        break;
                    }
                    if (ret == -2) {
                        break;
                    }
                    if (speex_bits_remaining(&bits_dec) < 0) {
                        break;
                    }
                    if (channels_dec == 2) {
                        speex_decode_stereo_int((spx_int16_t*)output, frame_size_dec, &stereo_dec);
                    }
                    memcpy(out, output, frame_size_dec * channels_dec * sizeof(short));
                    int frame_offset = 0;
                    int new_frame_size = frame_size_dec;
                    if (packet_no == 1 && j==0 && skip_samples_dec > 0) {
                        new_frame_size -= skip_samples_dec + lookahead_dec;
                        frame_offset = skip_samples_dec + lookahead_dec;
                    }
                    if (packet_no == page_nb_packets_dec && skip_samples_dec < 0) {
                        int packet_length = nframes_dec * frame_size_dec + skip_samples_dec + lookahead_dec;
                        new_frame_size = packet_length - j * frame_size_dec;
                        if (new_frame_size < 0) {
                            new_frame_size = 0;
                        }
                        if (new_frame_size > frame_size_dec) {
                            new_frame_size = frame_size_dec;
                        }
                    }
                    if (new_frame_size > 0) {
                        if (m_bDecodeSeek) {
                            //EnterCriticalSection(&csDecodeFile);
                            //m_bDecodeSeek = false;
                            //LeaveCriticalSection(&csDecodeFile);
                            //ogg_sync_reset(&oy_dec);
                            break;
                        }
                        WIN_Play_Samples(out + frame_offset * channels_dec, sizeof(short) * new_frame_size * channels_dec);
                        audio_size_dec += sizeof(short) * new_frame_size * channels_dec;
                    }
                }
            }
            ++packet_count_dec;
        }
    }
    if (feof(fin_dec)) {
        break;
    }
}               
if (st_dec) {
    speex_decoder_destroy(st_dec);
}
speex_bits_destroy(&bits_dec);
if (stream_init_dec) {
    ogg_stream_clear(&os_dec);
}
ogg_sync_clear(&oy_dec);
WIN_Audio_close ();
if (close_in_dec) {
    fclose(fin_dec);
}
frame_size_dec = 0;
granule_frame_size_dec = 0;
st_dec = NULL;
packet_count_dec = 0;
stream_init_dec = 0;
page_granule_dec = 0;
last_granule_dec = 0;
skip_samples_dec = 0;
page_nb_packets_dec = 0;
enh_enabled_dec = 1;
nframes_dec = 2;
print_bitrate_dec = 0;
close_in_dec = 0;
eos_dec = 0;
forceMode_dec = -1;
audio_size_dec = 0;
loss_percent_dec = -1;
channels_dec = 1;
rate_dec = 0;
extra_headers_dec = 0;
wav_format_dec = 1;
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

void SpeexCodec::comment_init(char **comments, int* length, char *vendor_string, int vendor_string_length) {
    int vendor_length=vendor_string_length;//strlen(vendor_string);
    int user_comment_list_length=0;
    int len=4+vendor_length+4;
    char *p=(char*)malloc(len);
    if(p==NULL) {
        return;
    }
    writeint(p, 0, vendor_length);
    memcpy(p+4, vendor_string, vendor_length);
    writeint(p, 4+vendor_length, user_comment_list_length);
    *length=len;
    *comments=p;
}

void SpeexCodec::comment_add(char **comments, int* length, char *tag, char *val, int val_length) {
    char* p=*comments;
    int vendor_length=readint(p, 0);
    int user_comment_list_length=readint(p, 4+vendor_length);
    int tag_len=(tag?strlen(tag):0);
    int val_len=val_length;//strlen(val);
    int len=(*length)+4+tag_len+val_len;

    p=(char*)realloc(p, len);
    if(p==NULL) {
        return;
    }

    writeint(p, *length, tag_len+val_len);      /* length of comment */
    if(tag) {
        memcpy(p+*length+4, tag, tag_len);  /* comment */
    }
    memcpy(p+*length+4+tag_len, val, val_len);  /* comment */
    writeint(p, 4+vendor_length, user_comment_list_length+1);

    *comments=p;
    *length=len;
}

int SpeexCodec::oe_write_page(ogg_page *page, FILE *fp) {
    int written = 0;
    outFile_enc = (char*)sEncFileName.c_str();
    fp = fopen(outFile_enc, "a+b");
    if (fp) {
        written = fwrite(page->header,1,page->header_len, fp);
        written += fwrite(page->body,1,page->body_len, fp);
        fflush(fp);
        fclose(fp);
    }
    return written;
}

FILE* SpeexCodec::out_file_open(char *outFile, int rate, int *channels) {
    FILE *fout=NULL;
    /*Open output file*/
    if (outFile == NULL || strlen(outFile)==0) {
        unsigned int speex_channels = *channels;
        if (Set_WIN_Params (INVALID_FILEDESC, rate, SAMPLE_SIZE, speex_channels)) {
            return NULL;
        }
    } else {
        fout = fopen(outFile, "wb");
        if (fout) {
            if (strcmp(outFile+strlen(outFile)-4,".wav")==0 || strcmp(outFile+strlen(outFile)-4,".WAV")==0) {
                write_wav_header(fout, rate, *channels, 0, 0);
            }
        }
    }

    return fout;
}


void* SpeexCodec::process_header(ogg_packet *op, spx_int32_t enh_enabled, spx_int32_t *frame_size, int *granule_frame_size, spx_int32_t *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo, int *extra_headers, int quiet) {
    void *st;
    const SpeexMode *mode;
    SpeexHeader *header;
    int modeID;
    SpeexCallback callback;

    header = speex_packet_to_header((char*)op->packet, op->bytes);
    if (!header) {
        return NULL;
    }
    if (header->mode >= SPEEX_NB_MODES || header->mode<0) {
        free(header);
        return NULL;
    }

    modeID = header->mode;
    if (forceMode!=-1) {
        modeID = forceMode;
    }

    mode = speex_lib_get_mode (modeID);

    if (header->speex_version_id > 1) {
        free(header);
        return NULL;
    }

    if (mode->bitstream_version < header->mode_bitstream_version) {
        free(header);
        return NULL;
    }
    if (mode->bitstream_version > header->mode_bitstream_version) {
        free(header);
        return NULL;
    }

    st = speex_decoder_init(mode);
    if (!st) {
        free(header);
        return NULL;
    }
    speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
    speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);
    *granule_frame_size = *frame_size;

    if (!(*channels==1)) {
        callback.callback_id = SPEEX_INBAND_STEREO;
        callback.func = speex_std_stereo_request_handler;
        callback.data = stereo;
        speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
    }
    if (!*rate) {
        *rate = header->rate;
    }
    /* Adjust rate if --force-* options are used */
    if (forceMode!=-1) {
        if (header->mode < forceMode) {
            *rate <<= (forceMode - header->mode);
            *granule_frame_size >>= (forceMode - header->mode);
        }
        if (header->mode > forceMode) {
            *rate >>= (header->mode - forceMode);
            *granule_frame_size <<= (header->mode - forceMode);
        }
    }


    speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);

    *nframes = header->frames_per_packet;

    if (*channels==-1) {
        *channels = header->nb_channels;
    }

    *extra_headers = header->extra_headers;

    free(header);
    return st;
}
#endif
