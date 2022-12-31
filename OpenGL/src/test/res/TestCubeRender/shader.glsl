#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_color;
out float v_alpha;

uniform mat4 u_MVP;
uniform float u_alpha;

void main()
{
    gl_Position = u_MVP * position;
    v_color = color;
    v_alpha = u_alpha;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in float v_alpha;

void main()
{   
    color = v_color;
    color.a = v_alpha;
}