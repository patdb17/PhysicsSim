#include <iostream>
#include <format>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "Timing.h"
#include "UnitTests.h"

Logger_t* Logger_t::logPtr = nullptr;

int main() 
{
    // Create the logger object
    Logger_t logger;
    Logger_t::logPtr = &logger;

    // // Run the unit tests
    // LogTimerTest();

    std::cout << std::format("Hello, World! This is a C++23 formatted string.\n");
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLEW Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << "\n"; 
    glfwTerminate();

    return 0;
}