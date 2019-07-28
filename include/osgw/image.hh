#ifndef OSGW_IMAGE_HH
#define OSGW_IMAGE_HH

#include <string>

namespace osgw {
    class Image final {
    public:
        Image(const std::string& path, bool flip_vertically);

        bool has_alpha() const;
        unsigned get_width() const;
        unsigned get_height() const;
        unsigned get_bit_depth() const;
        unsigned char* get_data() const;

    private:
        bool alpha_channel;
        unsigned width, height;
        int depth_in_bits;
        unsigned char* data;
    };
}

#endif
