#ifndef OSGW_WINDOW_HH
#define OSGW_WINDOW_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cstddef>

namespace osgw {
    class Window final {
    public:
        struct Context final {
            int major_version { 4 },
                minor_version { 1 };
            bool forward_compatible { true },
                 core_profile { true };
            int msaa_samples { 4 };
        };

        Window(std::size_t width, std::size_t height, const std::string& title,
               bool fullscreen, bool vertical_sync,
               const Context& context_config);
        Window(std::size_t width, std::size_t height, const std::string& title,
               bool fullscreen = false, bool vertical_sync = true)
               : Window { width, height, title, fullscreen, vertical_sync,
                          Context { /* Just use default value. */ } } {  }
        ~Window() noexcept;

        GLFWwindow* get_handle() const;

        void request_context();
        bool has_context() const;

        double delta_time() const;
        bool is_open() const;
        void close();
        void display();

        int width() const;
        int height() const;
        float vertical_dpi() const;
        float horizontal_dpi() const;
        float aspect_ratio() const;
        void resize(std::size_t width, std::size_t height);

        bool is_fullscreen() const;
        void toggle_fullscreen();

    private:
        std::string title,
                    current_title;

        int refresh_rate;
        bool fullscreen { false };
        int windowed_x, windowed_y;
        int windowed_width, windowed_height;
        int fullscreen_width, fullscreen_height;

        GLFWwindow* handle;

        double last_time { 0.0 },
               frame_time { .0 },
               fps_timer { 0.0 };
        std::size_t frames { 0 };
    };
}

#endif
