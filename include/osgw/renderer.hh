#ifndef OSGW_RENDERER_HH
#define OSGW_RENDERER_HH

#include <osgw/window.hh>
#include <osgw/camera.hh>
#include <osgw/vertex_array.hh>
#include <osgw/shader_program.hh>
#include <osgw/texture.hh>
#include <osgw/light.hh>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace osgw {
    class Renderer final {
    public:
        struct Parameters final {
            bool blending { false };
            bool face_culling { true };
            bool depth_test { true };
            bool multisample { true };
            bool wireframe { false };
            bool stencil_test { false };
            bool seamless_cubemap { true };
        };

        Renderer(Window& window, const Parameters& parameters);
        Renderer(Window& window) : Renderer { window, {} }  { }

        void set_parameters(const Parameters& new_parameters);
        const Parameters& get_parameters() const;

        void set_vertex_array(const VertexArray& vertex_array);
        void set_shader_program(const ShaderProgram& shader_program);
        bool is_current_vertex_array(const VertexArray* vertex_array) const;
        bool is_current_shader_program(const ShaderProgram* shader_program) const;
        std::size_t assign_texture_unit(const Texture* texture);
        int has_texture_unit(const Texture* texture) const;
        std::size_t next_texture_unit() const;

        void clear(float r, float g, float b);

        void draw_patches(VertexArray&);
        void draw_triangles(VertexArray&);
        void setup_shader_program(ShaderProgram&);
        void setup_texture_samplers(ShaderProgram&, std::vector<Texture::Sampler>&);
        void setup_transformation_matrices(ShaderProgram&, const Camera&, const glm::mat4&);
        void setup_light_sources(ShaderProgram&, const std::vector<Light>&, const AmbientLight&);

        void draw(VertexArray&, ShaderProgram&, std::vector<Texture::Sampler>&);
        void draw(VertexArray&, ShaderProgram&, std::vector<Texture::Sampler>&,
                  const Camera&, const glm::mat4&);
        void draw(VertexArray&, ShaderProgram&, std::vector<Texture::Sampler>&,
                  const Camera&, const glm::mat4&,
                  const std::vector<Light>&, const AmbientLight&);

    private:
        const VertexArray* current_vertex_array { nullptr };
        const ShaderProgram* current_shader_program { nullptr };
        std::array<const Texture*, 16> current_textures {  };
        std::size_t current_texture_unit { 0 };

        Window& window;
        Parameters parameters;
    };
}

#endif
