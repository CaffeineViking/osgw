#ifndef LWSS_CAMERA_HH
#define LWSS_CAMERA_HH

#include <glm/glm.hpp>

namespace lwss {
    class Camera final {
    public:
        Camera(float aspect_ratio, float field_of_view, float near_plane = 0.08, float far_plane = 800.0);

        void translate(const glm::vec3& translation);
        void rotate(const glm::vec3& axis, float radians);
        void look_at(const glm::vec3& eye, const glm::vec3& point, const glm::vec3& up);

        void change_projection(float aspect_ratio, float field_of_view,
                               float near_plane = 0.08, float far_plane = 800.0);

        const glm::vec4& get_position() const;
        const glm::vec4& get_direction() const;
        const glm::vec4& get_up_direction() const;

        const glm::mat4& get_projection_matrix() const;
        const glm::mat4& get_view_matrix() const;
        const glm::mat4& get_matrix();

        float get_aspect_ratio() const;
        float get_field_of_view() const;
        float get_near_plane() const;
        float get_far_plane() const;

    private:
        float aspect_ratio;
        float field_of_view;
        float near_plane;
        float far_plane;

        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 up_direction;

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 projection_view;
    };
}

#endif
