#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader.hpp>
#include <vector>

class Crosshair {
public:
    unsigned int VAO;

    std::vector<float> crosshair_vertices;

    float px, py;

    Crosshair(int width, int height) {
        px = 21.0f / width;
        py = 21.0f / height;

        crosshair_vertices = {-px, 0.0f, px, 0.0f, 0.0f, -py, 0.0f, py};

        setup();
    }

    void draw(Shader &shader) {
        shader.use();

        glBindVertexArray(VAO);

        glLineWidth(3.0f);

        glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_LINES, 2, 2);

        glLineWidth(1.0f);
    }

private:
    unsigned int VBO;

    void setup() {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (unsigned int)crosshair_vertices.size() * sizeof(float), &crosshair_vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }
};

#endif