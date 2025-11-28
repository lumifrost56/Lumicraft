#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <imgloader/stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <wrapper.hpp>
#include <utils.hpp>
#include <blockmesh.hpp>
#include <string>
#include <vector>

std::vector<Blockmesh> block_meshes;

enum block_ids_enum {
    AIR,
    STONE,
    TUFF,
    ICE,
    OAKLOG,
    OAKLOGZ,
    OAKLOGX
};

std::vector<std::string> block_tex_paths = {
    "../resources/images/stone.png",
    "../resources/images/tuff.png",
    "../resources/images/ice.png",
    "../resources/images/oaklogside.png",
    "../resources/images/oaklogtop.png"
};

std::vector<std::vector<int>> block_tex_data = {
    {0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2},
    {3, 3, 3, 3, 4, 4}
};

void load_block_textures() {
    stbi_set_flip_vertically_on_load(true);

    generate_texture(block_tex_array);
    bind_tex_array(block_tex_array);

    int width = 16;
    int height = 16;
    int n_channels = 4;
    int layers = block_tex_paths.size();

    set_tex_image_3D(width, height, layers);

    for (int i = 0; i < layers; i++) {
        unsigned char *data = stbi_load(block_tex_paths[i].c_str(), &width, &height, &n_channels, 4);

        set_tex_sub_image_3D(i, width, height, data);

        stbi_image_free(data);
    }

    set_tex_array_min_filter_near_lin();
    set_tex_array_mag_filter_near();

    set_tex_array_wrap_s_repeat();
    set_tex_array_wrap_t_repeat();

    generate_mipmap_tex_array();
}

std::vector<Vertex> std_block_vertices = {
    // +z
    {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0.0f},
    {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, 0.0f},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0.0f},
    {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 0.0f},
    // -z
    {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, 0.0f},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, 0.0f},
    {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, 0.0f},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, 0.0f},
    // -x
    {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
    {{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},
    {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},
    {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},
    // +x
    {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},
    {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},
    // +y
    {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
    {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0.0f},
    // -y
    {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
    {{0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, 0.0f},
    {{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, 0.0f},
    {{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, 0.0f}
};

std::vector<unsigned int> std_block_indices = {
    0,1,2, 0,2,3,
    4,5,6, 4,6,7,
    8,9,10, 8,10,11,
    12,13,14, 12,14,15,
    16,17,18, 16,18,19,
    20,21,22, 20,22,23
};

void generate_block(std::vector<Vertex> &vertices,
                    std::vector<unsigned int> &indices,
                    std::vector<int> tex_ids) {
    vertices = std_block_vertices;
    indices = std_block_indices;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            vertices[i * 4 + j].tex_id = (float)tex_ids[i];
        }
    }
}

void create_block_meshes() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < block_tex_data.size(); i++) {
        generate_block(vertices, indices, block_tex_data[i]);

        Blockmesh block_mesh;

        block_mesh.vertices = vertices;
        block_mesh.indices = indices;

        block_meshes.push_back(block_mesh);
    }
}

#endif