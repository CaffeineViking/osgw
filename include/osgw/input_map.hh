#ifndef OSGW_INPUT_MAP_HH
#define OSGW_INPUT_MAP_HH

#include <osgw/window.hh>
#include <osgw/input.hh>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace osgw {
    class InputMapper final {
    public:
        InputMapper(const Window& window);

        void unmap(const std::string& id);

        void map(const std::string& id, Input::Key key);
        void map(const std::string& id, const std::vector<Input::Key>& keys);
        void map(const std::string& id, Input::MouseButton mouse_button);
        void map(const std::string& id, const std::vector<Input::MouseButton>& mouse_buttons);

        std::vector<Input::MouseButton> get_mapped_mouse_buttons(const std::string& id) const;
        std::vector<Input::Key> get_mapped_keys(const std::string& id) const;

        bool pressed(Input::Key key) const;
        bool just_pressed(Input::Key key);
        bool released(Input::Key key) const;
        bool pressed(Input::MouseButton key) const;
        bool just_pressed(Input::MouseButton key);
        bool released(Input::MouseButton key) const;

        bool pressed(const std::string& id) const;
        bool just_pressed(const std::string& id);
        bool released(const std::string& id) const;

        glm::vec2 mouse_position() const;

        void toggle_mouse_lock();
        bool is_mouse_locked() const;

    private:
        // FIXME: Since we can't pass around private member functions
        // to the GLFW callback functions, we do a hack and map these
        // window pointers to their specific input mappers on create.
        //
        static void mouse_button_callback(GLFWwindow*, int, int, int);
        static std::unordered_map<GLFWwindow*, InputMapper*> callback_map;
        static void key_callback(GLFWwindow*, int, int, int, int);

        std::unordered_map<Input::Key, Input::State> key_state_map;
        std::unordered_map<Input::MouseButton, Input::State> mouse_button_state_map;
        std::unordered_multimap<std::string, Input::MouseButton> mouse_button_map;
        std::unordered_multimap<std::string, Input::Key> key_map;

        bool mouse_locked { false };
        GLFWwindow* handle;
    };
}

#endif
