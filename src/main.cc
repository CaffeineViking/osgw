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

#include <cmath>
#include <vector>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw" };
    osgw::InputMapper input_mapper { window };
    osgw::Renderer renderer { window };

    std::vector<int> indices { 0, 3, 2, 1 };
    std::vector<float> positions { -1.0, +1.0, 0.0,
                                   +1.0, +1.0, 0.0,
                                   +1.0, -1.0, 0.0,
                                   -1.0, -1.0, 0.0 };
    std::vector<float> normals { 0.0, 0.0, 1.0,
                                 0.0, 0.0, 1.0,
                                 0.0, 0.0, 1.0,
                                 0.0, 0.0, 1.0 };
    std::vector<float> texture_coordinates { 0.0, 1.0,
                                             1.0, 1.0,
                                             1.0, 0.0,
                                             0.0, 0.0 };

    osgw::Shader vertex_shader { PATH("shader/triangle.vert"), osgw::Shader::Type::Vertex },
                 tesselation_control_shader { PATH("shader/triangle.tesc"), osgw::Shader::Type::TessControl },
                 tesselation_evaluation_shader { PATH("shader/triangle.tese"), osgw::Shader::Type::TessEvaluation },
                 fragment_shader { PATH("shader/triangle.frag"), osgw::Shader::Type::Fragment };
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

    osgw::Image diffuse_map_image { PATH("images/megumin.png") };
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
    input_mapper.map("forward", { osgw::Input::Key::W, osgw::Input::Key::Up });
    input_mapper.map("backward", { osgw::Input::Key::S, osgw::Input::Key::Down });
    input_mapper.map("left", { osgw::Input::Key::A, osgw::Input::Key::Left });
    input_mapper.map("right", { osgw::Input::Key::D, osgw::Input::Key::Right });
    input_mapper.map("down", { osgw::Input::Key::Z, osgw::Input::Key::PageDown });
    input_mapper.map("up", { osgw::Input::Key::Q, osgw::Input::Key::PageUp });

    window.reset_time();
    while (window.is_open()) {
        float time = window.time();
        renderer.clear(0.3, 0.3, 0.3);

        if (input_mapper.pressed("quit")) window.close();
        if (input_mapper.just_pressed("fullscreen"))
            window.toggle_fullscreen();


        glm::mat4 model_matrix { 1.0 };
        renderer.draw(vertex_array, shader_program, texture_samplers,
                      camera, model_matrix, lights, ambient_light);


        window.display();
    }
}
