#include <osgw/primitives.hh>

namespace osgw {
    std::vector<int> Quad::indices = { 0, 3, 2, 1 };
    std::vector<float> Quad::positions { -1.0, 0.0, -1.0,
                                         +1.0, 0.0, -1.0,
                                         +1.0, 0.0, +1.0,
                                         -1.0, 0.0, +1.0 };
    std::vector<float> Quad::normals { 0.0, 1.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 1.0, 0.0 };
    std::vector<float> Quad::texture_coordinates { 0.0, 1.0,
                                                   1.0, 1.0,
                                                   1.0, 0.0,
                                                   0.0, 0.0 };
}
