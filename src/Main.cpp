// C++ Standard Library includes
#include <iostream>
#include <format>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <numbers>

// External Libraries includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project includes
#include "Logger.h"
#include "Timer.h"
#include "UnitTests.h"
#include "OpenGlUtils.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Application.h"

// #define TESTING

int main() 
{
    // Create the logger object
    Logger logger;
    loggerPtr = &logger;

    // Run the unit tests
    // LogTimerTest();
#ifndef TESTING
    LOG(LogLevel::INFO, "Hello, World! This is my OpenGL application using GLEW and GLFW.");
    
    // Initialize GLFW
    if (!glfwInit()) 
    {
        LOG(LogLevel::ERROR, "Failed to initialize GLFW");
        return -1;
    }
    // GLSL version '330' is not supported, macOS uses Legacy Profile as default for all created OpenGL context.
    // Therefore by default only OpenGL up to 2.1 and GLSL up to 1.20 is supported
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // This was 2, but 3 seems to work
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLEW Test", NULL, NULL);
    if (!window) 
    {
        LOG(LogLevel::ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Set the key callback function
    glfwSetKeyCallback(window, Application::KeyCallback);

    // Set the glfw user pointer to application state
    Application appState;
    glfwSetWindowUserPointer(window, &appState);

    // Set the current context to the created window
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    // glewExperimental = GL_TRUE; // Enable modern OpenGL functionality
    unsigned int err = glewInit();
    if (err != GLEW_OK) 
    {
        LOG(LogLevel::ERROR, "Failed to initialize GLEW: {}", reinterpret_cast<const char*>(glewGetErrorString(err)));
        glfwTerminate();
        return -1;
    }

    // Print OpenGL version
    LOG(LogLevel::INFO, "OpenGL Version {}", reinterpret_cast<const char*>(glGetString(GL_VERSION))); 

    // OpenGL documentation found at https://docs.gl/

    float positions[] = {-0.5f, -0.5f,
                          0.5f, -0.5f,
                          0.5f,  0.5f,
                         -0.5f,  0.5f}; // 2D vertex positions

    unsigned int indices[] = {0, 1, 2, 
                              2, 3, 0};

    appState.m_positions = positions;
    appState.m_updatePositions = true; // Add a flag to track changes in positions
    appState.m_positionsLength = sizeof(positions) / sizeof(positions[0]);
    appState.m_indices = indices;
    appState.m_indicesLength = sizeof(indices) / sizeof(indices[0]);
    
    {
        VertexArray va;
        // Create a vertex array buffer to put data into
        VertexBuffer vb(appState.m_positions, appState.m_positionsLength * sizeof(appState.m_positions[0]));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Create an index buffer object (IBO) for element array
        IndexBuffer ib(appState.m_indices, appState.m_indicesLength);
        
        // Create shaders
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        va.Unbind(); // Unbind the VAO
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Create renderer object
        Renderer renderer;

        float redChannelColor = 0.0f;
        float increment = 0.05f;

        Timer frameTimer{false};
        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            frameTimer.Start();

            // Clear here
            renderer.Clear();

            // Bind shader and set up the uniforms
            shader.Bind();
            shader.SetUniform4f("u_Color", redChannelColor, 0.3f, 0.8f, 1.0f); // Set the color uniform

            // Update vertex buffer data only if positions have changed
            if (appState.m_updatePositions) 
            {
                vb.UpdateData(appState.m_positions, appState.m_positionsLength * sizeof(appState.m_positions[0]));
                appState.m_updatePositions = false; // Reset the flag after updating
            }
            va.AddBuffer(vb, layout); // Re-add the buffer to update the VAO state

            // Draw the triangle using the shader program
            renderer.Draw(va, ib, shader);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
            
            std::chrono::microseconds frameTime = frameTimer.Stop();
            // Print out the time difference
            LOG(LogLevel::INFO, "Frame time delta = {}us, fps={}", frameTime.count(), 1e6 / frameTime.count());
        }
    }
    LOG(LogLevel::INFO, "GLEW version: {}", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))); 
    glfwTerminate();

    #endif // TESTING
    return 0;
}