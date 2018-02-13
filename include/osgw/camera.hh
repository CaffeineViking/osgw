#ifndef OSGW_CAMERA_HH
#define OSGW_CAMERA_HH

#include <glm/glm.hpp>

namespace osgw {
    class Camera final {
    public:
        Camera(float aspect_ratio, float field_of_view, float near_plane = 0.08, float far_plane = 800.0);

        void translate(const glm::vec3& translation);
        void rotate(const glm::vec3& axis, float radians);
        void look_at(const glm::vec3& eye, const glm::vec3& point,
                     const glm::vec3& up = { 0.0, 1.0, 0.0 });

        void change_projection(float aspect_ratio, float field_of_view,
                               float near_plane = 0.08, float far_plane = 800.0);

        const glm::vec3& get_position() const;
        const glm::vec3& get_direction() const;
        const glm::vec3& get_up_direction() const;

        const glm::mat4& get_projection_matrix() const;
        const glm::mat4& get_view_matrix() const;
        const glm::mat4& get_matrix() const;

        float get_aspect_ratio() const;
        float get_field_of_view() const;
        float get_near_plane() const;
        float get_far_plane() const;

    private:
        float aspect_ratio;
        float field_of_view;
        float near_plane;
        float far_plane;

        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up_direction;
        glm::vec3 look_at_point;

        glm::mat4 view;
        glm::mat4 projection;
        mutable glm::mat4 projection_view;
    };
}

#endif
