#include "PNG.h"

namespace Util {
    namespace Video {
        PNG::PNG(std::wstring const & filename) {
#if 0
            FILE *fp = fopen(Text::StringOp::ToUTF8(filename).c_str(), "rb");
            if (!fp) {
                return (ERROR);
            }
            fread(header, 1, number, fp);
            bool is_png = !png_sig_cmp(header, 0, number);
            if (!is_png) {
                return (NOT_PNG);
            }
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, //1.4.0
                (png_voidp)user_error_ptr, user_error_fn, user_warning_fn);
            if (!png_ptr)
                return (ERROR);

            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
                return (ERROR);
            }

            png_infop end_info = png_create_info_struct(png_ptr);
            if (!end_info) {
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
                return (ERROR);
            }

            if (setjmp(png_ptr->jmpbuf)) {
                /* Free all of the memory associated with the png_ptr and info_ptr */
                delete [] row_pointers;
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
                cx_throw("");
            }

            // use custom I/O functions
            png_set_read_fn(png_ptr, hFile, /*(png_rw_ptr)*/user_read_data);
            png_set_error_fn(png_ptr,info.szLastError,/*(png_error_ptr)*/user_error_fn,NULL);

            /* read the file information */
            png_read_info(png_ptr, info_ptr);

            if (info.nEscape == -1){
                head.biWidth = info_ptr->width;
                head.biHeight= info_ptr->height;
                info.dwType = CXIMAGE_FORMAT_PNG;
                longjmp(png_ptr->jmpbuf, 1);
            }

            /* calculate new number of channels */
            int channels=0;
            switch(info_ptr->color_type){
            case PNG_COLOR_TYPE_GRAY:
            case PNG_COLOR_TYPE_PALETTE:
                channels = 1;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                channels = 2;
                break;
            case PNG_COLOR_TYPE_RGB:
                channels = 3;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                channels = 4;
                break;
            default:
                strcpy(info.szLastError,"unknown PNG color type");
                longjmp(png_ptr->jmpbuf, 1);
            }

            //find the right pixel depth used for cximage
            int pixel_depth = info_ptr->pixel_depth;
            if (channels == 1 && pixel_depth>8) pixel_depth=8;
            if (channels == 2) pixel_depth=8;
            if (channels >= 3) pixel_depth=24;

            png_init_io(png_ptr, fp);
            png_set_sig_bytes(png_ptr, number);
            png_set_read_user_chunk_fn(png_ptr, user_chunk_ptr, read_chunk_callback);
            png_get_user_chunk_ptr(png_ptr);
            png_set_read_status_fn(png_ptr, read_row_callback);

            //png_read_png(png_ptr, info_ptr, png_transforms, NULL);
            //row_pointers = png_get_rows(png_ptr, info_ptr); //png_bytep row_pointers[height];
            if (height > PNG_UINT_32_MAX/png_sizeof(png_byte))
                png_error (png_ptr, "Image is too tall to process in memory");
            if (width > PNG_UINT_32_MAX/pixel_size)
                png_error (png_ptr, "Image is too wide to process in memory");
            row_pointers = png_malloc(png_ptr, height*png_sizeof(png_bytep));
            for (int i=0; i<height, i++)
                row_pointers[i]=NULL;  /* security precaution */
            for (int i=0; i<height, i++)
                row_pointers[i]=png_malloc(png_ptr, width*pixel_size);
            png_set_rows(png_ptr, info_ptr, &row_pointers);

            //png_read_info(png_ptr, info_ptr);
            //followed the set of transformations indicated by the transform mask;
            //png_read_image();
            //png_read_end();
#endif
        }

#if 0
        read_chunk_callback(png_ptr ptr, png_unknown_chunkp chunk) {
           /* The unknown chunk structure contains your
              chunk data, along with similar data for any other
              unknown chunks: */

               png_byte name[5];
               png_byte *data;
               png_size_t size;

           /* Note that libpng has already taken care of
              the CRC handling */

           /* put your code here.  Search for your chunk in the
              unknown chunk structure, process it, and return one
              of the following: */

           return (-n); /* chunk had an error */
           return (0); /* did not recognize */
           return (n); /* success */
        }

        void read_row_callback(png_ptr ptr, png_uint_32 row, int pass) {
          /* put your code here */
        }
#endif

#if 0
png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
    keep       - 0: default unknown chunk handling
                 1: ignore; do not keep
                 2: keep only if safe-to-copy
                 3: keep even if unsafe-to-copy
               You can use these definitions:
                 PNG_HANDLE_CHUNK_AS_DEFAULT   0
                 PNG_HANDLE_CHUNK_NEVER        1
                 PNG_HANDLE_CHUNK_IF_SAFE      2
                 PNG_HANDLE_CHUNK_ALWAYS       3
    chunk_list - list of chunks affected (a byte string,
                 five bytes per chunk, NULL or '\0' if
                 num_chunks is 0)
    num_chunks - number of chunks affected; if 0, all
                 unknown chunks are affected.  If nonzero,
                 only the chunks in the list are affected

    png_get_IHDR(png_ptr, info_ptr, &width, &height,
       &bit_depth, &color_type, &interlace_type,
       &compression_type, &filter_method);

    width          - holds the width of the image
                     in pixels (up to 2^31).
    height         - holds the height of the image
                     in pixels (up to 2^31).
    bit_depth      - holds the bit depth of one of the
                     image channels.  (valid values are
                     1, 2, 4, 8, 16 and depend also on
                     the color_type.  See also
                     significant bits (sBIT) below).
    color_type     - describes which color/alpha channels
                         are present.
                     PNG_COLOR_TYPE_GRAY
                        (bit depths 1, 2, 4, 8, 16)
                     PNG_COLOR_TYPE_GRAY_ALPHA
                        (bit depths 8, 16)
                     PNG_COLOR_TYPE_PALETTE
                        (bit depths 1, 2, 4, 8)
                     PNG_COLOR_TYPE_RGB
                        (bit_depths 8, 16)
                     PNG_COLOR_TYPE_RGB_ALPHA
                        (bit_depths 8, 16)

                     PNG_COLOR_MASK_PALETTE
                     PNG_COLOR_MASK_COLOR
                     PNG_COLOR_MASK_ALPHA

    filter_method  - (must be PNG_FILTER_TYPE_BASE
                     for PNG 1.0, and can also be
                     PNG_INTRAPIXEL_DIFFERENCING if
                     the PNG datastream is embedded in
                     a MNG-1.0 datastream)
    compression_type - (must be PNG_COMPRESSION_TYPE_BASE
                     for PNG 1.0)
    interlace_type - (PNG_INTERLACE_NONE or
                     PNG_INTERLACE_ADAM7)



    width            = png_get_image_width(png_ptr, info_ptr);
    height           = png_get_image_height(png_ptr, info_ptr);
    bit_depth        = png_get_bit_depth(png_ptr, info_ptr);
    color_type       = png_get_color_type(png_ptr, info_ptr);
    filter_method    = png_get_filter_type(png_ptr, info_ptr);
    compression_type = png_get_compression_type(png_ptr, info_ptr);
    interlace_type   = png_get_interlace_type(png_ptr, info_ptr);

    channels = png_get_channels(png_ptr, info_ptr);
    channels       - number of channels of info for the
                     color type (valid values are 1 (GRAY,
                     PALETTE), 2 (GRAY_ALPHA), 3 (RGB),
                     4 (RGB_ALPHA or RGB + filler byte))
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    rowbytes       - number of bytes needed to hold a row

    signature = png_get_signature(png_ptr, info_ptr);
    signature      - holds the signature read from the
                     file (if any).  The data is kept in
                     the same offset it would be if the
                     whole signature were read (i.e. if an
                     application had already read in 4
                     bytes of signature before starting
                     libpng, the remaining 4 bytes would
                     be in signature[4] through signature[7]
                     (see png_set_sig_bytes())).



    png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    palette        - the palette for the file (array of png_color)
    num_palette    - number of entries in the palette

    png_get_gAMA(png_ptr, info_ptr, &gamma);
    gamma          - the gamma the file is written at (PNG_INFO_gAMA)

    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);
    srgb_intent    - the rendering intent (PNG_INFO_sRGB)
                     The presence of the sRGB chunk
                     means that the pixel data is in the
                     sRGB color space.  This chunk also
                     implies specific values of gAMA and
                     cHRM.

    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    name            - The profile name.
    compression     - The compression type; always
                      PNG_COMPRESSION_TYPE_BASE for PNG 1.0.
                      You may give NULL to this argument to
                      ignore it.
    profile         - International Color Consortium color
                      profile data. May contain NULs.
    proflen         - length of profile data in bytes.

    png_get_sBIT(png_ptr, info_ptr, &sig_bit);
    sig_bit        - the number of significant bits for
                     (PNG_INFO_sBIT) each of the gray,
                     red, green, and blue channels,
                     whichever are appropriate for the
                     given color type (png_color_16)

    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    trans_alpha    - array of alpha (transparency)
                     entries for palette (PNG_INFO_tRNS)
    trans_color    - graylevel or color sample values of
                     the single transparent color for
                     non-paletted images (PNG_INFO_tRNS)
    num_trans      - number of transparent entries
                     (PNG_INFO_tRNS)

    png_get_hIST(png_ptr, info_ptr, &hist);
                     (PNG_INFO_hIST)
    hist           - histogram of palette (array of
                     png_uint_16)

    png_get_tIME(png_ptr, info_ptr, &mod_time);
    mod_time       - time image was last modified
                    (PNG_VALID_tIME)

    png_get_bKGD(png_ptr, info_ptr, &background);
    background     - background color (PNG_VALID_bKGD)
                     valid 16-bit red, green and blue
                     values, regardless of color_type

    num_comments   = png_get_text(png_ptr, info_ptr,
                     &text_ptr, &num_text);
    num_comments   - number of comments
    text_ptr       - array of png_text holding image
                     comments
    text_ptr[i].compression - type of compression used
                 on "text" PNG_TEXT_COMPRESSION_NONE
                           PNG_TEXT_COMPRESSION_zTXt
                           PNG_ITXT_COMPRESSION_NONE
                           PNG_ITXT_COMPRESSION_zTXt
    text_ptr[i].key   - keyword for comment.  Must contain
                         1-79 characters.
    text_ptr[i].text  - text comments for current
                         keyword.  Can be empty.
    text_ptr[i].text_length - length of text string,
                 after decompression, 0 for iTXt
    text_ptr[i].itxt_length - length of itxt string,
                 after decompression, 0 for tEXt/zTXt
    text_ptr[i].lang  - language of comment (empty
                         string for unknown).
    text_ptr[i].lang_key  - keyword in UTF-8
                         (empty string for unknown).
    Note that the itxt_length, lang, and lang_key
    members of the text_ptr structure only exist
    when the library is built with iTXt chunk support.

    num_text       - number of comments (same as
                     num_comments; you can put NULL here
                     to avoid the duplication)
    Note while png_set_text() will accept text, language,
    and translated keywords that can be NULL pointers, the
    structure returned by png_get_text will always contain
    regular zero-terminated C strings.  They might be
    empty strings but they will never be NULL pointers.

    num_spalettes = png_get_sPLT(png_ptr, info_ptr, &palette_ptr);
    palette_ptr    - array of palette structures holding
                     contents of one or more sPLT chunks
                     read.
    num_spalettes  - number of sPLT chunks read.

    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    offset_x       - positive offset from the left edge
                     of the screen
    offset_y       - positive offset from the top edge
                     of the screen
    unit_type      - PNG_OFFSET_PIXEL, PNG_OFFSET_MICROMETER

    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    res_x          - pixels/unit physical resolution in
                     x direction
    res_y          - pixels/unit physical resolution in
                     x direction
    unit_type      - PNG_RESOLUTION_UNKNOWN,
                     PNG_RESOLUTION_METER

    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height)
    unit        - physical scale units (an integer)
    width       - width of a pixel in physical scale units
    height      - height of a pixel in physical scale units
                 (width and height are doubles)

    png_get_sCAL_s(png_ptr, info_ptr, &unit, &width, &height)
    unit        - physical scale units (an integer)
    width       - width of a pixel in physical scale units
    height      - height of a pixel in physical scale units
                 (width and height are strings like "2.54")

    num_unknown_chunks = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns)
    unknowns          - array of png_unknown_chunk
                        structures holding unknown chunks
    unknowns[i].name  - name of unknown chunk
    unknowns[i].data  - data of unknown chunk
    unknowns[i].size  - size of unknown chunk's data
    unknowns[i].location - position of chunk in file

    The value of "i" corresponds to the order in which the
    chunks were read from the PNG file or inserted with the
    png_set_unknown_chunks() function.


    res_x = png_get_x_pixels_per_meter(png_ptr, info_ptr)
    res_y = png_get_y_pixels_per_meter(png_ptr, info_ptr)
    res_x_and_y = png_get_pixels_per_meter(png_ptr, info_ptr)
    res_x = png_get_x_pixels_per_inch(png_ptr, info_ptr)
    res_y = png_get_y_pixels_per_inch(png_ptr, info_ptr)
    res_x_and_y = png_get_pixels_per_inch(png_ptr, info_ptr)
    aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr)

   (Each of these returns 0 [signifying "unknown"] if
       the data is not present or if res_x is 0;
       res_x_and_y is 0 if res_x != res_y)



    x_offset = png_get_x_offset_microns(png_ptr, info_ptr);
    y_offset = png_get_y_offset_microns(png_ptr, info_ptr);
    x_offset = png_get_x_offset_inches(png_ptr, info_ptr);
    y_offset = png_get_y_offset_inches(png_ptr, info_ptr);

   (Each of these returns 0 [signifying "unknown" if both
       x and y are 0] if the data is not present or if the
       chunk is present but the unit is the pixel)
#endif
        PNG::~PNG() {
        }
    }
}
