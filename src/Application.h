#ifndef APPLICATION_H
#define APPLICATION_H
// Application.h

#include <optional>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef enum
{
    KEY_ACTION_PRESS    = GLFW_PRESS,
    KEY_ACTION_RELEASE  = GLFW_RELEASE
} KeyAction;

class Application
{
    public:
        bool m_updatePositions = false;
        float* m_positions = nullptr;
        unsigned int m_positionsLength = 0;

        unsigned int* m_indices = nullptr;
        unsigned int m_indicesLength = 0;

        // void SetPositions(float* const positions, const unsigned int positionsLength);
        // void SetIndices(unsigned int* const indices, const unsigned int indicesLength);

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        std::optional<std::string> HandleUpKey      (const KeyAction action = KEY_ACTION_PRESS);
        std::optional<std::string> HandleDownKey    (const KeyAction action = KEY_ACTION_PRESS);
        std::optional<std::string> HandleLeftKey    (const KeyAction action = KEY_ACTION_PRESS);
        std::optional<std::string> HandleRightKey   (const KeyAction action = KEY_ACTION_PRESS);

    private:

};

#endif // APPLICATION_H