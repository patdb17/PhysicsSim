// C++ Standard Library includes
#include <iostream>
#include <format>
#include <string>
#include <fstream>
#include <sstream>

// External Libraries includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project includes
#include "Logger.h"
#include "Timing.h"
#include "UnitTests.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
    
    {
        VertexArray va;
        // Create a vertex array buffer to put data into
        VertexBuffer vb(positions, 4 * 2 * sizeof(positions[0]));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Create an index buffer object (IBO) for element array
        IndexBuffer ib(indices, 6);
        
        // Create shaders
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        va.Unbind(); // Unbind the VAO
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float redChannelColor = 0.0f;
        float increment = 0.05f;
        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Clear here
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

            // Bind shader and set up the uniforms
            shader.Bind();
            shader.SetUniform4f("u_Color", redChannelColor, 0.3f, 0.8f, 1.0f); // Set the color uniform

            // Bind the Vertex Array Object (VAO)
            va.Bind();

            // Bind the index buffer
            ib.Bind();

            // Draw the triangle using the shader program
            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            // Update the color value for the next frame
            if (redChannelColor > 1.0f)
            {
                increment = -0.05f; // Reverse direction
            }
            else if (redChannelColor < 0.0f)
            {
                increment = 0.05f; // Reverse direction
            }
            redChannelColor += increment; // Update the red component

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }
    }
    LOG(LogLevel::INFO, "GLEW version: {}", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))); 
    glfwTerminate();

    return 0;
}