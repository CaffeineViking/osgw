#include <osgw/gerstner.hh>

#include <cmath>

namespace osgw {
    glm::vec3 gerstner_wave(const glm::vec2& position, float time, glm::vec3& normal) {
        return glm::vec3 { position.s, std::sin(position.s + time), position.t };
    }
}
