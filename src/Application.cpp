#include "Application.h"
#include "Logger.h"

// void Application::SetPositions(float* const positions, const unsigned int positionsLength)
// { 
//     m_positions = positions;
//     m_positionsLength = positionsLength;
//     return;
// }

// void Application::SetIndices(unsigned int* const indices, const unsigned int indicesLength)
// {
//     m_indices = indices;
//     m_indicesLength = indicesLength;
//     return;
// }

void Application::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Application* state = static_cast<Application*>(glfwGetWindowUserPointer(window));

    switch (key)
    {
        case GLFW_KEY_UP:
            state->HandleUpKey(static_cast<KeyAction> (action));
            break;

        case GLFW_KEY_DOWN:
            state->HandleDownKey(static_cast<KeyAction> (action));
            break;

        case GLFW_KEY_LEFT:
            state->HandleLeftKey(static_cast<KeyAction> (action));
            break;

        case GLFW_KEY_RIGHT:
            state->HandleRightKey(static_cast<KeyAction> (action));
            break;

        default:
            break;
    }
}

std::optional<std::string> Application::HandleUpKey(const KeyAction action)
{
    switch (action)
    {
        case KEY_ACTION_PRESS:
            // Move all y-positions up by 0.1 units
            for (unsigned int i = 1; i < m_positionsLength; i += 2)
            {
                m_positions[i] += 0.1f; // Move up by 0.1 units
                if (m_positions[i] > 1.0f) // Wrap around if it goes off-screen
                {
                    m_positions[i] = -1.0f + (m_positions[i] - 1.0f); // Wrap around to the bottom
                }

                LOG(LogLevel::DEBUG, "Position {}: ({}, {})", i / 2, m_positions[i - 1], m_positions[i]);
            }
            m_updatePositions = true; // Set the flag to indicate positions have changed
            break;

        case KEY_ACTION_RELEASE:
            // Handle key release if needed
            break;

        default:
            break;
    }

    return std::nullopt;
}

std::optional<std::string> Application::HandleDownKey(const KeyAction action)
{
    switch (action)
    {
        case KEY_ACTION_PRESS:
            // Move all y-positions down by 0.1 units
            for (unsigned int i = 1; i < m_positionsLength; i += 2)
            {
                m_positions[i] -= 0.1f; // Move down by 0.1 units
                if (m_positions[i] < -1.0f) // Wrap around if it goes off-screen
                {
                    m_positions[i] = 1.0f - (-1.0f - m_positions[i]); // Wrap around to the top
                }

                LOG(LogLevel::DEBUG, "Position {}: ({}, {})", i / 2, m_positions[i - 1], m_positions[i]);
            }
            m_updatePositions = true; // Set the flag to indicate positions have changed
            break;
            
        case KEY_ACTION_RELEASE:
            // Handle key release if needed
            break;
        default:
            break;
    }
    return std::nullopt;
}

std::optional<std::string> Application::HandleLeftKey(const KeyAction action)
{
    switch (action)
    {
        case KEY_ACTION_PRESS:
            // Move all x-positions left by 0.1 units
            for (unsigned int i = 0; i < m_positionsLength; i += 2)
            {
                m_positions[i] -= 0.1f; // Move left by 0.1 units
                if (m_positions[i] < -1.0f) // Wrap around if it goes off-screen
                {
                    m_positions[i] = 1.0f - (-1.0f - m_positions[i]); // Wrap around to the right
                }

                LOG(LogLevel::DEBUG, "Position {}: ({}, {})", i / 2, m_positions[i], m_positions[i+1]);
            }
            m_updatePositions = true; // Set the flag to indicate positions have changed
            break;

        case KEY_ACTION_RELEASE:
            // Handle key release if needed
            break;
        default:
            break;
    }
    return std::nullopt;
}

std::optional<std::string> Application::HandleRightKey(const KeyAction action)
{
    switch (action)
    {
        case KEY_ACTION_PRESS:
            // Move all x-positions right by 0.1 units
            for (unsigned int i = 0; i < m_positionsLength; i += 2)
            {
                m_positions[i] += 0.1f; // Move right by 0.1 units
                if (m_positions[i] > 1.0f) // Wrap around if it goes off-screen
                {
                    m_positions[i] = -1.0f + (m_positions[i] - 1.0f); // Wrap around to the left
                }

                LOG(LogLevel::DEBUG, "Position {}: ({}, {})", i / 2, m_positions[i], m_positions[i+1]);
            }
            m_updatePositions = true; // Set the flag to indicate positions have changed
            break;
            
        case KEY_ACTION_RELEASE:
            // Handle key release if needed
            break;
        default:
            break;
    }
    return std::nullopt;
}
