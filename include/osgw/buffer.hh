#ifndef OSGW_BUFFER_HH
#define OSGW_BUFFER_HH

#include <GL/glew.h>

#include <vector>

namespace osgw {
    class Buffer final {
    public:
        enum class Type : GLenum {
            Array = GL_ARRAY_BUFFER,
            ElementArray = GL_ELEMENT_ARRAY_BUFFER,
            CopyRead = GL_COPY_READ_BUFFER,
            CopyWrite = GL_COPY_WRITE_BUFFER,
            Texture = GL_TEXTURE_BUFFER,
            TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
            Uniform = GL_UNIFORM_BUFFER
        };

        enum class Usage : GLenum {
            StreamDraw = GL_STREAM_DRAW,
            StreamRead = GL_STREAM_READ,
            StreamCopy = GL_STREAM_COPY,
            StaticDraw = GL_STATIC_DRAW,
            StaticRead = GL_STATIC_READ,
            StaticCopy = GL_STATIC_COPY,
            DynamicDraw = GL_DYNAMIC_DRAW,
            DynamicRead = GL_DYNAMIC_READ,
            DynamicCopy = GL_DYNAMIC_COPY
        };

        Buffer(const void* data, std::size_t bytes, Type type,
               Usage usage = Usage::StaticDraw);

        template<typename T>
        Buffer(const std::vector<T>& data, Type type,
               Usage usage = Usage::StaticDraw)
               : Buffer { data.data(), data.size() * sizeof(T),
                          type, usage } {  }

        ~Buffer() noexcept;

        void bind() const;
        std::size_t size() const;

    private:
        friend class VertexArray;
        std::size_t size_in_bytes;
        Type type;
        GLuint handle;
    };
}

#endif
