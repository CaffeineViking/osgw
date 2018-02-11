#ifndef OSGW_PRIMITIVES_HH
#define OSGW_PRIMITIVES_HH

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

namespace osgw {
    struct Triangle final {
    };

    struct Quad final {
        static std::vector<int> indices;
        static std::vector<float> positions;
        static std::vector<float> normals;
        static std::vector<float> texture_coordinates;
    };
}

#endif
