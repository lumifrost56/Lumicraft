#ifndef BLOCK_H
#define BLOCK_H

#include <imgloader/stb_image.h>
#include <mesh.hpp>
#include <string>
#include <vector>

std::vector<Mesh> block_meshes;

std::vector<std::string> block_tex_paths = {
    "../resources/images/oaklogside.png",
    "../resources/images/oaklogtop.png",
    "../resources/images/stone.png",
    "../resources/images/ice.png",
    "../resources/images/tuff.png"
};

void load_block_textures() {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &block_tex_array);
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_tex_array);

    int width, height, n_channels;
    int layers = block_tex_paths.size();

    unsigned char *data = stbi_load(block_tex_paths[0].c_str(), &width, &height, &n_channels, 4);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    for (int i = 1; i < layers; i++) {
        unsigned char *data = stbi_load(block_tex_paths[i].c_str(), &width, &height, &n_channels, 4);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

std::vector<Vertex> std_vertices = {
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

std::vector<unsigned int> std_indices = {
    0,1,2, 0,2,3,       // Front
    4,5,6, 4,6,7,       // Back
    8,9,10, 8,10,11,    // Left
    12,13,14, 12,14,15, // Right
    16,17,18, 16,18,19, // Top
    20,21,22, 20,22,23  // Bottom
};

void generate_block(std::vector<Vertex> &vertices,
                    std::vector<unsigned int> &indices,
                    std::vector<unsigned int> tex_ids) {
    vertices = std_vertices;
    indices = std_indices;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            vertices[i*4+j].tex_id = (float)tex_ids[i];
        }
    }
}

void load_block_meshes() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    generate_block(vertices, indices, {0, 0, 0, 0, 1, 1});
    Mesh oaklog(vertices, indices);
    block_meshes.push_back(oaklog);

    generate_block(vertices, indices, {2, 2, 2, 2, 2, 2});
    Mesh stone(vertices, indices);
    block_meshes.push_back(stone);

    generate_block(vertices, indices, {3, 3, 3, 3, 3, 3});
    Mesh ice(vertices, indices);
    block_meshes.push_back(ice);

    generate_block(vertices, indices, {4, 4, 4, 4, 4, 4});
    Mesh tuff(vertices, indices);
    block_meshes.push_back(tuff);
}

#endif