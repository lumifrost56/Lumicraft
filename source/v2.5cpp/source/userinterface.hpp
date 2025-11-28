#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <wrapper.hpp>
#include <settings.hpp>
#include <utils.hpp>
#include <shader.hpp>
#include <string>
#include <vector>

Shader crosshair_shader;

void create_ui_shaders() {
    std::string crosshair_vertex_path = "../source/shaders/crosshair.vrs";
    std::string crosshair_fragment_path = "../source/shaders/crosshair.frs";
    crosshair_shader.create_self(crosshair_vertex_path, crosshair_fragment_path);
}

struct Crosshair {
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    std::vector<Vertex> vertices;

    float px = 21.0f / (float)screen_width;
    float py = 21.0f / (float)screen_height;

    Crosshair() {
        vertices = {
            {{-px, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
            {{px, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
            {{0.0f, -py, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
            {{0.0f, py, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f}
        };
    }

    void draw() {
        if (px != 21.0f / (float)screen_width || py != 21.0f / (float)screen_height) {
            px = 21.0f / (float)screen_width;
            py = 21.0f / (float)screen_height;

            vertices = {
                {{-px, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
                {{px, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
                {{0.0f, -py, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f},
                {{0.0f, py, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f}
            };

            delete_self();
            create_self();
        }
        
        crosshair_shader.use();

        bind_vertex_array(VAO);

        set_line_width(3.0f);
        draw_line(0, 2);
        draw_line(2, 2);
    }

    void create_self() {
        generate_vertex_array(VAO);

        generate_buffer(VBO);

        bind_vertex_array(VAO);

        bind_array_buffer(VBO);
        set_array_buffer_data(vertices);

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

        delete_vertex_array(VAO);
    }
};

struct UI {
    Crosshair crosshair;

    UI() {}

    void draw() {
        crosshair.draw();
    }

    void create_self() {
        crosshair.create_self();
    }
};

#endif