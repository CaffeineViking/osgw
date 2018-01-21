#include <osgw/window.hh>
#include <osgw/renderer.hh>

#include <osgw/shader.hh>
#include <osgw/shader_program.hh>
#include <osgw/vertex_array.hh>
#include <osgw/buffer.hh>

#include <osgw/image.hh>
#include <osgw/texture.hh>
#include <osgw/camera.hh>
#include <osgw/light.hh>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <vector>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw" };
    osgw::Renderer renderer { window };

    std::vector<int> indices { 2, 0, 3,
                               0, 2, 1 };
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

    osgw::Image diffuse_map_image { PATH("images/megumin.png") },
                opacity_map_image { PATH("images/squares.png") };

    osgw::Texture diffuse_map { diffuse_map_image },
                  opacity_map { opacity_map_image };
    std::vector<osgw::Texture::Sampler> texture_samplers {
        { diffuse_map, "diffuse_map" },
        { opacity_map, "opacity_map" }
    };

    glViewport(0, 0, window.width(), window.height());
    osgw::Camera camera { window.aspect_ratio(), 27.0 };

    window.reset_time();
    while (window.is_open()) {
        float time = window.time();
        renderer.clear(0.0, 0.0, 0.0);

        camera.rotate({ 0.0, 0.0, 1.0 }, 0.01);
        glm::mat4 model_matrix { glm::scale({},
                                 glm::vec3 { std::cos(time) }) };
        renderer.draw(vertex_array, shader_program, texture_samplers,
                      camera, model_matrix);

        window.display();
    }
}
