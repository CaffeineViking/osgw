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

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>

#include <cmath>
#include <iostream>
#include <vector>

#ifndef SHARE_PATH
#define SHARE_PATH "share/"
#endif
#define PATH(X) SHARE_PATH X

int main(int, char**) {
    osgw::Window window { 1280, 720, "osgw", false, false };
    osgw::InputMapper input_mapper { window };
    osgw::Renderer renderer { window };

    // This shader program tessellates the target geometry proportional to the eye distance,
    // adds the fractal Gerstner wave function in the normal direction according to the x, z
    // position of the vertex in world coordinates. And then shades it by using Blinn-Phong.

    osgw::Shader vertex_shader { PATH("shader/gerstner.vert"), osgw::Shader::Type::Vertex },
                 tesselation_control_shader { PATH("shader/gerstner.tesc"), osgw::Shader::Type::TessControl },
                 tesselation_evaluation_shader { PATH("shader/gerstner.tese"), osgw::Shader::Type::TessEvaluation },
                 fragment_shader { PATH("shader/gerstner.frag"), osgw::Shader::Type::Fragment };

    osgw::ShaderProgram shader_program { vertex_shader,
                                         tesselation_control_shader, tesselation_evaluation_shader,
                                         fragment_shader };

    // Basically, we use an quad as the target primitive to tessellate our ocean with.
    // However the shader program above should work with basically any other geometry.

    osgw::Buffer position_buffer { osgw::Quad::positions, osgw::Buffer::Type::Array },
                 normal_buffer { osgw::Quad::normals, osgw::Buffer::Type::Array },
                 texture_coordinate_buffer { osgw::Quad::texture_coordinates, osgw::Buffer::Type::Array },
                 index_buffer { osgw::Quad::indices, osgw::Buffer::Type::ElementArray };

    std::vector<osgw::VertexArray::Attribute> vertex_attributes {
        { position_buffer, "position", 3, osgw::VertexArray::Attribute::Type::Float },
        { normal_buffer, "normal", 3, osgw::VertexArray::Attribute::Type::Float },
        { texture_coordinate_buffer, "texture_coordinate",  2, osgw::VertexArray::Attribute::Type::Float }
    }; osgw::VertexArray vertex_array { shader_program, index_buffer, vertex_attributes };

    // Below we setup other necessary things like textures and an
    // camera, plus any necessary lighting information for scene.

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

    // Specify list of input --> action mappings.

    input_mapper.map("quit", osgw::Input::Key::Q);
    input_mapper.map("wireframe", osgw::Input::Key::W);
    input_mapper.map("fullscreen", osgw::Input::Key::F);

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

    window.reset_time();
    while (window.is_open()) {
        float time = window.time();
        renderer.clear(1.0, 1.0, 1.0);

        if (input_mapper.pressed("quit")) window.close();
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

        glm::mat4 model_matrix { 1.0 };
        for (int z { ocean_z_min }; z <= ocean_z_max; ++z) {
            for (int x { ocean_x_min }; x <= ocean_x_max; ++x) {
                glm::vec3 grid { grid_size*x, 0, grid_size*z };
                if (glm::dot(grid - camera_eye_position,
                             pan - camera_eye_position) < 0.0) continue;
                model_matrix = glm::translate(glm::mat4 { 1.0 }, grid);
                renderer.draw(vertex_array, shader_program,
                              texture_samplers, camera,
                              model_matrix, lights,
                              ambient_light);
            }
        }

        window.display();
    }
}
