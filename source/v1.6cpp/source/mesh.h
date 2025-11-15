#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgloader/stb_image.h>
#include <shader.h>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcoor;
    float texid;
};

struct Texture {
    unsigned int id;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    Mesh() {
        vertices = {};
        indices = {};
        textures = {};
    }
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool load) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        if (load) {
            loadtextures();
        }
        setupmesh();
    }
    void draw(Shader &shader) {
        shader.setint("nt", textures.size());
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            shader.setint("texture" + std::to_string(i + 1), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
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
    void loadtextures() {
        stbi_set_flip_vertically_on_load(true);
        for (unsigned int i = 0; i < textures.size(); i++) {
            glGenTextures(1, &textures[i].id);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            int width, height, nchannels;
            std::string path = textures[i].path;
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nchannels, 0);
            if (data) {
                if (nchannels == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                } else if (nchannels == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
        }
    }
};
#endif