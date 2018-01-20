#ifndef OSGW_LIGHT_HH
#define OSGW_LIGHT_HH

#include <glm/glm.hpp>

namespace osgw {
    class AmbientLight final {
    public:
        AmbientLight(const glm::vec3& color) : color { color } {  }
        void set_color(const glm::vec3& value) { color = value; }
        const glm::vec3& get_color() const { return color; }

    private:
        glm::vec3 color;
    };

    class Light final {
    public:
        enum class Type {
            Directional,
            Point
        };

        Light(const glm::vec3& location, const glm::vec3& color, Type type )
             : location { location, type == Type::Point ? 1.0 : 0.0  },
               color { color }, type { type } {  }

        void translate(const glm::vec3& offset);
        void rotate(const glm::vec3& axis, float radians);

        void set_color(const glm::vec3& value) { color = value; }
        void set_location(const glm::vec4& value) { location = value; }
        const glm::vec4& get_location() const { return location; }
        const glm::vec3& get_color() const { return color; }

    private:
        glm::vec4 location;
        glm::vec3 color;
        Type type;
    };
}

#endif
