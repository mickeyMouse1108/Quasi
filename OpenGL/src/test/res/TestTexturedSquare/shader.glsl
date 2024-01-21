#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
out vec4 v_color;
out vec2 v_texCoord;
uniform mat4 u_projection;
uniform mat4 u_view;
void main(){
    gl_Position = u_projection * u_view * position;
    v_color = color;
    v_texCoord = texCoord;
}
#shader fragment
#version 330 core
layout(location = 0) out vec4 color;
in vec4 v_color;
in vec2 v_texCoord;
uniform sampler2D u_font;
void main(){
    color = v_color;
    color.a = texture(u_font, v_texCoord);
}