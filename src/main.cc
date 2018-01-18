#include <lwss/window.hh>

#include <lwss/shader.hh>
#include <lwss/shader_program.hh>

#include <lwss/vertex_array.hh>
#include <lwss/buffer.hh>

#include <lwss/image.hh>
#include <lwss/texture.hh>

#include <lwss/camera.hh>
#include <lwss/light.hh>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <vector>
#include <iostream>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    lwss::Window window { 1280, 720, "LWSS" };

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

    lwss::Shader vertex_shader { PATH("shader/triangle.vert"), lwss::Shader::Type::Vertex },
                 fragment_shader { PATH("shader/triangle.frag"), lwss::Shader::Type::Fragment };
    lwss::ShaderProgram shader_program { vertex_shader, fragment_shader };

    shader_program.use();

    lwss::Buffer position_buffer { positions, lwss::Buffer::Type::Array },
                 mapping_buffer { mappings, lwss::Buffer::Type::Array },
                 index_buffer { indices, lwss::Buffer::Type::ElementArray };

    lwss::VertexArray::Attribute position_attribute { position_buffer, "position", 3, lwss::VertexArray::Attribute::Type::Float },
                                 mapping_attribute { mapping_buffer, "mapping", 2, lwss::VertexArray::Attribute::Type::Float };
    lwss::VertexArray vertex_array { shader_program, index_buffer, { position_attribute, mapping_attribute } };

    vertex_array.bind();

    lwss::Image image { PATH("images/megumin.png") };
    lwss::Texture image_texture { image, 0 };

    shader_program.sampler("sampler", image_texture);

    glViewport(0,  0,  window.width(), window.height());
    lwss::Camera camera { window.aspect_ratio(), 27.0 };

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
