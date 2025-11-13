#version 330 core
out vec4 FragColor;

in vec2 ntexcoor;
in float ntexid;

uniform int nt;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main() {
    if (ntexid == 1) {
        FragColor = texture(texture1, ntexcoor);
    } else if (ntexid == 2) {
        FragColor = texture(texture2, ntexcoor);
    } else if (ntexid == 3) {
        FragColor = texture(texture3, ntexcoor);
    }
}