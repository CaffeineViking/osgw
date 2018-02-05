#include <osgw/window.hh>
#include <osgw/input.hh>
#include <osgw/input_map.hh>
#include <osgw/renderer.hh>

#include <osgw/shader.hh>
#include <osgw/shader_program.hh>
#include <osgw/vertex_array.hh>
#include <osgw/buffer.hh>

#include <osgw/image.hh>
#include <osgw/texture.hh>
#include <osgw/camera.hh>
#include <osgw/light.hh>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <cmath>
#include <vector>
#include <iostream>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw" };
    osgw::InputMapper input_mapper { window };
    osgw::Renderer renderer { window };

    std::vector<int> indices { 0, 3, 2, 1 };
    std::vector<float> positions { -1.0, 0.0, -1.0,
                                   +1.0, 0.0, -1.0,
                                   +1.0, 0.0, +1.0,
                                   -1.0, 0.0, +1.0 };
    std::vector<float> normals { 0.0, 1.0, 0.0,
                                 0.0, 1.0, 0.0,
                                 0.0, 1.0, 0.0,
                                 0.0, 1.0, 0.0 };
    std::vector<float> texture_coordinates { 0.0, 1.0,
                                             1.0, 1.0,
                                             1.0, 0.0,
                                             0.0, 0.0 };

    osgw::Shader vertex_shader { PATH("shader/gerstner.vert"), osgw::Shader::Type::Vertex },
                 tesselation_control_shader { PATH("shader/gerstner.tesc"), osgw::Shader::Type::TessControl },
                 tesselation_evaluation_shader { PATH("shader/gerstner.tese"), osgw::Shader::Type::TessEvaluation },
                 fragment_shader { PATH("shader/gerstner.frag"), osgw::Shader::Type::Fragment };
    osgw::ShaderProgram shader_program { vertex_shader,
                                         tesselation_control_shader, tesselation_evaluation_shader,
                                         fragment_shader };

    osgw::Buffer position_buffer { positions, osgw::Buffer::Type::Array },
                 normal_buffer { normals, osgw::Buffer::Type::Array },
                 texture_coordinate_buffer { texture_coordinates, osgw::Buffer::Type::Array },
                 index_buffer { indices, osgw::Buffer::Type::ElementArray };

    std::vector<osgw::VertexArray::Attribute> vertex_attributes {
        { position_buffer, "position", 3, osgw::VertexArray::Attribute::Type::Float },
        { normal_buffer, "normal", 3, osgw::VertexArray::Attribute::Type::Float },
        { texture_coordinate_buffer, "texture_coordinate",  2, osgw::VertexArray::Attribute::Type::Float }
    };
    osgw::VertexArray vertex_array { shader_program, index_buffer, vertex_attributes };

    osgw::Image diffuse_map_image { PATH("images/checker.png") };
    osgw::Texture diffuse_map { diffuse_map_image };
    std::vector<osgw::Texture::Sampler> texture_samplers {
        { diffuse_map, "diffuse_map" }
    };

    glViewport(0, 0, window.width(), window.height());
    osgw::Camera camera { window.aspect_ratio(), 27.0 };

    osgw::AmbientLight ambient_light { 0.3, 0.3, 0.3 };
    std::vector<osgw::Light> lights {
        { { 0.0, 0.0, -1.0 }, { 0.0, 1.0, 0.0 }, osgw::Light::Type::Directional },
        { { +1.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, osgw::Light::Type::Point, 1.00 },
        { { -1.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 }, osgw::Light::Type::Point, 1.00 }
    };

    input_mapper.map("quit", { osgw::Input::Key::Q, osgw::Input::Key::Escape });
    input_mapper.map("fullscreen", osgw::Input::Key::F);

    input_mapper.map("zoom", osgw::Input::MouseButton::Right);
    input_mapper.map("rotate", osgw::Input::MouseButton::Left);
    input_mapper.map("pan", osgw::Input::MouseButton::Middle);

    float camera_zoom { 1.0f };
    glm::vec3 camera_panning_position { 0.0, 0.0, 0.0 };
    float camera_inclination { glm::quarter_pi<float>() },
          camera_azimuth { 0.0 };

    float relative_zoom { 0.0 };
    glm::vec2 relative_mouse { 0.0 };
    float relative_inclination { 0.0 },
          relative_azimuth { 0.0 };
    glm::vec3 relative_pan { 0.0 };

    window.reset_time();
    while (window.is_open()) {
        float time = window.time();
        renderer.clear(0.3, 0.3, 0.3);

        if (input_mapper.pressed("quit")) window.close();
        if (input_mapper.just_pressed("fullscreen")) {
            window.toggle_fullscreen();
            glViewport(0, 0, window.width(),
                             window.height());
        }

        glm::vec2 mouse { input_mapper.mouse_position() };
        if (input_mapper.just_pressed("zoom") ||
            input_mapper.just_pressed("rotate") ||
            input_mapper.just_pressed("pan"))
            relative_mouse = mouse;
        glm::vec2 mouse_offset { mouse - relative_mouse };

        if (input_mapper.pressed("zoom")) {
        } else if (input_mapper.pressed("rotate")) {
        } else if (input_mapper.pressed("pan")) {
        } else {
            camera_zoom += relative_zoom;
            camera_panning_position += relative_pan;
            camera_inclination += relative_inclination;
            camera_azimuth += relative_azimuth;

            relative_zoom = 0.0;
            relative_azimuth = 0.0;
            relative_pan = glm::vec3 { 0.0 };
            relative_inclination = 0.0;
            relative_mouse = mouse;
        }

        float zoom { camera_zoom + relative_zoom };
        float azimuth { camera_azimuth + relative_azimuth };
        float inclination { camera_inclination + relative_inclination };

        camera_panning_position += relative_pan;
        glm::vec3 camera_eye_position { 0.0, 0.0, 1.0 };
        camera_eye_position.y = std::sin(inclination);
        camera_eye_position.z = std::sin(azimuth);
        camera_eye_position.x = std::cos(azimuth);
        camera_eye_position *= zoom;

        camera.look_at(camera_eye_position, camera_panning_position);
        glm::mat4 model_matrix { 1.0 }; // Don't do anything for now.
        renderer.draw(vertex_array, shader_program, texture_samplers,
                      camera, model_matrix, lights, ambient_light);

        window.display();
    }
}
