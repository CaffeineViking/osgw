#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

static bool VERTICAL_SYNC { true };
static std::string TITLE { "LWSS" };
static int WIDTH { 1280 }, HEIGHT { 720 };

int main(int, char**) {
    if (!glfwInit())
        return -1;

    std::cerr << "GLFW version: " << glfwGetVersionString() << std::endl;

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(),
                                          nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gl3wInit()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    if (!gl3wIsSupported(3, 3)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cerr << "GL3W loader support for OpenGL 3.3 (core profile) was found!" << std::endl;
    std::cerr << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cerr << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "OpenGL GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    int extensions;
    std::cerr << "OpenGL extensions: ";
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
    for (int i { 0 }; i < extensions - 1; ++i)
        std::cerr << glGetStringi(GL_EXTENSIONS, i) << ", ";
    std::cerr << glGetStringi(GL_EXTENSIONS, extensions - 1) << std::endl;;

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    std::size_t frames { 0 };
    double previous_frame { glfwGetTime() };

    glfwSwapInterval(VERTICAL_SYNC);
    while (!glfwWindowShouldClose(window)) {
        // Finds out the current framerate.
        double current_frame { glfwGetTime() };
        if (current_frame - previous_frame >= 1.0) {
            std::string title { TITLE + " @ " };
            title += std::to_string(frames) + " FPS";
            glfwSetWindowTitle(window, title.c_str());
            previous_frame = current_frame;
            frames = 0;
        }

        // Handle the case where the user wants to exit.
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        ++frames;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
