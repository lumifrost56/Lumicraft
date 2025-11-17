#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoor;
layout (location = 3) in float texid;

out vec2 ntexcoor;
out float ntexid;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    ntexcoor = texcoor;
    ntexid = texid;
}