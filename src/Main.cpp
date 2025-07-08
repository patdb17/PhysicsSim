#include <iostream>
#include <format>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "Timing.h"
#include "UnitTests.h"


int main() 
{
    // Create the logger object
    Logger logger;
    loggerPtr = &logger;
    LOG(LogLevel::WARNING, "Starting Logging...");

    // Run the unit tests
    LogTimerTest();

    LOG(LogLevel::INFO, "Hello, World! This is my OpenGL application using GLEW and GLFW.");
    if (!glfwInit()) {
        LOG(LogLevel::ERROR, "Failed to initialize GLFW");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLEW Test", NULL, NULL);
    if (!window) {
        LOG(LogLevel::ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(LogLevel::ERROR, "Failed to initialize GLEW: {}", GlewStrToStrView(glewGetErrorString(err)));
        glfwTerminate();
        return -1;
    }

    LOG(LogLevel::INFO, "GLEW version: {}", GlewStrToStrView(glewGetString(GLEW_VERSION))); 
    glfwTerminate();

    return 0;
}