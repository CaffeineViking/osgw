#include <lwss/window.hh>

#include <lwss/shader.hh>
#include <lwss/shader_program.hh>

#include <lwss/vertex_array.hh>
#include <lwss/buffer.hh>

#include <vector>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    lwss::Window window { 1280, 720, "LWSS" };

    std::vector<int> vertex_indices { 0, 1, 2 };
    std::vector<float> vertex_positions { +0.0, +0.5, 0.0,
                                          -0.5, -0.5, 0.0,
                                          +0.5, -0.5, 0.0 };

    lwss::Shader vertex_shader { PATH("shader/triangle.vert"), lwss::Shader::Type::Vertex },
                 fragment_shader { PATH("shader/triangle.frag"), lwss::Shader::Type::Fragment };
    lwss::ShaderProgram shader_program { vertex_shader, fragment_shader };

    shader_program.use();

    lwss::Buffer vertex_position_buffer { vertex_positions, lwss::Buffer::Type::Array },
                 vertex_index_buffer { vertex_indices, lwss::Buffer::Type::ElementArray };

    lwss::VertexArray vertex_array { shader_program, vertex_index_buffer, {
        { vertex_position_buffer, "position", 3, lwss::VertexArray::Attribute::Type::Float }
    } };

    vertex_array.bind();

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.is_open()) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        window.display();
    }
}
