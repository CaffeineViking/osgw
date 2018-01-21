#include <osgw/window.hh>

#include <iostream>
#include <stdexcept>

namespace osgw {
    Window::Window(std::size_t width, std::size_t height, const std::string& title,
                   bool fullscreen, bool vertical_sync, const Context& context_config)
                   : title { title }, current_title { title } {
        if (!glfwInit()) throw std::runtime_error { "Failed to start GLFW!" };
        std::cerr << "GLFW version: " << glfwGetVersionString() << std::endl;

        glfwWindowHint(GLFW_SAMPLES, context_config.msaa_samples);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_config.major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_config.minor_version);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, context_config.forward_compatible);
        int profile_flag { context_config.core_profile ? GLFW_OPENGL_CORE_PROFILE
                                                       : GLFW_OPENGL_ANY_PROFILE };
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile_flag);

        GLFWmonitor* monitor { glfwGetPrimaryMonitor() };
        const GLFWvidmode* mode { glfwGetVideoMode(monitor) };

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!handle) throw std::runtime_error { "Couldn't open a GLFW window!" };

        fullscreen_width = mode->width;
        fullscreen_height = mode->height;
        refresh_rate = mode->refreshRate;
        windowed_height = height;
        windowed_width = width;
        windowed_x = fullscreen_width  / 2 - width  / 2;
        windowed_y = fullscreen_height / 2 - height / 2;

        glfwSetWindowPos(handle, windowed_x, windowed_y);
        if (fullscreen) toggle_fullscreen();
        glfwShowWindow(handle);

        request_context();

        glewExperimental = GL_TRUE; // Actually provides support for "modern" OpenGL...
        if (glewInit() != GLEW_OK) throw std::runtime_error { "Failed to load GLEW!" };
        std::cerr << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

        std::cerr << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cerr << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cerr << "OpenGL GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        std::cerr << "OpenGL extensions: ";

        // Notes: glGetString(GL_EXTENSIONS) is
        // deprecated in later versions of OGL,
        // below we handle the case for this...
        if (context_config.major_version > 2) {
        int extensions;
            glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
            for (int i { 0 }; i < extensions; ++i)
                std::cerr << glGetStringi(GL_EXTENSIONS, i) << " ";
        } else std::cerr << glGetString(GL_EXTENSIONS);
        std::cerr << std::endl; // FIXME: checks above.

        if (context_config.msaa_samples > 0)
            glEnable(GL_MULTISAMPLE);
        else glDisable(GL_MULTISAMPLE);

        glfwSwapInterval(vertical_sync);
        double elapsed_time { glfwGetTime() };
        last_time = fps_timer = elapsed_time;
    }

    Window::~Window() noexcept {
        glfwDestroyWindow(handle);
        handle = nullptr;
    }

    GLFWwindow* Window::get_handle() const {
        return handle;
    }

    void Window::request_context() {
        glfwMakeContextCurrent(handle);
    }

    bool Window::has_context() const {
        return handle == glfwGetCurrentContext();
    }

    void Window::reset_time() {
        glfwSetTime(0.0);
    }

    float Window::time() const {
        return glfwGetTime();
    }

    double Window::delta_time() const {
        return frame_time;
    }

    bool Window::is_open() const {
        return !glfwWindowShouldClose(handle);
    }

    void Window::close() {
        glfwSetWindowShouldClose(handle, GL_TRUE); 
    }

    void Window::display() {
        double elapsed_time { glfwGetTime() };
        frame_time = elapsed_time - last_time;
        last_time = elapsed_time; // Per frame

        if (elapsed_time - fps_timer >= 1.0) {
            std::string fps { std::to_string(frames) };
            current_title = title + " @ " + fps + " FPS";
            glfwSetWindowTitle(handle, current_title.c_str());
            fps_timer = elapsed_time;
            frames = 0;
        }

        glfwSwapBuffers(handle);
        glfwPollEvents();
        ++frames;
    }

    int Window::width() const {
        int value;
        glfwGetWindowSize(handle, &value, nullptr);
        return value;
    }

    int Window::height() const {
        int value;
        glfwGetWindowSize(handle, nullptr, &value);
        return value;
    }

    float Window::vertical_dpi() const {
        int height_in_mm;
        double inch_to_mm { 25.4 };
        glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(),
                                   nullptr, &height_in_mm);
        return fullscreen_width / (height_in_mm/inch_to_mm);
    }

    float Window::horizontal_dpi() const {
        int width_in_mm;
        double inch_to_mm { 25.4 };
        glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(),
                                   &width_in_mm, nullptr);
        return fullscreen_width / (width_in_mm/inch_to_mm);
    }

    float Window::aspect_ratio() const {
        return width() / static_cast<float>(height());
    }

    void Window::resize(std::size_t width, std::size_t height) {
        windowed_height = height;
        windowed_width = width;
        if (!is_fullscreen())
            glfwSetWindowSize(handle, width, height);
    }

    bool Window::is_fullscreen() const {
        return fullscreen;
    }

    void Window::toggle_fullscreen() {
        int x { windowed_x },
            y { windowed_y };
        int width { windowed_width },
            height { windowed_height };

        GLFWmonitor* monitor { glfwGetPrimaryMonitor() };
        if (!is_fullscreen()) {
            x = 0; y = 0;
            width = fullscreen_width;
            height = fullscreen_height;
            glfwGetWindowPos(handle, &windowed_x, &windowed_y);
        } else monitor = nullptr;

        glfwSetWindowMonitor(handle, monitor,
                             x, y, width, height,
                             refresh_rate);
        fullscreen = !fullscreen;
    }
}
