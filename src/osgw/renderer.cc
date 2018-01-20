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

    void Renderer::draw(VertexArray& vertex_array) {
        set_vertex_array(vertex_array);
        glDrawElements(GL_TRIANGLES, vertex_array.size(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers) {
        set_shader_program(shader_program);
        shader_program.uniform("time", window.time());
        for (Texture::Sampler& texture_sampler : texture_samplers) {
            std::size_t texture_unit { assign_texture_unit(&texture_sampler.texture) };
            if (texture_unit != texture_sampler.texture.active_unit())
                texture_sampler.texture.active_unit(texture_unit);
            shader_program.sampler(texture_sampler.name, texture_sampler.texture);
        }

        draw(vertex_array);
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers,
                        const Camera& camera, const glm::mat4& model_matrix) {
        set_shader_program(shader_program);
        shader_program.uniform("time", window.time());
        for (Texture::Sampler& texture_sampler : texture_samplers) {
            std::size_t texture_unit { assign_texture_unit(&texture_sampler.texture) };
            if (texture_unit != texture_sampler.texture.active_unit())
                texture_sampler.texture.active_unit(texture_unit);
            shader_program.sampler(texture_sampler.name, texture_sampler.texture);
        }

        shader_program.uniform4x4("projection_view", camera.get_matrix());
        shader_program.uniform4x4("model", model_matrix);
        draw(vertex_array);
    }

    void Renderer::draw(VertexArray& vertex_array, ShaderProgram& shader_program,
                        std::vector<Texture::Sampler>& texture_samplers,
                        const Camera& camera, const glm::mat4& model_matrix,
                        const std::vector<Light>& lights, const AmbientLight& ambient_light) {
        set_shader_program(shader_program);
        shader_program.uniform("time", window.time());
        for (Texture::Sampler& texture_sampler : texture_samplers) {
            std::size_t texture_unit { assign_texture_unit(&texture_sampler.texture) };
            if (texture_unit != texture_sampler.texture.active_unit())
                texture_sampler.texture.active_unit(texture_unit);
            shader_program.sampler(texture_sampler.name, texture_sampler.texture);
        }

        // Upload light and ambient light via uniforms to the shader prog.

        shader_program.uniform4x4("projection_view", camera.get_matrix());
        shader_program.uniform4x4("model", model_matrix);
        draw(vertex_array);
    }
}
