#version 330 core
out vec4 FragColor;

in vec2 ntexcoor;
in float ntexid;

uniform sampler2DArray texarray;

void main() {
    FragColor = texture(texarray, vec3(ntexcoor, ntexid));
}