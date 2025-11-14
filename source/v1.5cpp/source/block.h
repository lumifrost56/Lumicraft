#ifndef BLOCK_H
#define BLOCK_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mesh.h>
#include <string>
#include <vector>

std::vector<Vertex> stdvertices = {
    // Front (+Z)
    {{-0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}, 0},
    {{-0.5f, 0.5f, 0.5f}, {0,0,1}, {0,1}, 0},
    {{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1}, 0},
    {{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {1,0}, 0},
    // Back (-Z)
    {{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}, 1},
    {{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,1}, 1},
    {{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1}, 1},
    {{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,0}, 1},
    // Left (-X)
    {{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}, 2},
    {{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,1}, 2},
    {{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1}, 2},
    {{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,0}, 2},
    // Right (+X)
    {{ 0.5f,-0.5f, 0.5f}, {1,0,0}, {0,0}, 3},
    {{ 0.5f, 0.5f, 0.5f}, {1,0,0}, {0,1}, 3},
    {{ 0.5f, 0.5f,-0.5f}, {1,0,0}, {1,1}, 3},
    {{ 0.5f,-0.5f,-0.5f}, {1,0,0}, {1,0}, 3},
    // Top (+Y)
    {{-0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}, 4},
    {{-0.5f, 0.5f,-0.5f}, {0,1,0}, {0,1}, 4},
    {{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1}, 4},
    {{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {1,0}, 4},
    // Bottom (-Y)
    {{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}, 5},
    {{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,1}, 5},
    {{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1}, 5},
    {{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,0}, 5},
};

std::vector<unsigned int> stdindices = {
    0,1,2, 0,2,3,       // Front
    4,5,6, 4,6,7,       // Back
    8,9,10, 8,10,11,    // Left
    12,13,14, 12,14,15, // Right
    16,17,18, 16,18,19, // Top
    20,21,22, 20,22,23  // Bottom
};

void generateblock(std::vector<Vertex> &vertices,
                   std::vector<unsigned int> &indices,
                   std::vector<Texture> &textures,
                   std::vector<unsigned int> texids) {
    vertices = stdvertices;
    indices = stdindices;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            vertices[i*4 + j].texid = (float)texids[i];
        }
    }
}
#endif