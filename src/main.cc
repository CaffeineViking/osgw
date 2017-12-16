#include <lwss/window.hh>

int main(int, char**) {
    lwss::Window window { 1280, 720, "LWSS" };
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    while (window.is_open()) {
        glClear(GL_COLOR_BUFFER_BIT);
        window.display();
    }
}
