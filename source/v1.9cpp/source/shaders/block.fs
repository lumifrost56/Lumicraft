#version 330 core
out vec4 FragColor;

in vec2 ntexcoor;
in float ntexid;

uniform sampler2DArray blocktexarray;

void main() {
    FragColor = texture(blocktexarray, vec3(ntexcoor, ntexid));
}