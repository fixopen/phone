#include "OggFile.h"

#include "inc/speex/speex_header.h"
#include "inc/speex/speex.h"

namespace Util {
    namespace Audio {
        OggFile::OggFile(std::wstring const & filename, bool const isRead)
        : isRead_(isRead) {
            //int ogg_stream_init(ogg_stream_state *os, int serialno);
            int r = ogg_stream_init(os_, 0);
            if (isRead) {
                //int ogg_sync_init(ogg_sync_state *oy);
                r = ogg_sync_init(&sync_);
                //int ogg_sync_reset(ogg_sync_state *oy);
                r = ogg_sync_reset(&sync_);
                //int ogg_sync_clear(ogg_sync_state *oy);
                r = ogg_sync_clear(&sync_);
                //@@op init
                ogg_packet* op;
                SpeexHeader* header = speex_packet_to_header((char*)op->packet, op->bytes);
                if (!header) {
                    return;
                }
                if (header->mode >= SPEEX_NB_MODES || header->mode < 0) {
                    free(header);
                    return;
                }

                //@@forceMode init
                int forceMode;
                int modeId = header->mode;
                if (forceMode != -1) {
                    modeId = forceMode;
                }

                if (header->speex_version_id > 1) {
                    free(header);
                    return;
                }

                SpeexMode const * mode = speex_lib_get_mode(modeId);

                if (mode->bitstream_version < header->mode_bitstream_version) {
                    free(header);
                    return;
                }
                if (mode->bitstream_version > header->mode_bitstream_version) {
                    free(header);
                    return;
                }

                void* st = speex_decoder_init(mode);
                if (!st) {
                    free(header);
                    return;
                }
                spx_int32_t enh_enabled;
                speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
                spx_int32_t granule_frame_size = 0;
                speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, &granule_frame_size);

                //int* channels;
                //SpeexCallback callback;
                //SpeexStereoState* stereo;
                //if (!(*channels == 1)) {
                //    callback.callback_id = SPEEX_INBAND_STEREO;
                //    callback.func = speex_std_stereo_request_handler;
                //    callback.data = stereo;
                //    speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
                //}
                //spx_int32_t* rate;
                //if (!*rate) {
                //    *rate = header->rate;
                //}
                ///* Adjust rate if --force-* options are used */
                //if (forceMode != -1) {
                //    if (header->mode < forceMode) {
                //        *rate <<= (forceMode - header->mode);
                //        *granule_frame_size >>= (forceMode - header->mode);
                //    }
                //    if (header->mode > forceMode) {
                //        *rate >>= (header->mode - forceMode);
                //        *granule_frame_size <<= (header->mode - forceMode);
                //    }
                //}

                //speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);


                //int *nframes = header->frames_per_packet;

                //if (*channels==-1) {
                //    *channels = header->nb_channels;
                //}

                //*extra_headers = header->extra_headers;

                //free(header);
                //return;
                ////int *extra_headers;
                ////int quiet;
            }
        }

        OggFile::~OggFile() {
            if (isRead_) {
                //int ogg_sync_clear(ogg_sync_state *oy);
                int r = ogg_sync_clear(&sync_);
                //int ogg_sync_destroy(ogg_sync_state *oy);
                r = ogg_sync_destroy(&sync_);
            }
            //int ogg_stream_clear(ogg_stream_state *os);
            int r = ogg_stream_clear(os_);
            //int ogg_stream_destroy(ogg_stream_state *os);
            r = ogg_stream_destroy(os_);
        }

        void OggFile::PushData(void* data, size_t const length) {
            //speex_lib_get_mode();
            //speex_init_header(&header_enc, rate_enc, 1, mode_enc);
            //header_enc.frames_per_packet = nframes_enc;
            //header_enc.vbr = vbr_enabled_enc;
            //header_enc.nb_channels = chan_enc;
            //st_enc = speex_encoder_init(mode_enc);
            //outFile_enc = (char*)sEncFileName.c_str();
            //close_out_enc = 1;
            //speex_encoder_ctl(st_enc, SPEEX_GET_FRAME_SIZE, &frame_size_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_COMPLEXITY, &complexity_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_SAMPLING_RATE, &rate_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_QUALITY, &quality_enc);
            //speex_encoder_ctl(st_enc, SPEEX_GET_LOOKAHEAD, &lookahead_enc);
            //int packet_size;
            //op_enc.packet = (unsigned char *)speex_header_to_packet(&header_enc, &packet_size);
            //op_enc.bytes = packet_size;
            //op_enc.b_o_s = 1;
            //op_enc.e_o_s = 0;
            //op_enc.granulepos = 0;
            //op_enc.packetno = 0;
            SpeexHeader speexHeader;
            //speex_init_header(&speexHeader, rate, channelsCount, mode);
            speexHeader.frames_per_packet = -1;
            speexHeader.vbr = -1;
            speexHeader.nb_channels = -1;
            int size = 0;
            ogg_packet op;
            op.packet = reinterpret_cast<unsigned char*>(speex_header_to_packet(&speexHeader, &size));
            op.bytes = size;
            op.b_o_s = 1;
            op.e_o_s = 0;
            op.granulepos = 0;
            op.packetno = 0;
            //int ogg_stream_packetin(ogg_stream_state *os, ogg_packet *op);
            int r = ogg_stream_packetin(os_, &op);
            ogg_page og;
            //int ogg_stream_pageout(ogg_stream_state *os, ogg_page *og);
            //int ogg_stream_flush(ogg_stream_state *os, ogg_page *og);
            while (!ogg_stream_pageout(os_, &og)) {
                ogg_stream_packetin(os_, &op);
            }
        }

        std::string const OggFile::ReadData(size_t const length) {
            int r = 0;
            ogg_page og;
            //int ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
            while ((r = ogg_sync_pageout(&sync_, &og)) != 1) {
                //char *ogg_sync_buffer(ogg_sync_state *oy, long size);
                char* buffer = ogg_sync_buffer(&sync_, length);
                //int ogg_sync_wrote(ogg_sync_state *oy, long bytes);
                r = ogg_sync_wrote(&sync_, length);
            }
            //int ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
            r = ogg_stream_pagein(os_, &og);
            //int ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
            ogg_packet op;
            r = ogg_stream_packetout(os_, &op);
            return std::string(reinterpret_cast<char*>(op.packet), static_cast<size_t>(op.bytes));
            //int ogg_sync_check(ogg_sync_state *oy);
            //int ogg_sync_pageseek(ogg_sync_state *oy, ogg_page *og);
            //int ogg_stream_reset(ogg_stream_state *os);
            //int ogg_stream_clear(ogg_stream_state *os);
            //int ogg_stream_check(ogg_stream_state *os);
            //int ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op);

            //ogg_sync_init();
            //ogg_sync_reset();
            //ogg_sync_buffer();
            //ogg_sync_wrote();
            //ogg_sync_pageout();
            //ogg_stream_init();
            //ogg_page_serialno();
            //ogg_stream_reset_serialno();
            //ogg_stream_pagein();
            //ogg_page_granulepos();
            //ogg_page_packets();
            //ogg_page_eos();
            //ogg_stream_packetout();

            //speex_packet_to_header();
            //speex_lib_get_mode();
            //speex_decoder_init();
            //speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
            //speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);
            //speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
            //speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);
            //speex_decoder_ctl(st_dec, SPEEX_GET_LOOKAHEAD, &lookahead_dec);
            //speex_bits_remaining();
            //speex_decode_stereo_int();

            //ogg_stream_clear();
            //ogg_sync_clear();
        }
    }
}
