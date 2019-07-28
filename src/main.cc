#include <osgw/window.hh>
#include <osgw/input.hh>
#include <osgw/input_map.hh>
#include <osgw/renderer.hh>

#include <osgw/shader.hh>
#include <osgw/shader_program.hh>
#include <osgw/vertex_array.hh>
#include <osgw/primitives.hh>
#include <osgw/buffer.hh>

#include <osgw/image.hh>
#include <osgw/texture.hh>
#include <osgw/camera.hh>
#include <osgw/light.hh>

#include <osgw/gerstner.hh>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <cmath>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw" };
    osgw::InputMapper input_mapper { window };
    osgw::Renderer renderer { window };

    window.set_icon(PATH("icon.png"));

    input_mapper.map("quit", osgw::Input::Key::Q);
    input_mapper.map("pause", osgw::Input::Key::P);
    input_mapper.map("wireframe", osgw::Input::Key::W);
    input_mapper.map("fullscreen", osgw::Input::Key::F);

    // This shader program tessellates the target geometry proportional to the eye distance,
    // adds the fractal Gerstner wave function in the normal direction according to the x, z
    // position of the vertex in world coordinates. And then shades it by using Blinn-Phong.

    osgw::Shader ocean_vertex_shader { PATH("shaders/gerstner.vert"), osgw::Shader::Type::Vertex },
                 ocean_tesselation_control_shader { PATH("shaders/gerstner.tesc"), osgw::Shader::Type::TessControl },
                 ocean_tesselation_evaluation_shader { PATH("shaders/gerstner.tese"), osgw::Shader::Type::TessEvaluation },
                 ocean_fragment_shader { PATH("shaders/gerstner.frag"), osgw::Shader::Type::Fragment };

    osgw::ShaderProgram ocean_shader_program { &ocean_vertex_shader,
                                               &ocean_tesselation_control_shader, &ocean_tesselation_evaluation_shader,
                                               &ocean_fragment_shader };

    // Basically, we use an quad as the target primitive to tessellate our ocean with.
    // However the shader program above should work with basically any other geometry.

    osgw::Buffer ocean_position_buffer { osgw::Quad::positions, osgw::Buffer::Type::Array },
                 ocean_normal_buffer { osgw::Quad::normals, osgw::Buffer::Type::Array },
                 ocean_texture_coordinate_buffer { osgw::Quad::texture_coordinates, osgw::Buffer::Type::Array },
                 ocean_index_buffer { osgw::Quad::indices, osgw::Buffer::Type::ElementArray };

    std::vector<osgw::VertexArray::Attribute> ocean_vertex_attributes {
        { ocean_position_buffer, "position", 3, osgw::VertexArray::Attribute::Type::Float },
        { ocean_normal_buffer, "normal", 3, osgw::VertexArray::Attribute::Type::Float },
        { ocean_texture_coordinate_buffer, "texture_coordinate",  2, osgw::VertexArray::Attribute::Type::Float }
    };

    osgw::VertexArray ocean_vertex_array { ocean_shader_program, ocean_index_buffer, ocean_vertex_attributes };

    // Below we setup other necessary things like textures and an
    // camera, plus any necessary lighting information for scene.

    // Load textures here by using osgw::Image and osgw::Texture.
    // e.g. osgw::Image image { "image.png" };
    // then osgw::Texture image_texture { image };
    // then add a texture sampler bound to name...
    std::vector<osgw::Texture::Sampler> ocean_texture_samplers;

    glViewport(0, 0, window.width(), window.height());
    osgw::Camera camera { window.aspect_ratio(), 27.0 };

    osgw::AmbientLight ambient_light { 0.0, 0.0, 0.0 };
    std::vector<osgw::Light> lights {
        { { 0.0, 0.7, -0.7 }, { 1.0, 1.0, 1.0 },
            osgw::Light::Type::Directional },
    };

    input_mapper.map("zoom", osgw::Input::MouseButton::Right);
    input_mapper.map("rotate", osgw::Input::MouseButton::Left);
    input_mapper.map("pan", osgw::Input::MouseButton::Middle);

    // Setup the camera states.

    float camera_zoom { 6.0f };
    glm::vec3 camera_panning_position { 0.0, 0.0, 0.0 };
    float camera_inclination { glm::quarter_pi<float>() },
          camera_azimuth { 0.0 };

    float relative_zoom { 0.0 };
    glm::vec2 relative_mouse { 0.0 };
    float relative_inclination { 0.0 },
          relative_azimuth { 0.0 };
    glm::vec3 relative_pan { 0.0 };

    osgw::GerstnerWave gerstner_wave; // Parameters.
    input_mapper.map("next", osgw::Input::Key::Down);
    input_mapper.map("decrease", osgw::Input::Key::Left);
    input_mapper.map("increase", osgw::Input::Key::Right);
    input_mapper.map("previous", osgw::Input::Key::Up);

    window.reset_time();
    while (window.is_open()) {
        float time = window.time();
        renderer.clear(0, 0.188, 0.203);

        if (input_mapper.pressed("quit")) window.close();
        if (input_mapper.just_pressed("pause"))
            window.toggle_pause();
        if (input_mapper.just_pressed("wireframe"))
            renderer.toggle_wireframe();
        if (input_mapper.just_pressed("fullscreen")) {
            window.toggle_fullscreen();
            glViewport(0, 0, window.width(),
                             window.height());
        }

        // Handle the camera movement, essentially, we are
        // using the lookAt-function, and mouse coordiante
        // X-Y to control the azimuth and inclination, and
        // displace the target position by panning in X-Z.

        glm::vec2 mouse { input_mapper.mouse_position() };
        if (input_mapper.just_pressed("zoom") ||
            input_mapper.just_pressed("rotate") ||
            input_mapper.just_pressed("pan"))
            relative_mouse = mouse;

        glm::vec2 mouse_offset { mouse - relative_mouse };
        mouse_offset.y /= window.height();
        mouse_offset.x /= window.width();

        float Z { camera_zoom + relative_zoom };
        if (input_mapper.pressed("zoom")) {
            relative_zoom = 0.5f*Z*mouse_offset.y;
        } else if (input_mapper.pressed("rotate")) {
            relative_azimuth = -mouse_offset.x;
            relative_inclination = mouse_offset.y;
        } else if (input_mapper.pressed("pan")) {
            relative_pan.x = -Z*mouse_offset.x;
            relative_pan.z = -Z*mouse_offset.y;
        } else {
            camera_zoom += relative_zoom;
            camera_panning_position += relative_pan;
            camera_inclination += relative_inclination;
            camera_zoom = glm::clamp(camera_zoom, 1.0f, 24.0f);
            camera_inclination = glm::clamp(camera_inclination,
                                 0.0f, glm::half_pi<float>());
            camera_azimuth += relative_azimuth;

            relative_zoom = 0.0;
            relative_azimuth = 0.0;
            relative_pan = glm::vec3 { 0.0 };
            relative_inclination = 0.0;
            relative_mouse = mouse;
        }

        float azimuth { camera_azimuth + relative_azimuth };
        float zoom { glm::clamp(camera_zoom + relative_zoom, 1.0f, 24.0f) };
        float inclination { glm::clamp(camera_inclination + relative_inclination,
                                       0.0f, glm::half_pi<float>()) };
        glm::vec3 pan { camera_panning_position + relative_pan };
        pan = glm::rotateY(pan, azimuth);

        glm::vec3 camera_eye_position;
        camera_eye_position.y = std::sin(inclination);
        camera_eye_position.z = std::cos(azimuth);
        camera_eye_position.x = std::sin(azimuth);
        camera_eye_position *= zoom;
        camera_eye_position += pan;

        camera.look_at(camera_eye_position, pan);

        // Below we change the parameters of the Gerstner waves dynamically,
        // we do this by uploading shader uniforms linked to the wave param-
        // eters (things like amplitude, steepness, wave directions, speed).

        if (input_mapper.just_pressed("next")) gerstner_wave.next();
        else if (input_mapper.pressed("increase")) gerstner_wave.change(+0.01);
        else if (input_mapper.pressed("decrease")) gerstner_wave.change(-0.01);
        else if (input_mapper.just_pressed("previous")) gerstner_wave.previous();
        gerstner_wave.select(input_mapper); // Select the current wave to modify.
        if (gerstner_wave.check_and_unset_dirty_bit()) {
            gerstner_wave.upload_uniform(ocean_shader_program);
            std::string updated_title { "osgw (wave " };
            updated_title += std::to_string(gerstner_wave.get_current_wave());
            if (gerstner_wave.is_on()) {
                updated_title += " " + gerstner_wave.get_current_mode() + " = ";
                updated_title += std::to_string(gerstner_wave.get_current_value())
                                                .substr(0, 4);
            } else updated_title += " was disabled";
            window.change_title(updated_title + ")");
        }

        float grid_size = 2; int ocean_radius = 12;
        int ocean_x = std::round(pan.x / grid_size),
            ocean_z = std::round(pan.z / grid_size);
        int ocean_z_min { ocean_z - ocean_radius },
            ocean_z_max { ocean_z + ocean_radius };
        int ocean_x_min { ocean_x - ocean_radius },
            ocean_x_max { ocean_x + ocean_radius };

        // Finally, render the ocean. We use a 24x24 grid of the planes
        // we have instantiated, and wraps them around the viewer. This
        // creates the illusion of having an infinite ocean, while what
        // we actually do is hide the popin by using a fog-like effect.
        // We ignore any of the grid points that are behind the viewer.

        glm::mat4 model_matrix { 1.0 };
        for (int z { ocean_z_min }; z <= ocean_z_max; ++z) {
            for (int x { ocean_x_min }; x <= ocean_x_max; ++x) {
                glm::vec3 grid { grid_size*x, 0, grid_size*z };
                if (glm::dot(grid - camera_eye_position,
                             pan - camera_eye_position) < 0.0) continue;
                model_matrix = glm::translate(glm::mat4 { 1.0 }, grid);
                renderer.draw(ocean_vertex_array, ocean_shader_program,
                              ocean_texture_samplers, camera,
                              model_matrix, lights,
                              ambient_light);
            }
        }

        window.display();
    }
}
