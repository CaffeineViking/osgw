#include <osgw/camera.hh>

#include <glm/gtc/matrix_transform.hpp>

namespace osgw {
    Camera::Camera(float aspect_ratio, float field_of_view, float near_plane, float far_plane)
                  : aspect_ratio { aspect_ratio }, field_of_view { field_of_view },
                    near_plane { near_plane}, far_plane { far_plane } {
        change_projection(aspect_ratio, field_of_view, near_plane, far_plane);
        look_at({ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 });
    }

    void Camera::translate(const glm::vec3& offset) {
        glm::mat4 translation { glm::translate(glm::mat4 {  }, -offset) };
        view = glm::translate(view, -offset);
        position = position + offset;
    }

    void Camera::rotate(const glm::vec3& axis, float radians) {
        glm::mat4 rotation { glm::rotate(glm::mat4 {  }, -radians, axis) };
        view = glm::rotate(view, -radians, axis);

        glm::mat4 irot { glm::rotate(glm::mat4 {  }, radians, axis) };
        glm::vec4 up { irot * glm::vec4 { up_direction, 0.0 } };
        glm::vec4 dir { irot * glm::vec4 { direction, 0.0 } };

        direction = glm::vec3 { dir.x, dir.y, dir.z };
        up_direction = glm::vec3 { up.x, up.y, up.z };
    }

    void Camera::look_at(const glm::vec3& eye, const glm::vec3& point, const glm::vec3& up) {
        view = glm::lookAt(eye, point, up);
        position = eye;
        up_direction = up;
        look_at_point = point;
        direction = glm::normalize(point - eye);
    }

    void Camera::change_projection(float aspect_ratio, float field_of_view,
                                   float near_plane, float far_plane) {
        projection = glm::perspective(field_of_view, aspect_ratio,
                                      near_plane, far_plane);
        this->field_of_view = field_of_view;
        this->aspect_ratio = aspect_ratio;
        this->near_plane = near_plane;
        this->far_plane = far_plane;
    }

    const glm::vec3& Camera::get_position() const {
        return position;
    }

    const glm::vec3& Camera::get_direction() const {
        return direction;
    }

    const glm::vec3& Camera::get_up_direction() const {
        return up_direction;
    }

    const glm::mat4& Camera::get_projection_matrix() const {
        return projection;
    }

    const glm::mat4& Camera::get_view_matrix() const {
        return view;
    }

    const glm::mat4& Camera::get_matrix() const {
        projection_view = projection*view;
        return projection_view;
    }

    float Camera::get_aspect_ratio() const {
        return aspect_ratio;
    }

    float Camera::get_field_of_view() const {
        return field_of_view;
    }

    float Camera::get_near_plane() const {
        return near_plane;
    }

    float Camera::get_far_plane() const {
        return far_plane;
    }
}
