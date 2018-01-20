#include <osgw/light.hh>

#include <glm/gtc/matrix_transform.hpp>

namespace osgw {
    void Light::translate(const glm::vec3& offset) {
        glm::mat4 translation { glm::translate(glm::mat4 {  }, offset) };
        location = translation*location;
    }

    void Light::rotate(const glm::vec3& axis, float radians) {
        glm::mat4 rotation { glm::rotate(glm::mat4 {  }, radians, axis) };
        location = rotation*location;
    }
}
