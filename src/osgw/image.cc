#include <osgw/image.hh>

#include <png.h>

#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace osgw {
    Image::Image(const std::string& path) {
        png_size_t row_bytes;
        png_bytepp row_pointers;
        png_structp png_ptr;
        png_infop info_ptr;

        int color_type, interlace_type;

        unsigned char header_buffer[8];
        std::FILE* file_descriptor { std::fopen(path.c_str(), "rb") };
        if (!file_descriptor) goto close_descriptor;

        std::fread(header_buffer, 1, sizeof(header_buffer), file_descriptor);
        if (png_sig_cmp(header_buffer, 0, sizeof(header_buffer))) goto close_descriptor;

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) goto close_descriptor;

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) goto destroy_png_info;
        if (setjmp(png_jmpbuf(png_ptr))) goto destroy_png_info;

        png_init_io(png_ptr, file_descriptor);
        png_set_sig_bytes(png_ptr, sizeof(header_buffer));
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 |
                                        PNG_TRANSFORM_PACKING |
                                        PNG_TRANSFORM_EXPAND, nullptr);

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth_in_bits,
                     &color_type, &interlace_type, nullptr, nullptr);

        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) alpha_channel = true;
        else if (color_type == PNG_COLOR_TYPE_RGB) alpha_channel = false;
        else throw std::runtime_error { "Unknown PNG color format!" };

        row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        data = reinterpret_cast<unsigned char*>(std::malloc(row_bytes * height));
        row_pointers = png_get_rows(png_ptr, info_ptr);

        // Flip image to the format OpenGL knows.
        for (unsigned i { 0 }; i < height; ++i) {
            std::memcpy(data + (row_bytes) * (height - 1 - i),
                        row_pointers[i], row_bytes);
        }

destroy_png_info:
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
close_descriptor:
        std::fclose(file_descriptor);
    }

    bool Image::has_alpha() const {
        return alpha_channel;
    }

    unsigned Image::get_width() const {
        return width;
    }

    unsigned Image::get_height() const {
        return height;
    }

    unsigned Image::get_bit_depth() const {
        return depth_in_bits;
    }

    unsigned char* Image::get_data() const {
        return data;
    }
}
