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

    // Run the unit tests
    //LogTimerTest();

    LOG(LogLevel::INFO, "Hello, World! This is my OpenGL application using GLEW and GLFW.");
    
    // Initialize GLFW
    if (!glfwInit()) 
    {
        LOG(LogLevel::ERROR, "Failed to initialize GLFW");
        return -1;
    }


    GLFWwindow* window = glfwCreateWindow(800, 600, "GLEW Test", NULL, NULL);
    if (!window) 
    {
        LOG(LogLevel::ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Set the current context to the created window
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // glewExperimental = GL_TRUE; // Enable modern OpenGL functionality
    GLenum err = glewInit();
    if (err != GLEW_OK) 
    {
        LOG(LogLevel::ERROR, "Failed to initialize GLEW: {}", GlewStrToStrView(glewGetErrorString(err)));
        glfwTerminate();
        return -1;
    }

    // OpenGL documentation found at https://docs.gl/

    float positions[6] = {-0.5f, -0.5f,
                           0.0f,  0.5f,
                           0.5f, -0.5f }; // 2D vertex positions

    // Create a GL buffer to put data into
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); // Enable the vertex attribute at index 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

    }

    LOG(LogLevel::INFO, "GLEW version: {}", GlewStrToStrView(glewGetString(GLEW_VERSION))); 
    glfwTerminate();

    return 0;
}