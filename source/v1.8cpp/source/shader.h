#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int id;

    Shader(const char *vertexpath, const char *fragmentpath) {
        std::string vertexcode, fragmentcode;

        std::ifstream vsfile, fsfile;
        vsfile.open(vertexpath);
        fsfile.open(fragmentpath);

        std::stringstream vsstream, fsstream;
        vsstream << vsfile.rdbuf();
        fsstream << fsfile.rdbuf();

        vsfile.close();
        fsfile.close();

        vertexcode = vsstream.str();
        fragmentcode = fsstream.str();

        const char *vscode = vertexcode.c_str();
        const char *fscode = fragmentcode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vscode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fscode, NULL);
        glCompileShader(fragment);

        id = glCreateProgram();

        glAttachShader(id, vertex);
        glAttachShader(id, fragment);

        glLinkProgram(id);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(id);
    }

    void setbool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    void setint(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    void setfloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    void setvec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setvec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setvec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setmat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setmat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setmat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
#endif