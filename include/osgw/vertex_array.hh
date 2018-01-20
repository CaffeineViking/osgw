#ifndef OSGW_VERTEX_ARRAY_HH
#define OSGW_VERTEX_ARRAY_HH

#include <osgw/buffer.hh>
#include <osgw/shader_program.hh>

#include <GL/glew.h>

#include <string>

namespace osgw {
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

        std::size_t size() const { return element_buffer.size() / sizeof(GLuint); }
        const std::vector<Attribute>& get_attributes() const { return attributes; }
        const ShaderProgram& get_shader_program() const { return shader_program; }
        const Buffer& get_element_buffer() const { return element_buffer; }

        void bind() const;

    private:
        const std::vector<Attribute>& attributes;
        const ShaderProgram& shader_program;
        const Buffer& element_buffer;
        GLuint handle;
    };
}

#endif
