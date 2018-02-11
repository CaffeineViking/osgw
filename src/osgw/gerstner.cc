#include <osgw/gerstner.hh>

#include <cmath>

namespace osgw {
    float gerstner_wave(const glm::vec2& position, float time) {
        return std::sin(position.x + time);
    }
}
