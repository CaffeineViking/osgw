#include <osgw/gerstner.hh>

// NOTE: this is *not* where the actual Gerstner wave stuff
// is taking place, this class is only here for communicate
// with the GLSL shader and upload any updated uniforms and
// to fetch the Gerstner position when doing physics stuff.
// See:          share/shaders/gerstner.glsl

namespace osgw {
    void GerstnerWave::next() {
        current_mode = Mode((current_mode + 1) % Last);
    }

    void GerstnerWave::previous() {
        current_mode = Mode((current_mode - 1));
        if (current_mode < 0)
            current_mode = Mode(Last - 1);
    }

    void GerstnerWave::increase() {
        switch (current_mode) {
        case Direction: wave_parameters[current_wave].angle += change_rate; break;
        case Amplitude: wave_parameters[current_wave].amplitude += change_rate; break;
        case Steepness: wave_parameters[current_wave].steepness += change_rate; break;
        case Frequency: wave_parameters[current_wave].frequency += change_rate; break;
        case Speed: wave_parameters[current_wave].speed += change_rate;
        default: break;
        }
    }

    void GerstnerWave::decrease() {
        switch (current_mode) {
        case Direction: wave_parameters[current_wave].angle -= change_rate; break;
        case Amplitude: wave_parameters[current_wave].amplitude -= change_rate; break;
        case Steepness: wave_parameters[current_wave].steepness -= change_rate; break;
        case Frequency: wave_parameters[current_wave].frequency -= change_rate; break;
        case Speed: wave_parameters[current_wave].speed -= change_rate;
        default: break;
        }
    }

    void GerstnerWave::select(osgw::InputMapper& input_mapper) {
        int selected_wave = -1;
        if (input_mapper.just_pressed(osgw::Input::Key::One)) selected_wave = 1;
        else if (input_mapper.just_pressed(osgw::Input::Key::Two)) selected_wave = 2;
        else if (input_mapper.just_pressed(osgw::Input::Key::Three)) selected_wave = 3;
        else if (input_mapper.just_pressed(osgw::Input::Key::Four)) selected_wave = 4;

        if (selected_wave == current_wave)
            wave_parameters[current_wave].on = !wave_parameters[current_wave].on;
        if (selected_wave != -1) current_wave = selected_wave;
    }

    void GerstnerWave::upload_uniform(osgw::ShaderProgram& shader_program) const {
        unsigned int waves_on { 0 };
        unsigned int allocated_index { 0 };
        for (std::size_t i { 0 }; i < MAX_WAVES; ++i) {
            if (!wave_parameters[i].on) continue;
            std::string target_array { "gerstner_waves[" };
            auto index = std::to_string(allocated_index++);

            std::string target_struct { target_array + index + "]." };
            shader_program.uniform(target_struct + "direction", std::cos(wave_parameters[i].angle),
                                                                std::sin(wave_parameters[i].angle));
            shader_program.uniform(target_struct + "amplitude", wave_parameters[i].amplitude);
            shader_program.uniform(target_struct + "steepness", wave_parameters[i].steepness);
            shader_program.uniform(target_struct + "frequency", wave_parameters[i].frequency);
            shader_program.uniform(target_struct + "speed", wave_parameters[i].speed);
            ++waves_on;
        } shader_program.uniform("gerstner_waves_size", waves_on);
    }

    glm::vec3 GerstnerWave::value(const glm::vec2&, float, glm::vec3&) const {
        return glm::vec3 { 0.0 };
    }

    float GerstnerWave::get_current_value() const {
        switch (current_mode) {
        case Direction: return wave_parameters[current_wave].angle;
        case Amplitude: return wave_parameters[current_wave].amplitude;
        case Steepness: return wave_parameters[current_wave].steepness;
        case Frequency: return wave_parameters[current_wave].frequency;
        case Speed: return wave_parameters[current_wave].speed;
        default: return 0.0; // If we get here, shit is fucked.
        }
    }

    std::size_t GerstnerWave::get_current_wave() const {
        return current_wave;
    }

    std::string GerstnerWave::get_current_mode() const {
        switch (current_mode) {
        case Direction: return "angle";
        case Amplitude: return "amplitude";
        case Steepness: return "steepness";
        case Frequency: return "frequency";
        case Speed: return "speed";
        default: return "unknown!";
        }
    }
}
