#include <osgw/renderer.hh>

namespace osgw {
    Renderer::Renderer(Window& window, const Parameters& parameters)
                      : window { window }, parameters { parameters } {
        set_parameters(parameters);
    }

    void Renderer::clear(float r, float g, float b) {
        if (!window.has_context()) window.request_context();
        GLenum bitfield = GL_COLOR_BUFFER_BIT;

        if (parameters.depth_test)
            bitfield |= GL_DEPTH_BUFFER_BIT;
        if (parameters.stencil_test)
            bitfield |= GL_STENCIL_BUFFER_BIT;

        fog.r = r; fog.g = g; fog.b = b;
        glClearColor(r, g, b, 1.0);
        glClear(bitfield);
    }

    const Renderer::Parameters& Renderer::get_parameters() const {
        return parameters;
    }

    void Renderer::set_parameters(const Parameters& new_parameters) {
        if (new_parameters.blending) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
        } else glDisable(GL_BLEND);

        if (new_parameters.face_culling) {
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            glEnable(GL_CULL_FACE);
        } else glDisable(GL_CULL_FACE);

        if (new_parameters.depth_test) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
        if (new_parameters.multisample) glEnable(GL_MULTISAMPLE);
        else glDisable(GL_MULTISAMPLE);
        if (new_parameters.stencil_test) glEnable(GL_STENCIL_TEST);
        else glDisable(GL_STENCIL_TEST);
        if (new_parameters.seamless_cubemap) glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        else glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        if (new_parameters.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glPatchParameteri(GL_PATCH_VERTICES, 4);
        parameters = new_parameters;
    }

    void Renderer::set_vertex_array(const VertexArray& vertex_array) {
        if (!is_current_vertex_array(&vertex_array))
            vertex_array.bind();
        current_vertex_array = &vertex_array;
    }

    void Renderer::set_shader_program(const ShaderProgram& shader_program) {
        if (!is_current_shader_program(&shader_program))
            shader_program.use();
        current_shader_program = &shader_program;
    }

    bool Renderer::is_current_vertex_array(const VertexArray* vertex_array) const {
        return vertex_array == current_vertex_array;
    }

    bool Renderer::is_current_shader_program(const ShaderProgram* shader_program) const {
        return shader_program == current_shader_program;
    }

    // Assignment of texture units is done by a ring buffer, the oldest
    // assigned texture is the one that is going to be replaced first!!
    std::size_t Renderer::assign_texture_unit(const Texture* texture) {
        int assigned_texture_unit { has_texture_unit(texture) };
        if (assigned_texture_unit == -1) {
            assigned_texture_unit = current_texture_unit;
            current_textures[current_texture_unit] = texture;
            current_texture_unit = (current_texture_unit + 1) % current_textures.size();
        }

        return static_cast<std::size_t>(assigned_texture_unit);
    }

    int Renderer::has_texture_unit(const Texture* texture) const {
        int texture_unit { -1 };
        for (std::size_t i { 0 }; i < current_textures.size(); ++i)
            if (current_textures[i] == texture) texture_unit = i;
        return texture_unit;
    }

    // Here we assume we are limited to 16 TUs, which
    // is pessimistic, but common in "lower-end" GPU.
    std::size_t Renderer::next_texture_unit() const {
        return current_texture_unit;
    }

    void Renderer::setup_shader_program(ShaderProgram& shader_program) {
        set_shader_program(shader_program);
        shader_program.uniform("fog_diffuse", fog);
        shader_program.uniform("time", window.time());
    }

    void Renderer::setup_texture_samplers(ShaderProgram& shader_program, std::vector<Texture::Sampler>& samplers) {
        for (Texture::Sampler& sampler : samplers) {
            std::size_t texture_unit { assign_texture_unit(&sampler.texture) };
            if (texture_unit != sampler.texture.active_unit())
                sampler.texture.active_unit(texture_unit);
            shader_program.sampler(sampler.name, sampler.texture);
        }
    }

    void Renderer::setup_transformation_matrices(ShaderProgram& shader_program, const Camera& camera,
                                                 const glm::mat4& model_matrix) {

        shader_program.uniform("eye_position", camera.get_position());
        shader_program.uniform4x4("projection_view", camera.get_matrix());
        shader_program.uniform4x4("model", model_matrix);
    }

    void Renderer::setup_light_sources(ShaderProgram& shader_program, const std::vector<Light>& lights,
                                       const AmbientLight& ambient_light) {
        shader_program.uniform("ambient_light_color", ambient_light.get_color());
        unsigned int current_directional_light { 0 },
                     current_point_light { 0 };
        for (const Light& light : lights) {
            if (light.get_type() == Light::Type::Directional) {
                std::string target_array { "directional_lights[" };
                auto index = std::to_string(current_directional_light);
                std::string target_struct { target_array + index + "]." };
                shader_program.uniform(target_struct + "color", light.get_color());
                shader_program.uniform(target_struct + "direction", light.get_direction());
                ++current_directional_light;
            } else if (light.get_type() == Light::Type::Point) {
                std::string target_array { "point_lights[" };
                auto index = std::to_string(current_directional_light);
                std::string target_struct { target_array + index + "]." };
                shader_program.uniform(target_struct + "color", light.get_color());
                shader_program.uniform(target_struct + "position", light.get_location());
                shader_program.uniform(target_struct + "falloff", light.get_falloff());
                ++current_point_light;
            }
        }

        shader_program.uniform("point_lights_size", current_point_light);
        shader_program.uniform("directional_lights_size", current_directional_light);
    }

    void Renderer::draw_triangles(VertexArray& vertex_array) {
        set_vertex_array(vertex_array);
        glDrawElements(GL_TRIANGLES, vertex_array.size(),
                       GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::draw_patches(VertexArray& vertex_array) {
        set_vertex_array(vertex_array);
        glDrawElements(GL_PATCHES, vertex_array.size(),
                       GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers) {
        setup_shader_program(shader_program);
        setup_texture_samplers(shader_program, texture_samplers);
        if (!shader_program.has_tess_eval_shader()) draw_triangles(vertex_array);
        else draw_patches(vertex_array); // Checks if we'll use the tessellators.
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers,
                        const Camera& camera, const glm::mat4& model_matrix) {
        setup_shader_program(shader_program);
        setup_texture_samplers(shader_program, texture_samplers);
        setup_transformation_matrices(shader_program, camera, model_matrix);
        if (!shader_program.has_tess_eval_shader()) draw_triangles(vertex_array);
        else draw_patches(vertex_array); // Checks if we'll use the tessellators.
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers,
                        const Camera& camera, const glm::mat4& model_matrix,
                        const std::vector<Light>& lights, const AmbientLight& ambient_light) {
        setup_shader_program(shader_program);
        setup_texture_samplers(shader_program, texture_samplers);
        setup_light_sources(shader_program, lights, ambient_light);
        setup_transformation_matrices(shader_program, camera, model_matrix);
        if (!shader_program.has_tess_eval_shader()) draw_triangles(vertex_array);
        else draw_patches(vertex_array); // Checks if we'll use the tessellators.
    }
}
