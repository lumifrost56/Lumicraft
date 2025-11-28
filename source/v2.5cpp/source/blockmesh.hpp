#ifndef BLOCKMESH_HPP
#define BLOCKMESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <wrapper.hpp>
#include <utils.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <string>
#include <vector>

unsigned int block_tex_array = 0;

Shader blockmesh_shader;

void create_blockmesh_shader() {
    std::string blockmesh_vertex_path = "../source/shaders/blockmesh.vrs";
    std::string blockmesh_fragment_path = "../source/shaders/blockmesh.frs";
    blockmesh_shader.create_self(blockmesh_vertex_path, blockmesh_fragment_path);
}

struct Blockmesh {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Blockmesh() {}

    void draw(glm::vec3 pos, Camera &camera) {
        blockmesh_shader.use();

        blockmesh_shader.set_int("block_tex_array", 0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        blockmesh_shader.set_mat4("model", model);

        glm::mat4 view = camera.get_view_mat();
        blockmesh_shader.set_mat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspect_ratio, 0.1f, 100.0f);
        blockmesh_shader.set_mat4("projection", projection);

        set_active_texture(0);
        bind_tex_array(block_tex_array);

        bind_vertex_array(VAO);

        set_line_width(3.0f);
        draw_triangles(indices.size());
    }

    void create_self() {
        generate_vertex_array(VAO);

        generate_buffer(VBO);
        generate_buffer(EBO);

        bind_vertex_array(VAO);

        bind_array_buffer(VBO);
        set_array_buffer_data(vertices);

        bind_element_array_buffer(EBO);
        set_element_array_buffer_data(indices);

        enable_vertex_attrib_array(0);
        set_vertex_attrib_pointer(0, 3, offsetof(Vertex, pos));

        enable_vertex_attrib_array(1);
        set_vertex_attrib_pointer(1, 3, offsetof(Vertex, normal));

        enable_vertex_attrib_array(2);
        set_vertex_attrib_pointer(2, 2, offsetof(Vertex, uv));

        enable_vertex_attrib_array(3);
        set_vertex_attrib_pointer(3, 1, offsetof(Vertex, tex_id));
    }

    void delete_self() {
        delete_buffer(VBO);
        delete_buffer(EBO);
        
        delete_vertex_array(VAO);
    }
};

#endif