#include "Renderer.h"

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char*        functionName, 
               const char*        fileName, 
               const unsigned int lineNumber)
{
    while (unsigned int error = glGetError())
    {
        // Log the error immediately using the Logger class's formatting and bypass the queue
        PrintMessageNow(LogLevel::ERROR, fileName, lineNumber, 
                std::format("OpenGL Error Logged: Function call - {} : Error - {}", functionName, GetGlErrorString(error)));

        return false; // Return false if an error occurred
    }
    return true; // Return true if no errors occurred
}