#ifndef OSGW_SHADER_PROGRAM_HH
#define OSGW_SHADER_PROGRAM_HH

#include <osgw/shader.hh>
#include <osgw/texture.hh>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <functional>

namespace osgw {
    class ShaderProgram final {
    public:
        ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> shaders);
        ~ShaderProgram() noexcept;

        void use() const;

        bool has_vertex_shader() const { return shader_stages[0]; }
        bool has_tess_control_shader() const { return shader_stages[1]; }
        bool has_tess_eval_shader() const { return shader_stages[2]; }
        bool has_geometry_shader() const { return shader_stages[3]; }
        bool has_fragment_shader() const { return shader_stages[4]; }

        void sampler(const std::string& id, const Texture& texture);
        void uniform(const std::string& id, float x) { glUniform1f(umap(id), x); }
        void uniform(const std::string& id, unsigned int x) { glUniform1ui(umap(id), x); }
        void uniform(const std::string& id, float x, float y) { glUniform2f(umap(id), x, y); }
        void uniform(const std::string& id, const glm::vec2& v) { glUniform2f(umap(id), v.x, v.y); }
        void uniform2x2(const std::string& id, const glm::mat2& M) { glUniformMatrix2fv(umap(id), 1, false,
                                                                                        glm::value_ptr(M)); }
        void uniform(const std::string& id, const glm::vec3& v) { glUniform3f(umap(id), v.x, v.y, v.z); }
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
        std::array<bool, 5> shader_stages {  };
        std::unordered_map<std::string, GLint> uniforms;
        GLuint handle;
    };
}

#endif
