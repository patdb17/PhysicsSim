#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//attribute vec4 position;
void main() 
{
    gl_Position = position; // Pass the vertex position to the fragment shader
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color; // Uniform variable for color

void main() 
{
    color = u_Color; // Set the fragment color
}
