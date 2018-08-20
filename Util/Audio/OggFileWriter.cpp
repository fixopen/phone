#include "OggFileWriter.h"
#include "../Text/StringOp.h"

#include "inc/speex/speex_header.h"

namespace Util {
    namespace Audio {
        OggFileWriter::OggFileWriter(std::wstring const& filename)
        : file_(0)
        , stream_(0)
        , packetNo_(0) {
            file_ = fopen(Util::Text::StringOp::ToUTF8(filename).c_str(), "a+b");
            //int ogg_stream_init(ogg_stream_state *os, int serialno);
            int r = ogg_stream_init(stream_, 0);
        }

        OggFileWriter::~OggFileWriter() {
            //int ogg_stream_clear(ogg_stream_state *os);
            int r = ogg_stream_clear(stream_);
            //int ogg_stream_destroy(ogg_stream_state *os);
            r = ogg_stream_destroy(stream_);
            fclose(file_);
        }

        void OggFileWriter::WriteHeader() {
            SpeexHeader speexHeader;
            //speex_init_header(&speexHeader, rate, channelsCount, mode);
            //speex_init_header(&header_enc, rate_enc, 1, mode_enc);
            speexHeader.frames_per_packet = -1;
            speexHeader.vbr = -1;
            speexHeader.nb_channels = -1;
            //speex_lib_get_mode();
            //st_enc = speex_encoder_init(mode_enc);
            //speex_encoder_ctl(st_enc, SPEEX_GET_FRAME_SIZE, &frame_size_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_COMPLEXITY, &complexity_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_SAMPLING_RATE, &rate_enc);
            //speex_encoder_ctl(st_enc, SPEEX_SET_QUALITY, &quality_enc);
            //speex_encoder_ctl(st_enc, SPEEX_GET_LOOKAHEAD, &lookahead_enc);
            int size = 0;
            ogg_packet packet;
            packet.packet = reinterpret_cast<unsigned char*>(speex_header_to_packet(&speexHeader, &size));
            packet.bytes = size;
            packet.b_o_s = 1;
            packet.e_o_s = 0;
            packet.granulepos = 0;
            packet.packetno = packetNo_;
            //int ogg_stream_packetin(ogg_stream_state *os, ogg_packet *op);
            int r = ogg_stream_packetin(stream_, &packet);
            ++packetNo_;
            //int ogg_stream_pageout(ogg_stream_state *os, ogg_page *og);
            //int ogg_stream_flush(ogg_stream_state *os, ogg_page *og);
            if (ogg_stream_pageout(stream_, &page_)) {
                fwrite(page_.header, 1, page_.header_len, file_);
                fwrite(page_.body, 1, page_.body_len, file_);
            }
        }

        void OggFileWriter::WriteComment() {
            op_enc.packet = (unsigned char *)comments_enc;
            op_enc.bytes = comments_length_enc;
            op_enc.b_o_s = 0;
            op_enc.e_o_s = 0;
            op_enc.granulepos = 0;
            op_enc.packetno = packetNo_;
            int r = ogg_stream_packetin(&os_enc, &op_enc);
            ++packetNo_;

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
        }

        void OggFileWriter::PushData(void* data, size_t const length) {
            ogg_packet packet;
            packet.packet = reinterpret_cast<unsigned char*>(data);
            packet.bytes = length;
            packet.b_o_s = 0;
            packet.e_o_s = 0;
            packet.granulepos = 0;
            packet.packetno = packetNo_;
            //int ogg_stream_packetin(ogg_stream_state *os, ogg_packet *op);
            ogg_stream_packetin(stream_, &packet);
            ++packetNo_;
            //int ogg_stream_pageout(ogg_stream_state *os, ogg_page *og);
            //int ogg_stream_flush(ogg_stream_state *os, ogg_page *og);
            if (ogg_stream_pageout(stream_, &page_)) {
                fwrite(page_.header, 1, page_.header_len, file_);
                fwrite(page_.body, 1, page_.body_len, file_);
            }
        }
    }
}
