#ifndef OSGW_GERSTNER_HH
#define OSGW_GERSTNER_HH

#include <glm/glm.hpp>

namespace osgw {
    glm::vec3 gerstner_wave(const glm::vec2& position, float time, glm::vec3& normal);
}

#endif
