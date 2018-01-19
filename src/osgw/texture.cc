#include <osgw/texture.hh>

namespace osgw {
    Texture::Texture(const Image& image, const Parameters& parameters, GLuint texture_unit) {
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

        glActiveTexture(GL_TEXTURE0 + texture_unit);
        unit = texture_unit;
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, handle);
    }

    GLenum Texture::active_unit() const {
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

    CubeTexture::CubeTexture(const Image& nx, const Image& px,
                             const Image& ny, const Image& py,
                             const Image& nz, const Image& pz,
                             const Parameters& parameters, GLuint texture_unit) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,  0, nx.has_alpha() ? GL_RGBA : GL_RGB,
                     nx.get_width(), nx.get_height(), 0, nx.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, nx.get_data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,  0, px.has_alpha() ? GL_RGBA : GL_RGB,
                     px.get_width(), px.get_height(), 0, px.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, px.get_data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,  0, ny.has_alpha() ? GL_RGBA : GL_RGB,
                     ny.get_width(), ny.get_height(), 0, ny.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, ny.get_data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,  0, py.has_alpha() ? GL_RGBA : GL_RGB,
                     py.get_width(), py.get_height(), 0, py.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, py.get_data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,  0, nz.has_alpha() ? GL_RGBA : GL_RGB,
                     nz.get_width(), nz.get_height(), 0, nz.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, nz.get_data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,  0, pz.has_alpha() ? GL_RGBA : GL_RGB,
                     pz.get_width(), pz.get_height(), 0, pz.has_alpha() ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, pz.get_data());

        if (parameters.min_filter == MinFilter::NearestMipmap ||
            parameters.min_filter == MinFilter::LinearMipmap ||
            parameters.min_filter == MinFilter::SmoothMipmap) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(parameters.mag_filter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(parameters.min_filter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLenum>(parameters.wrap_r));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(parameters.wrap_s));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLenum>(parameters.wrap_t));
        glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, parameters.border_color);

        glActiveTexture(GL_TEXTURE0 + texture_unit);
        unit = texture_unit;
    }

    CubeTexture::~CubeTexture() noexcept {
        glDeleteTextures(1, &handle);
    }

    void CubeTexture::bind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    }

    GLenum CubeTexture::active_unit() const {
        return unit;
    }

    void CubeTexture::active_unit(GLuint texture_unit) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        unit = texture_unit;
    }
}
