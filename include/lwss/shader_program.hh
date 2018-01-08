#ifndef LWSS_SHADER_PROGRAM_HH
#define LWSS_SHADER_PROGRAM_HH

#include <lwss/shader.hh>

#include <GL/glew.h>

#include <functional>

namespace lwss {
    class ShaderProgram final {
    public:
        ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> shaders);
        ~ShaderProgram() noexcept;

        void use() const;

    private:
        friend class VertexArray;
        GLuint handle;
    };
}

#endif
