#include "SpeexEncoder.h"

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
        SpeexEncoder::SpeexEncoder()
        : enc_state_(0)
        , file_(0) {
            //memset(version_, 0, 256);
            //speex_lib_ctl(SPEEX_LIB_GET_VERSION_STRING, (void*)version_);

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

        SpeexEncoder::~SpeexEncoder() {
        }

        void SpeexEncoder::InitEncode(SpeexMode const & mode, size_t const frameSize, size_t const quality) {
            //SpeexBits bits;
            speex_bits_init(&bits_);
            /*void **/enc_state_ = speex_encoder_init(&speex_nb_mode); //speex_wb_mode, speex_uwb_mode
            speex_encoder_ctl(enc_state_, SPEEX_GET_FRAME_SIZE, const_cast<size_t*>(&frameSize));
            speex_encoder_ctl(enc_state_, SPEEX_SET_QUALITY, const_cast<size_t*>(&quality)); //0-10
        }
        std::string const SpeexEncoder::Encode(spx_int16_t* in, size_t const length) {
            speex_bits_reset(&bits_);
            speex_encode_int(enc_state_, in, &bits_);
            size_t byteCount = speex_bits_nbytes(&bits_);
            char* byte_ptr = static_cast<char*>(malloc(byteCount));
            size_t nbBytes = speex_bits_write(&bits_, byte_ptr, byteCount);
            return std::string(byte_ptr, nbBytes);
            file_->PushData(byte_ptr, nbBytes);
        }

        void SpeexEncoder::UninitEncode() {
            speex_encoder_destroy(enc_state_);
            speex_bits_destroy(&bits_);
        }
    }
}
