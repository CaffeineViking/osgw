#ifndef LWSS_VERTEX_ARRAY_HH
#define LWSS_VERTEX_ARRAY_HH

#include <lwss/buffer.hh>
#include <lwss/shader_program.hh>

#include <GL/glew.h>

#include <string>

namespace lwss {
    class VertexArray final {
    public:
        struct Attribute final {
            enum class Type : GLenum {
                Byte = GL_BYTE,
                UnsignedByte = GL_UNSIGNED_BYTE,
                Short = GL_SHORT,
                UnsignedShort = GL_UNSIGNED_SHORT,
                Int = GL_INT,
                UnsignedInt = GL_UNSIGNED_INT,
                HalfFloat = GL_HALF_FLOAT,
                Float = GL_FLOAT,
                Double = GL_DOUBLE
            };

            const Buffer& buffer;
            std::string name;
            GLint components;
            Type type;
            GLsizei stride { 0 };
            const void* offset { 0 };
        };

        VertexArray(const ShaderProgram& shader_program, const Buffer& element_buffer,
                    const std::vector<Attribute>& vertex_attributes);
        ~VertexArray() noexcept;

        void bind() const;

    private:
        GLuint handle;
    };
}

#endif
