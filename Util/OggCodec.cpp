#include "OggCodec.h"
#include <string.h>

namespace Util
{
    namespace Audio
    {
        OggCodec::OggCodec() {
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
            agc_enabled_enc = 0;
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
            InitializeCriticalSection(&csDecodeFile);
        }

        OggCodec::~OggCodec() {
            StopEncode(true);
            StopDecode(true);
        }

        unsigned long OggCodec::Encode() {
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
            _snprintf_s(vendor_string, 64, sizeof(vendor_string), "Encoded with Speex %s", speex_version_enc);
            comment_init(&comments_enc, &comments_length_enc, vendor_string, sizeof(vendor_string));
            comment_add(&comments_enc, &comments_length_enc, NULL, comments, sizeof(comments)); 

            //srand(time(NULL));
            if (ogg_stream_init(&os_enc, rand()) == -1) {
                return 0;
            }

            wave_input_enc = 1;
            lsb_enc = 1;

            rate_enc = SAMPLESPERSEC;
            chan_enc = CHANNELS;
            fmt_enc = BITSPERSAMPLE;

            if (rate_enc > 48000) {
                return 0;
            } else if (rate_enc > 25000) {
                modeID_enc = SPEEX_MODEID_UWB;
            } else if (rate_enc > 12500) {
                modeID_enc = SPEEX_MODEID_WB;
            } else if (rate_enc >= 6000) {
                modeID_enc = SPEEX_MODEID_NB;
            } else {
                return 0;
            }

            if (!mode_enc) {
                mode_enc = speex_lib_get_mode(modeID_enc);
            }
            speex_init_header(&header_enc, rate_enc, 1, mode_enc);
            header_enc.frames_per_packet = nframes_enc;
            header_enc.vbr = vbr_enabled_enc;
            header_enc.nb_channels = chan_enc;

            st_enc = speex_encoder_init(mode_enc);

            outFile_enc = (char*)sEncFileName.c_str();
            //fout_enc = fopen(outFile_enc, "wb"); //for save every times
            //if (!fout_enc) {
            //  return 0;
            //}
            close_out_enc = 1;
            speex_encoder_ctl(st_enc, SPEEX_GET_FRAME_SIZE, &frame_size_enc);
            speex_encoder_ctl(st_enc, SPEEX_SET_COMPLEXITY, &complexity_enc);
            speex_encoder_ctl(st_enc, SPEEX_SET_SAMPLING_RATE, &rate_enc);
            speex_encoder_ctl(st_enc, SPEEX_SET_QUALITY, &quality_enc);
            speex_encoder_ctl(st_enc, SPEEX_GET_LOOKAHEAD, &lookahead_enc);

            //preprocess_enc = speex_preprocess_state_init(frame_size_enc, rate_enc);
            //speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_DENOISE, &denoise_enabled_enc);
            //speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_AGC, &agc_enabled_enc);//
            //speex_preprocess_ctl(preprocess_enc, SPEEX_PREPROCESS_SET_VAD, &denoise_enabled_enc);
            //lookahead_enc += frame_size_enc;

            int packet_size;
            op_enc.packet = (unsigned char*)speex_header_to_packet(&header_enc, &packet_size);
            op_enc.bytes = packet_size;
            op_enc.b_o_s = 1;
            op_enc.e_o_s = 0;
            op_enc.granulepos = 0;
            op_enc.packetno = 0;
            ogg_stream_packetin(&os_enc, &op_enc);
            free(op_enc.packet);

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

            op_enc.packet = (unsigned char*)comments_enc;
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
                    speex_encode_int(st_enc, (spx_int16_t *)input, &bits_enc);
                    if ((id_enc + 1) % nframes_enc != 0) {
                        continue;
                    }

                    speex_bits_insert_terminator(&bits_enc);
                    nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
                    speex_bits_reset(&bits_enc);
                    op_enc.packet = (unsigned char*)cbits;
                    op_enc.bytes = nbBytes_enc;
                    op_enc.b_o_s = 0;

                    if (OggQueue.empty() && bOggEncStop && (size_enc <= 0)) {
                        op_enc.e_o_s = 1;
                    } else {
                        op_enc.e_o_s = 0;
                    }
                    op_enc.granulepos = (id_enc + 1) * frame_size_enc - lookahead_enc;
                    op_enc.packetno = 2 + id_enc / nframes_enc;
                    ogg_stream_packetin(&os_enc, &op_enc);
                    while (ogg_stream_pageout(&os_enc, &og_enc)) {
                        ret_enc = oe_write_page(&og_enc, fout_enc);
                        if (ret_enc != og_enc.header_len + og_enc.body_len) {
                            return 0;
                        } else {
                            bytes_written_enc += ret_enc;
                        }
                    }
                }
            }

            if ((id_enc + 1) % nframes_enc != 0) {
                while ((id_enc + 1) % nframes_enc != 0) {
                    ++id_enc;
                    speex_bits_pack(&bits_enc, 15, 5);
                }
                nbBytes_enc = speex_bits_write(&bits_enc, cbits, MAX_FRAME_BYTES);
                op_enc.packet = (unsigned char*)cbits;
                op_enc.bytes = nbBytes_enc;
                op_enc.b_o_s = 0;
                op_enc.e_o_s = 1;
                op_enc.granulepos = (id_enc + 1) * frame_size_enc - lookahead_enc;
                if (op_enc.granulepos > total_samples_enc) {
                    op_enc.granulepos = total_samples_enc;
                }

                op_enc.packetno = 2 + id_enc / nframes_enc;
                ogg_stream_packetin(&os_enc, &op_enc);
            }

            while (ogg_stream_flush(&os_enc, &og_enc)) {
                ret_enc = oe_write_page(&og_enc, fout_enc);
                if (ret_enc != og_enc.header_len + og_enc.body_len) {
                    return 0;
                } else {
                    bytes_written_enc += ret_enc;
                }
            }

            speex_encoder_destroy(st_enc);
            speex_bits_destroy(&bits_enc);
            ogg_stream_clear(&os_enc);

            //speex_preprocess_state_destroy(preprocess_enc);

            //if (close_out_enc)       //no need
            //  fclose(fout_enc);

            if (!OggQueue.empty()) {
                for (size_t i = 0; i < OggQueue.size(); ++i) {
                    PcmData* m = OggQueue.front();
                    free(m->data);
                    delete m;
                    OggQueue.pop();
                }
            }
            nb_samples_enc = 0;
            total_samples_enc = 0;
            nb_encoded_enc = 0;
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
            size_enc;
            chan_enc = 1;
            fmt_enc = 16;
            quality_enc = 8;
            vbr_quality_enc = -1;
            lsb_enc = 1;
            bytes_written_enc = 0;
            ret_enc = 0;
            result_enc = 0;
            id_enc = -1;
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
            agc_enabled_enc = 0;
            lookahead_enc = 0;

            OggBufferLenght = 0;
            OggBufferPosition = 0;
            //CloseHandle(hOggEncThread);
            //hOggEncThread = NULL;
            return 0;
        }

        unsigned long OggCodec::Decode() {
            inFile_dec = (char*)sDecFileName.c_str();
            fin_dec = 0;
            fopen_s(&fin_dec, inFile_dec, "rb");
            if (!fin_dec) {
                return 0;
            }
            fseek(fin_dec, 0, SEEK_END);
            file_size = ftell(fin_dec);
            fseek(fin_dec, 0, SEEK_SET);

            close_in_dec = 1;
            ogg_sync_init(&oy_dec); 
            speex_bits_init(&bits_dec);

            while (!bOggDecStop) {
                char* data;
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
                while (ogg_sync_pageout(&oy_dec, &og_dec) == 1) {
                    if (bOggDecStop) {
                        break;
                    }
                    if (m_bDecodeSeek) {
                        //EnterCriticalSection(&csDecodeFile);
                        //m_bDecodeSeek = false;
                        //LeaveCriticalSection(&csDecodeFile);
                        //ogg_sync_reset(&oy_dec);
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
                    if (page_granule_dec > 0 && frame_size_dec) {
                        skip_samples_dec = static_cast<int>(frame_size_dec * (page_nb_packets_dec * granule_frame_size_dec * nframes_dec - (page_granule_dec - last_granule_dec)) / granule_frame_size_dec);
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
                            //EnterCriticalSection(&csDecodeFile);
                            //m_bDecodeSeek = false;
                            //LeaveCriticalSection(&csDecodeFile);
                            //ogg_sync_reset(&oy_dec);
                            break;
                        }

                        if (!memcmp(op_dec.packet, "Speex", 5)) {
                            speex_serialno_dec = os_dec.serialno;
                        }
                        if (speex_serialno_dec == -1 || os_dec.serialno != speex_serialno_dec) {
                            break;
                        }
                        if (packet_count_dec == 0) {
                            st_dec = process_header(&op_dec, enh_enabled_dec, (spx_int32_t *)&frame_size_dec, &granule_frame_size_dec, (spx_int32_t *)&rate_dec, &nframes_dec, forceMode_dec, &channels_dec, &stereo_dec, &extra_headers_dec, 0);
                            if (!st_dec) {
                                exit(1);
                            }
                            speex_decoder_ctl(st_dec, SPEEX_GET_LOOKAHEAD, &lookahead_dec);
                            if (!nframes_dec) {
                                nframes_dec = 1;
                            }
                            out_file_open(NULL, rate_dec, &channels_dec);
                        } else if (packet_count_dec == 1) {
                              //
                        } else if (packet_count_dec <= 1 + extra_headers_dec) {
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
                            for (j = 0; j != nframes_dec; ++j) {
                                if (bOggDecStop) {
                                    break;
                                }
                                if (m_bDecodeSeek) {
                                    //EnterCriticalSection(&csDecodeFile);
                                    //m_bDecodeSeek = false;
                                    //LeaveCriticalSection(&csDecodeFile);
                                    //ogg_sync_reset(&oy_dec);
                                    break;
                                }

                                int ret;
                                if (!lost) {
                                    ret = speex_decode_int(st_dec, &bits_dec, (spx_int16_t *)output);
                                } else {
                                      ret = speex_decode_int(st_dec, NULL, (spx_int16_t *)output);
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
                                    speex_decode_stereo_int((spx_int16_t *)output, frame_size_dec, &stereo_dec);
                                }

                                memcpy(out, output, frame_size_dec * channels_dec * sizeof(short));

                                int frame_offset = 0;
                                int new_frame_size = frame_size_dec;

                                if (packet_no == 1 && j == 0 && skip_samples_dec > 0) {
                                    new_frame_size -= skip_samples_dec + lookahead_dec;
                                    frame_offset = skip_samples_dec + lookahead_dec;
                                }
                                if (packet_no == page_nb_packets_dec && skip_samples_dec < 0) {
                                    int packet_length = nframes_dec* frame_size_dec + skip_samples_dec + lookahead_dec;
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

            WIN_Audio_close();

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
            //CloseHandle(hOggDecThread);
            //hOggDecThread = NULL;
            return 0;
        }

        void Util::Audio::OggCodec::PushPcmData(PcmData* pd) {
            OggQueue.push(pd);
        }

        void Util::Audio::OggCodec::SetQuality(unsigned int level) {
            if (level >= 0 && level <= 10) {
                quality_enc = level;
            }
        }

        bool Util::Audio::OggCodec::StartEncode(std::string encodeFileName, std::string comment) {
            sEncFileName = encodeFileName;
            memset(comments, 0, sizeof(comments));
            if (!comment.empty()) {
                int len = comment.size() < sizeof(comments) ? comment.size() : sizeof(comments) - 1;
                memcpy(comments, comment.c_str(), len);
            }

            bOggEncStop = false;
            hOggEncThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EncodeProcess, this, 0, NULL);
            if (hOggEncThread == INVALID_HANDLE_VALUE) {
                hOggEncThread = NULL;
                bOggEncStop = true;
                return false;
            }
            return true;
        }

        void Util::Audio::OggCodec::StopEncode(bool sync) {
            if (bOggEncStop) {
                return;
            }
            bOggEncStop = true;
            if (sync) {
                //while (hOggEncThread != NULL) {
                //  Sleep(10);
                //}
                WaitForSingleObject(hOggEncThread, 5000);
                CloseHandle(hOggEncThread);
                hOggEncThread = NULL;
            }
        }

        bool Util::Audio::OggCodec::StartDecode(std::string decodeFileName) {
            sDecFileName = decodeFileName;
            bOggDecStop = false;
            hOggDecThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DecodeProcess, this, 0, NULL);
            if (hOggDecThread == INVALID_HANDLE_VALUE) {
                hOggDecThread = NULL;
                bOggDecStop = true;
                return false;
            }
            return true;
        }

        void Util::Audio::OggCodec::StopDecode(bool sync) {
            if (m_bDecodePause) {
                PauseDecode(false);
            }

            if (bOggDecStop) {
                return;
            }
            bOggDecStop = true;

            if (sync) {
                //while (hOggDecThread != NULL) {
                //  Sleep(10);
                //}
                WaitForSingleObject(hOggDecThread, 5000);
                CloseHandle(hOggDecThread);
                hOggDecThread = NULL;
            }
        }

        void Util::Audio::OggCodec::PauseDecode(bool pause) {
            m_bDecodePause = pause;
            if (m_bDecodePause) {
                SuspendThread(hOggDecThread);
            } else {
                ResumeThread(hOggDecThread);
            }
        }

        bool Util::Audio::OggCodec::DecodeSeek(unsigned int second) {
            bool b = false;
            if (!bOggDecStop && (fin_dec != NULL) && (file_size > 0)) {
                int pos = 276;
                int step = 4107;//2 quality page length
                if (sDecFileName.find("HQ") != std::string::npos) {
                    step = 4239;//8 quality page length //4  4332
                    pos += (second / 2) * step;
                } else/* if (sDecFileName.find("ST") != std::string::npos)*/ {
                    step = 4107;//2 quality page length
                    pos += (second / 5) * step;
                }

                EnterCriticalSection(&csDecodeFile);
                if (fseek(fin_dec, pos, SEEK_SET) == 0) {
                    m_bDecodeSeek = true;
                    b = true;
                }
                LeaveCriticalSection(&csDecodeFile);
            }
            return b;
        }

        unsigned long __stdcall Util::Audio::OggCodec::EncodeProcess(void* param) {
            return ((Util::Audio::OggCodec *)param)->Encode();
        }

        unsigned long __stdcall Util::Audio::OggCodec::DecodeProcess(void* param) {
            return ((Util::Audio::OggCodec *)param)->Decode();
        }

        void Util::Audio::OggCodec::comment_init(char** comments, int* length, char* vendor_string, int vendor_string_length) {
            int vendor_length = vendor_string_length;//strlen(vendor_string);
            int user_comment_list_length = 0;
            int len = 4 + vendor_length + 4;
            char* p = (char*)malloc(len);
            if (p == NULL) {
                return;
            }
            writeint(p, 0, vendor_length);
            memcpy(p + 4, vendor_string, vendor_length);
            writeint(p, 4 + vendor_length, user_comment_list_length);
            *length = len;
            *comments = p;
        }

        void Util::Audio::OggCodec::comment_add(char** comments, int* length, char* tag, char* val, int val_length) {
            char* p = *comments;
            int vendor_length = readint(p, 0);
            int user_comment_list_length = readint(p, 4 + vendor_length);
            int tag_len = (tag ? strlen(tag) : 0);
            int val_len = val_length;//strlen(val);
            int len = (*length) + 4 + tag_len + val_len;

            p = (char*)realloc(p, len);
            if (p == NULL) {
                return;
            }

            writeint(p, *length, tag_len + val_len);      /* length of comment */
            if (tag) {
                memcpy(p + *length + 4, tag, tag_len);  /* comment */
            }
            memcpy(p + *length + 4 + tag_len, val, val_len);  /* comment */
            writeint(p, 4 + vendor_length, user_comment_list_length + 1);

            *comments = p;
            *length = len;
        }

        int Util::Audio::OggCodec::oe_write_page(ogg_page* page, FILE* fp) {
            int written = 0;
            outFile_enc = (char*)sEncFileName.c_str();
            fp = 0;
            fopen_s(&fp, outFile_enc, "a+b");
            if (fp) {
                written = fwrite(page->header, 1, page->header_len, fp);
                written += fwrite(page->body, 1, page->body_len, fp);
                fflush(fp);
                fclose(fp);
            }
            return written;
        }

        //-----------------------------------------------------------------------------------------------------------//

        FILE* Util::Audio::OggCodec::out_file_open(char* outFile, int rate, int* channels) {
            FILE* fout = NULL;
            /*Open output file*/
            if (outFile == NULL || strlen(outFile) == 0) {
                unsigned int speex_channels = *channels;
                if (Set_WIN_Params(INVALID_FILEDESC, rate, SAMPLE_SIZE, speex_channels)) {
                    return NULL;
                }
            } else {
                  fout = 0;
                  fopen_s(&fout, outFile, "wb");
                if (fout) {
                    if (strcmp(outFile + strlen(outFile) - 4, ".wav") == 0 || strcmp(outFile + strlen(outFile) - 4, ".WAV") == 0) {
                        write_wav_header(fout, rate, *channels, 0, 0);
                    }
                }
            }

            return fout;
        }


        void* Util::Audio::OggCodec::process_header(ogg_packet* op, spx_int32_t enh_enabled, spx_int32_t* frame_size, int* granule_frame_size, spx_int32_t* rate, int* nframes, int forceMode, int* channels, SpeexStereoState* stereo, int* extra_headers, int quiet) {
            void* st;
            const SpeexMode* mode;
            SpeexHeader* header;
            int modeID;
            SpeexCallback callback;

            header = speex_packet_to_header((char*)op->packet, op->bytes);
            if (!header) {
                return NULL;
            }
            if (header->mode >= SPEEX_NB_MODES || header->mode < 0) {
                free(header);
                return NULL;
            }

            modeID = header->mode;
            if (forceMode != -1) {
                modeID = forceMode;
            }

            mode = speex_lib_get_mode(modeID);

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

            if (!(*channels == 1)) {
                callback.callback_id = SPEEX_INBAND_STEREO;
                callback.func = speex_std_stereo_request_handler;
                callback.data = stereo;
                speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
            }
            if (!*rate) {
                *rate = header->rate;
            }
            /* Adjust rate if --force-* options are used */
            if (forceMode != -1) {
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

            if (*channels == -1) {
                *channels = header->nb_channels;
            }

            *extra_headers = header->extra_headers;

            free(header);
            return st;
        }

        /* Set TABS = 4 */
        /********************************************************************
        function: To provide playback of 16 bit PCM wave data in Win32
        environments from decoded compressed files.
        ********************************************************************/

#if defined WIN32 || defined _WIN32

#define MAXWAVESIZE     4294967040LU
#define MAX_WAVEBLOCKS    32

        // This is modified for USE_WIN_AUDIO - ONLY 2002-02-27


        static CRITICAL_SECTION cs;
        static HWAVEOUT dev = NULL;
        static int ScheduledBlocks = 0;
        static int PlayedWaveHeadersCount = 0;          // free index
        static WAVEHDR* PlayedWaveHeaders[MAX_WAVEBLOCKS];

        static int Box(const wchar_t* msg) {
            MessageBox(NULL, msg, L" " VERSION_STRING L": Error Message . . .", MB_OK | MB_ICONEXCLAMATION);
            return -1;
        }


        /*
        *  This function registers already played WAVE chunks. Freeing is done by free_memory(),
        */

        static void CALLBACK wave_callback(HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
            if (uMsg == WOM_DONE) {
                EnterCriticalSection(&cs);
                PlayedWaveHeaders[PlayedWaveHeadersCount++] = (WAVEHDR *)dwParam1;
                LeaveCriticalSection(&cs);
            }
        }


        static void free_memory(void) {
            WAVEHDR* wh;
            HGLOBAL hg;

            EnterCriticalSection(&cs);
            wh = PlayedWaveHeaders[--PlayedWaveHeadersCount];
            ScheduledBlocks--;                               // decrease the number of USED blocks
            LeaveCriticalSection(&cs);

            waveOutUnprepareHeader(dev, wh, sizeof(WAVEHDR));

            hg = GlobalHandle(wh -> lpData);       // Deallocate the buffer memory
            GlobalUnlock(hg);
            GlobalFree(hg);

            hg = GlobalHandle(wh);                 // Deallocate the header memory
            GlobalUnlock(hg);
            GlobalFree(hg);
        }

        int Set_WIN_Params(FILE* dummyFile, long double  SampleFreq, unsigned int     BitsPerSample, unsigned int     Channels) {
            WAVEFORMATEX outFormat;
            UINT deviceID = WAVE_MAPPER;

            (void)dummyFile;

            if (waveOutGetNumDevs() == 0) {
                return Box(L"No audio device present.");
            }

            outFormat.wFormatTag = WAVE_FORMAT_PCM;
            outFormat.wBitsPerSample = BitsPerSample;
            outFormat.nChannels = Channels;
            outFormat.nSamplesPerSec = (unsigned long)(SampleFreq + 0.5);
            outFormat.nBlockAlign = (outFormat.wBitsPerSample + 7) / 8 * outFormat.nChannels;
            outFormat.nAvgBytesPerSec = outFormat.nSamplesPerSec * outFormat.nBlockAlign;

            switch (waveOutOpen(&dev, deviceID, &outFormat, (DWORD)wave_callback, 0, CALLBACK_FUNCTION)) {
            case MMSYSERR_ALLOCATED:
                return Box(L"Device is already open.");
            case MMSYSERR_BADDEVICEID:
                return Box(L"The specified device is out of range.");
            case MMSYSERR_NODRIVER:
                return Box(L"There is no audio driver in this system.");
            case MMSYSERR_NOMEM:
                return Box(L"Unable to allocate sound memory.");
            case WAVERR_BADFORMAT:
                return Box(L"This audio format is not supported.");
            case WAVERR_SYNC:
                return Box(L"The device is synchronous.");
            default:
                return Box(L"Unknown media error.");
            case MMSYSERR_NOERROR:
                break;
            }

            waveOutReset(dev);
            InitializeCriticalSection(&cs);
            //@@SetPriorityClass ( GetCurrentProcess (), HIGH_PRIORITY_CLASS );
            return 0;
        }


        int WIN_Play_Samples(const void* data, size_t len) {
            HGLOBAL hg;
            HGLOBAL hg2;
            LPWAVEHDR wh;
            void* allocptr;

            do {
                while (PlayedWaveHeadersCount > 0) {
                    // free used blocks ...
                    free_memory();
                }

                if (ScheduledBlocks < sizeof(PlayedWaveHeaders) / sizeof(*PlayedWaveHeaders)) {
                    // wait for a free block ...
                    break;
                }
                Sleep(26);
            } while (1);

            if ((hg2 = GlobalAlloc(GMEM_MOVEABLE, len)) == NULL) {
                // allocate some memory for a copy of the buffer
                return Box(L"GlobalAlloc failed.");
            }

            allocptr = GlobalLock(hg2);
            CopyMemory(allocptr, data, len);                         // Here we can call any modification output functions we want....

            if ((hg = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(WAVEHDR))) == NULL) {
                // now make a header and WRITE IT!
                return -1;
            }

            wh = (WAVEHDR *)GlobalLock(hg);
            wh -> dwBufferLength = len;
            wh -> lpData = (PSTR)allocptr;

            if (waveOutPrepareHeader(dev, wh, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
                GlobalUnlock(hg);
                GlobalFree(hg);
                return -1;
            }

            if (waveOutWrite(dev, wh, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
                GlobalUnlock(hg);
                GlobalFree(hg);
                return -1;
            }

            EnterCriticalSection(&cs);
            ScheduledBlocks++;
            LeaveCriticalSection(&cs);

            return len;
        }

        int WIN_Audio_close(void) {
            if (dev != NULL) {
                while (ScheduledBlocks > 0) {
                    Sleep(ScheduledBlocks);
                    while (PlayedWaveHeadersCount > 0) {
                        // free used blocks ...
                        free_memory();
                    }
                }

                waveOutReset(dev);      // reset the device
                waveOutClose(dev);      // close the device
                dev = NULL;
            }

            DeleteCriticalSection(&cs);
            ScheduledBlocks = 0;
            return 0;
        }

#endif

        /* end of wave_out.c */

        int read_wav_header(FILE* file, int* rate, int* channels, int* format, spx_int32_t* size) {
            char ch[5];
            spx_int32_t itmp;
            spx_int16_t stmp;
            spx_int32_t bpersec;
            spx_int16_t balign;
            int skip_bytes;
            int i;

            ch[4] = 0;
#if 0
            fread(ch, 1, 4, file);
            if (strcmp(ch, "RIFF")!=0) {
                fseek(file, 0, SEEK_SET);
                return 0;
            }

            fread(&itmp, 4, 1, file);
            *size = le_int(itmp-36);

            fread(ch, 1, 4, file);
            if (strcmp(ch, "WAVE")!=0) {
                fprintf (stderr, "RIFF file is not a WAVE file\n");
                return -1;
            }
#endif
            fread(ch, 1, 4, file);
            while (strcmp(ch, "fmt ") != 0) {
                fread(&itmp, 4, 1, file);
                itmp = le_int(itmp);
                /*fprintf (stderr, "skip=%d\n", itmp);*/
                /*strange way of seeking, but it works even for pipes*/
                for (i = 0; i < itmp; i++) {
                    fgetc(file);
                }
                /*fseek(file, itmp, SEEK_CUR);*/
                fread(ch, 1, 4, file);
                if (feof(file)) {
                    fprintf(stderr, "Corrupted WAVE file: no \"fmt \"\n");
                    return -1;
                }
            }
            /*
            if (strcmp(ch, "fmt ")!=0) {
            fprintf (stderr, "Corrupted WAVE file: no \"fmt \"\n");
            return -1;
            }
            */

            fread(&itmp, 4, 1, file);
            itmp = le_int(itmp);
            skip_bytes = itmp - 16;
            /*fprintf (stderr, "skip=%d\n", skip_bytes);*/

            fread(&stmp, 2, 1, file);
            stmp = le_short(stmp);
            if (stmp != 1) {
                fprintf(stderr, "Only PCM encoding is supported\n");
                return -1;
            }

            fread(&stmp, 2, 1, file);
            stmp = le_short(stmp);
            *channels = stmp;

            if (stmp > 2) {
                fprintf(stderr, "Only mono and (intensity) stereo supported\n");
                return -1;
            }

            fread(&itmp, 4, 1, file);
            itmp = le_int(itmp);
            *rate = itmp;
            if (*rate != 8000 && *rate != 16000 && *rate != 11025 && *rate != 22050 && *rate != 32000 && *rate != 44100 && *rate != 48000) {
                fprintf(stderr, "Only 8 kHz (narrowband) and 16 kHz (wideband) supported (plus 11.025 kHz and 22.05 kHz, but your mileage may vary)\n");
                return -1;
            }

            fread(&itmp, 4, 1, file);
            bpersec = le_int(itmp);

            fread(&stmp, 2, 1, file);
            balign = le_short(stmp);

            fread(&stmp, 2, 1, file);
            stmp = le_short(stmp);
            if (stmp != 16 && stmp != 8) {
                fprintf(stderr, "Only 8/16-bit linear supported\n");
                return -1;
            }
            *format = stmp;

            if (bpersec != *rate * *channels * stmp / 8) {
                fprintf(stderr, "Corrupted header: ByteRate mismatch\n");
                return -1;
            }

            if (balign != *channels * stmp / 8) {
                fprintf(stderr, "Corrupted header: BlockAlign mismatch\n");
                return -1;
            }


            /*strange way of seeking, but it works even for pipes*/
            if (skip_bytes > 0) {
                for (i = 0; i < skip_bytes; i++) {
                    fgetc(file);
                }
            }

            /*fseek(file, skip_bytes, SEEK_CUR);*/

            fread(ch, 1, 4, file);
            while (strcmp(ch, "data") != 0) {
                fread(&itmp, 4, 1, file);
                itmp = le_int(itmp);
                /*strange way of seeking, but it works even for pipes*/
                for (i = 0; i < itmp; i++) {
                    fgetc(file);
                }
                /*fseek(file, itmp, SEEK_CUR);*/
                fread(ch, 1, 4, file);
                if (feof(file)) {
                    fprintf(stderr, "Corrupted WAVE file: no \"data\"\n");
                    return -1;
                }
            }

            /*Ignore this for now*/
            fread(&itmp, 4, 1, file);
            itmp = le_int(itmp);

            *size = itmp;

            return 1;
        }

        void write_wav_header(FILE* file, int rate, int channels, int format, int size) {
            char ch[5];
            spx_int32_t itmp;
            spx_int16_t stmp;

            ch[4] = 0;

            fprintf(file, "RIFF");

            itmp = 0x7fffffff;
            fwrite(&itmp, 4, 1, file);

            fprintf(file, "WAVEfmt ");

            itmp = le_int(16);
            fwrite(&itmp, 4, 1, file);

            stmp = le_short(1);
            fwrite(&stmp, 2, 1, file);

            stmp = le_short(channels);
            fwrite(&stmp, 2, 1, file);

            itmp = le_int(rate);
            fwrite(&itmp, 4, 1, file);

            itmp = le_int(rate * channels * 2);
            fwrite(&itmp, 4, 1, file);

            stmp = le_short(2 * channels);
            fwrite(&stmp, 2, 1, file);

            stmp = le_short(16);
            fwrite(&stmp, 2, 1, file);

            fprintf(file, "data");

            itmp = le_int(0x7fffffff);
            fwrite(&itmp, 4, 1, file);
        }
    }
}
