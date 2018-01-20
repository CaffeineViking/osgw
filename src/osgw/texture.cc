#include <osgw/texture.hh>

namespace osgw {
    Texture::Texture(const Image& image, const Parameters& parameters) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glEnable(GL_TEXTURE_2D); // AMD fix?

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLint format { image.has_alpha() ? GL_RGBA : GL_RGB };
        glTexImage2D(GL_TEXTURE_2D, 0, format, image.get_width(), image.get_height(), 0,
                     format, GL_UNSIGNED_BYTE, image.get_data());

        // Maybe it's the Gallium implementation but...
        // IDK, Mesa's mipmapping implementation is really shit.
        if (parameters.min_filter == MinFilter::NearestMipmap ||
            parameters.min_filter == MinFilter::LinearMipmap ||
            parameters.min_filter == MinFilter::SmoothMipmap) glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(parameters.mag_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(parameters.min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(parameters.wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(parameters.wrap_t));
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, parameters.border_color);
        glActiveTexture(GL_TEXTURE0 + unit);
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, handle);
    }

    GLuint Texture::active_unit() const {
        return unit;
    }

    void Texture::active_unit(GLuint texture_unit) {
        glBindTexture(GL_TEXTURE_2D, handle);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        unit = texture_unit;
    }

    Texture::~Texture() noexcept {
        glDeleteTextures(1, &handle);
    }
}
