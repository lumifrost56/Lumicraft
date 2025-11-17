#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgloader/stb_image.h>
#include <shader.h>
#include <string>
#include <vector>

unsigned int blocktexarray;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcoor;
    float texid;
};

std::vector<Vertex> stdvertices = {
    // Front (+Z)
    {{0.0f, 0.0f, 1.0f}, {0,0,1}, {0,0}, 0},
    {{0.0f, 1.0f, 1.0f}, {0,0,1}, {0,1}, 0},
    {{1.0f, 1.0f, 1.0f}, {0,0,1}, {1,1}, 0},
    {{1.0f, 0.0f, 1.0f}, {0,0,1}, {1,0}, 0},
    // Back (-Z)
    {{1.0f, 0.0f, 0.0f}, {0,0,-1}, {0,0}, 1},
    {{1.0f, 1.0f, 0.0f}, {0,0,-1}, {0,1}, 1},
    {{0.0f, 1.0f, 0.0f}, {0,0,-1}, {1,1}, 1},
    {{0.0f, 0.0f, 0.0f}, {0,0,-1}, {1,0}, 1},
    // Left (-X)
    {{0.0f, 0.0f, 0.0f}, {-1,0,0}, {0,0}, 2},
    {{0.0f, 1.0f, 0.0f}, {-1,0,0}, {0,1}, 2},
    {{0.0f, 1.0f, 1.0f}, {-1,0,0}, {1,1}, 2},
    {{0.0f, 0.0f, 1.0f}, {-1,0,0}, {1,0}, 2},
    // Right (+X)
    {{1.0f, 0.0f, 1.0f}, {1,0,0}, {0,0}, 3},
    {{1.0f, 1.0f, 1.0f}, {1,0,0}, {0,1}, 3},
    {{1.0f, 1.0f, 0.0f}, {1,0,0}, {1,1}, 3},
    {{1.0f, 0.0f, 0.0f}, {1,0,0}, {1,0}, 3},
    // Top (+Y)
    {{0.0f, 1.0f, 1.0f}, {0,1,0}, {0,0}, 4},
    {{0.0f, 1.0f, 0.0f}, {0,1,0}, {0,1}, 4},
    {{1.0f, 1.0f, 0.0f}, {0,1,0}, {1,1}, 4},
    {{1.0f, 1.0f, 1.0f}, {0,1,0}, {1,0}, 4},
    // Bottom (-Y)
    {{0.0f, 0.0f, 0.0f}, {0,-1,0}, {0,0}, 5},
    {{0.0f, 0.0f, 1.0f}, {0,-1,0}, {0,1}, 5},
    {{1.0f, 0.0f, 1.0f}, {0,-1,0}, {1,1}, 5},
    {{1.0f, 0.0f, 0.0f}, {0,-1,0}, {1,0}, 5}
};

std::vector<unsigned int> stdindices = {
    0,1,2, 0,2,3,       // Front
    4,5,6, 4,6,7,       // Back
    8,9,10, 8,10,11,    // Left
    12,13,14, 12,14,15, // Right
    16,17,18, 16,18,19, // Top
    20,21,22, 20,22,23  // Bottom
};

class Blockmesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO;

    Blockmesh() {}

    Blockmesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        this->vertices = vertices;
        this->indices = indices;
        setupmesh();
    }

    void draw(Shader &shader) {
        shader.setint("blocktexarray", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, blocktexarray);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    }
private:
    unsigned int VBO, EBO;

    void setupmesh() {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoor));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texid));
    }
};

void loadblocktextures(std::vector<std::string> texturepaths) {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &blocktexarray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, blocktexarray);

    int width, height, nchannels;
    const int layers = texturepaths.size();

    unsigned char *data = stbi_load(texturepaths[0].c_str(), &width, &height, &nchannels, 4);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    for (int i = 1; i < layers; i++) {
        unsigned char *data = stbi_load(texturepaths[i].c_str(), &width, &height, &nchannels, 4);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

std::vector<Blockmesh> blockmeshes;

void generateblock(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<unsigned int> texids) {
    vertices = stdvertices;
    indices = stdindices;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            vertices[i*4+j].texid = (float)texids[i];
        }
    }
}

void loadblockmeshes() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    generateblock(vertices, indices, {0, 0, 0, 0, 1, 1});

    Blockmesh oaklog(vertices, indices);

    blockmeshes.push_back(oaklog);
}
#endif