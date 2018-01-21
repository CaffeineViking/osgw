#ifndef OSGW_LIGHT_HH
#define OSGW_LIGHT_HH

#include <glm/glm.hpp>

namespace osgw {
    class AmbientLight final {
    public:
        AmbientLight(const glm::vec3& color) : color { color } {  }
        AmbientLight(float r, float g, float b)
                    : AmbientLight { { r, g, b } } {  }
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

        Light(const glm::vec3& location, const glm::vec3& color, Type type, float falloff = 1.0)
             : location { location }, color { color },  type { type }, falloff { falloff } {  }

        void scale(const glm::vec3& intensity);
        void translate(const glm::vec3& offset);
        void rotate(const glm::vec3& axis, float radians);

        void set_falloff(float value) { falloff = value; }
        void set_color(const glm::vec3& value) { color = value; }
        void set_intensity(const glm::vec3& value) { color *= value; }
        void set_location(const glm::vec3& value) { location = value; }
        void set_direction(const glm::vec3& value) { location = value; }

        const glm::vec3& get_location() const { return location; }
        const glm::vec3& get_direction() const { return location; }
        const glm::vec3& get_color() const { return color; }
        float get_falloff() const { return falloff; }
        Type get_type() const { return type; }

    private:
        glm::vec3 location;
        glm::vec3 color;
        Type type;
        float falloff;
    };
}

#endif
