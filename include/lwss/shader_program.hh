#ifndef LWSS_SHADER_PROGRAM_HH
#define LWSS_SHADER_PROGRAM_HH

#include <lwss/shader.hh>
#include <lwss/texture.hh>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <functional>

namespace lwss {
    class ShaderProgram final {
    public:
        ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> shaders);
        ~ShaderProgram() noexcept;

        void use() const;

        void sampler(const std::string& id, const Texture& texture);
        void sampler(const std::string& id, const CubeTexture& texture);
        void uniform(const std::string& id, float x) { glUniform1f(umap(id), x); }
        void uniform(const std::string& id, float x, float y) { glUniform2f(umap(id), x, y); }
        void uniform2x2(const std::string& id, const glm::mat2& M) { glUniformMatrix2fv(umap(id), 1, false,
                                                                                        glm::value_ptr(M)); }
        void uniform(const std::string& id, float x, float y, float z) { glUniform3f(umap(id), x, y, z); }
        void uniform3x3(const std::string& id, const glm::mat3& M) { glUniformMatrix3fv(umap(id), 1, false,
                                                                                        glm::value_ptr(M)); }
        void uniform4x4(const std::string& id, const glm::mat4& M) { glUniformMatrix4fv(umap(id), 1, false,
                                                                                        glm::value_ptr(M)); }
        void uniform(const std::string& id, float x, float y, float z, float w) {
            glUniform4f(umap(id), x, y, z, w);
        }

    private:
        friend class VertexArray;
        GLint umap(const std::string&);
        std::unordered_map<std::string, GLint> uniforms;
        GLuint handle;
    };
}

#endif
