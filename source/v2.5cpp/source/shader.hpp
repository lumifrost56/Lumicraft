#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <wrapper.hpp>
#include <string>
#include <fstream>
#include <sstream>

struct Shader {
    unsigned int id = 0;

    Shader() {}

    void create_self(std::string vertex_path, std::string fragment_path) {
        std::string vertex_code;
        std::string fragment_code;

        std::ifstream vertex_file;
        std::ifstream fragment_file;

        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);

        std::stringstream vertex_stream;
        std::stringstream fragment_stream;

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();

        unsigned int vertex = 0;
        unsigned int fragment = 0;

        create_vertex_shader(vertex, vertex_code.c_str());
        create_fragment_shader(fragment, fragment_code.c_str());

        create_shader_program(id);

        attach_shader(id, vertex);
        attach_shader(id, fragment);

        link_shader_program(id);

        delete_shader(vertex);
        delete_shader(fragment);
    }

    void use() {
        use_shader_program(id);
    }

    void set_bool(std::string name, bool value) {
        set_uniform_1i(id, name.c_str(), (int)value);
    }

    void set_int(std::string name, int value) {
        set_uniform_1i(id, name.c_str(), value);
    }

    void set_float(std::string name, float value) {
        set_uniform_1f(id, name.c_str(), value);
    }

    void set_vec2(std::string name, glm::vec2 value) {
        set_uniform_2fv(id, name.c_str(), value);
    }

    void set_vec3(std::string name, glm::vec3 value) {
        set_uniform_3fv(id, name.c_str(), value);
    }

    void set_vec4(std::string name, glm::vec4 value) {
        set_uniform_4fv(id, name.c_str(), value);
    }

    void set_mat2(std::string name, glm::mat2 value) {
        set_uniform_matrix2fv(id, name.c_str(), value);
    }

    void set_mat3(std::string name, glm::mat3 value) {
        set_uniform_matrix3fv(id, name.c_str(), value);
    }

    void set_mat4(std::string name, glm::mat4 value) {
        set_uniform_matrix4fv(id, name.c_str(), value);
    }
};

#endif