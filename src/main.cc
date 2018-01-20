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
#include <iostream>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw" };
    osgw::Renderer renderer { window };

    std::vector<int> indices { 0, 3, 2,
                               0, 2, 1 };
    std::vector<float> positions { -1.00, +1.00, 0.0,
                                   +1.00, +1.00, 0.0,
                                   +1.00, -1.00, 0.0,
                                   -1.00, -1.00, 0.0 };
    std::vector<float> texture_coordinates { 0.0, 1.0,
                                             1.0, 1.0,
                                             1.0, 0.0,
                                             0.0, 0.0 };

    osgw::Shader vertex_shader   { PATH("shader/triangle.vert"), osgw::Shader::Type::Vertex },
                 fragment_shader { PATH("shader/triangle.frag"), osgw::Shader::Type::Fragment };
    osgw::ShaderProgram shader_program { vertex_shader, fragment_shader };

    osgw::Buffer position_buffer { positions, osgw::Buffer::Type::Array },
                 texture_coordinate_buffer { texture_coordinates, osgw::Buffer::Type::Array },
                 index_buffer { indices, osgw::Buffer::Type::ElementArray };

    osgw::VertexArray::Attribute position_attribute { position_buffer, "position", 3, osgw::VertexArray::Attribute::Type::Float },
                                 texture_coordinate_attribute { texture_coordinate_buffer, "texture_coordinate",  2, osgw::VertexArray::Attribute::Type::Float };
    osgw::VertexArray vertex_array { shader_program, index_buffer, { position_attribute, texture_coordinate_attribute } };

    osgw::Image image { PATH("images/megumin.png") };
    osgw::Texture image_texture { image };
    std::vector<osgw::Texture::Sampler> texture_samplers {
        { image_texture, "texture_sampler" }
    };

    glViewport(0,  0,  window.width(), window.height());
    osgw::Camera camera { window.aspect_ratio(), 27.0 };

    while (window.is_open()) {
        float time = glfwGetTime();
        renderer.clear(0.0, 0.0, 0.0);

        camera.rotate({ 0.0, 0.0, 1.0 }, 0.01);
        renderer.draw(vertex_array, shader_program, texture_samplers, camera,
                      glm::scale(glm::mat4 {  }, glm::vec3 { std::cos(time) }));

        window.display();
    }
}
