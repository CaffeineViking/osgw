#ifndef OSGW_SHADER_HH
#define OSGW_SHADER_HH

#include <GL/glew.h>

#include <string>
#include <unordered_set>

namespace osgw {
    class Shader final {
    public:
        enum class Type : GLenum {
            Vertex = GL_VERTEX_SHADER,
            TessControl = GL_TESS_CONTROL_SHADER,
            TessEvaluation = GL_TESS_EVALUATION_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            Fragment = GL_FRAGMENT_SHADER
        };

        Shader(const std::string& path, Type type);
        ~Shader() noexcept;

    private:
        friend class ShaderProgram;
        // For handling GLSL dependencies.
        mutable std::string version { "???" };
        mutable std::unordered_set<std::string> deps;
        std::string load(const std::string& path) const;
        std::string preprocess(const std::string& source, const std::string& path) const;

        Type type;
        GLuint handle;
    };
}

#endif
