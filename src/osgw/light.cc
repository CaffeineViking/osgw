#include <osgw/light.hh>

#include <glm/gtc/matrix_transform.hpp>

namespace osgw {
    void Light::translate(const glm::vec3& offset) {
        location += offset;
    }

    void Light::rotate(const glm::vec3& axis, float radians) {
        glm::mat4 rotation { glm::rotate(glm::mat4 {  }, radians, axis) };
        glm::vec4 loc { rotation * glm::vec4(location, 1.0) };
        location = glm::vec3 { loc.x, loc.y, loc.z };
    }

    void Light::scale(const glm::vec3& intensity) {
        color *= intensity;
    }
}
