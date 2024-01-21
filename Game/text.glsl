#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec4 v_color;
out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_color = color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = v_color;
    v_color.a = texture(u_Texture, v_TexCoord).r;
    color = texColor * u_Color;
}