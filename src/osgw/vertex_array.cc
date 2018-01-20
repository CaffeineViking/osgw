#include <osgw/vertex_array.hh>

namespace osgw {
    VertexArray::VertexArray(const ShaderProgram& shader_program, const Buffer& element_buffer,
                             const std::vector<Attribute>& vertex_attributes)
                            : attributes { vertex_attributes },
                              shader_program { shader_program },
                              element_buffer { element_buffer } {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);

        shader_program.use();
        if (element_buffer.type != Buffer::Type::ElementArray)
            throw std::runtime_error { "Must be an  EBO!" };

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer.handle);
        for(const Attribute& attribute : vertex_attributes) {
            if (attribute.buffer.type != Buffer::Type::Array)
                throw std::runtime_error { "Must be VBO!" };

            glBindBuffer(GL_ARRAY_BUFFER, attribute.buffer.handle);
            GLint attribute_location { glGetAttribLocation(shader_program.handle, attribute.name.c_str()) };

            glEnableVertexAttribArray(attribute_location);
            glVertexAttribPointer(attribute_location, attribute.components,
                                  static_cast<GLenum>(attribute.type),
                                  false, attribute.stride,
                                  attribute.offset);
        }
    }

    VertexArray::~VertexArray() noexcept {
        glDeleteVertexArrays(1, &handle);
    }

    void VertexArray::bind() const {
        glBindVertexArray(handle);
    }
}
