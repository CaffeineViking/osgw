#include <lwss/shader_program.hh>

namespace lwss {
    ShaderProgram::ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> shaders) {
        handle = glCreateProgram();
        for (const Shader& shader : shaders)
            glAttachShader(handle, shader.handle);
        glLinkProgram(handle);

        GLint link_success;
        glGetShaderiv(handle, GL_LINK_STATUS, &link_success);
        if (!link_success) {
            GLint log_length;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

            GLchar* log { new GLchar[log_length] }; // Will leak >:).
            glGetShaderInfoLog(handle, log_length, &log_length, log);
            throw std::runtime_error { log };
        }
    }

    ShaderProgram::~ShaderProgram() noexcept {
        glDeleteProgram(handle);
    }

    void ShaderProgram::use() const {
        glUseProgram(handle);
    }

    GLint ShaderProgram::umap(const std::string& id) {
        auto uniform = uniforms.find(id);
        if (uniform == uniforms.end())
            uniforms[id] = glGetUniformLocation(handle,
                                                id.c_str());
        return uniforms[id];
    }
}
