#include <osgw/window.hh>

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

    std::vector<int> indices { 0, 3, 2,
                               0, 2, 1 };
    std::vector<float> positions { -1.00, +1.00, 0.0,
                                   +1.00, +1.00, 0.0,
                                   +1.00, -1.00, 0.0,
                                   -1.00, -1.00, 0.0 };
    std::vector<float> mappings { 0.0, 1.0,
                                  1.0, 1.0,
                                  1.0, 0.0,
                                  0.0, 0.0 };

    osgw::Shader vertex_shader   { PATH("shader/triangle.vert"), osgw::Shader::Type::Vertex },
                 fragment_shader { PATH("shader/triangle.frag"), osgw::Shader::Type::Fragment };
    osgw::ShaderProgram shader_program { vertex_shader, fragment_shader };

    shader_program.use();

    osgw::Buffer position_buffer { positions, osgw::Buffer::Type::Array },
                 mapping_buffer  { mappings, osgw::Buffer::Type::Array },
                 index_buffer    { indices, osgw::Buffer::Type::ElementArray };

    osgw::VertexArray::Attribute position_attribute { position_buffer, "position", 3, osgw::VertexArray::Attribute::Type::Float },
                                 mapping_attribute  { mapping_buffer,  "mapping",  2, osgw::VertexArray::Attribute::Type::Float };
    osgw::VertexArray vertex_array { shader_program, index_buffer, { position_attribute, mapping_attribute } };

    vertex_array.bind();

    osgw::Image image { PATH("images/megumin.png") };
    osgw::Texture image_texture { image, 0 };

    shader_program.sampler("sampler", image_texture);

    glViewport(0,  0,  window.width(), window.height());
    osgw::Camera camera { window.aspect_ratio(), 27.0 };

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (window.is_open()) {
        shader_program.uniform("time", glfwGetTime());
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        camera.rotate({ 0.0, 0.0, 1.0 }, 0.01);
        shader_program.uniform4x4("perspective_view", camera.get_matrix());
        shader_program.uniform4x4("model", glm::scale(glm::mat4 {  }, glm::vec3 { std::cos(static_cast<float>(glfwGetTime())) }));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        window.display();
    }
}
