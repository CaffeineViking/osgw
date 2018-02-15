#ifndef OSGW_GERSTNER_HH
#define OSGW_GERSTNER_HH

// NOTE: this is *not* where the actual Gerstner wave stuff
// is taking place, this class is only here for communicate
// with the GLSL shader and upload any updated uniforms and
// to fetch the Gerstner position when doing physics stuff.
// See:          share/shaders/gerstner.glsl

#include <osgw/shader_program.hh>
#include <osgw/input_map.hh>
#include <osgw/input.hh>

#include <glm/glm.hpp>

#include <string>
#include <cmath>

namespace osgw {
    class GerstnerWave {
    public:
        struct Parameter {
            float angle { 0.0 };
            float amplitude { 0.0 };
            float steepness { 0.0 };
            float frequency { 1.0 };
            float speed { 1.0 };
            bool on { false };
        };

        GerstnerWave() {
            wave_parameters[1].angle = 0.32;
            wave_parameters[1].amplitude = 0.64;
            wave_parameters[1].steepness = 0.64;
            wave_parameters[1].frequency = 0.64;
            wave_parameters[1].speed = 1.28;
            wave_parameters[1].on = true;
        }

        void next();
        void previous();
        void change(float value);
        std::size_t waves_on() const;

        bool check_and_unset_dirty_bit();
        void select(osgw::InputMapper& input_mapper);
        void upload_uniform(osgw::ShaderProgram& shader_program) const;
        // This is the function call that actually evaluates the Gerstner wave at (x,z).
        glm::vec3 value(const glm::vec2& position, float time, glm::vec3& normal) const;

        bool is_on() const;
        float get_current_value() const;
        std::size_t get_current_wave() const;
        std::string get_current_mode() const;

    private:
        enum Mode {
            Direction  = 0,
            Amplitude  = 1,
            Steepness  = 2,
            Frequency  = 3,
            Speed      = 4,
            Last
        } current_mode = Direction;

        bool dirty { true };
        int current_wave { 1 };
        static constexpr std::size_t MAX_WAVES { 8 };
        Parameter wave_parameters[MAX_WAVES];
    };
}

#endif
