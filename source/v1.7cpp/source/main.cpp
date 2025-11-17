#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <camera.h>
#include <mesh.h>
#include <block.h>
#include <chunk.h>
#include <raycast.h>

void framebuffersizecallback(GLFWwindow* window, int width, int height);
void mousecallback(GLFWwindow* window, double x, double y);
void processinput(GLFWwindow* window);
void mousebuttoncallback(GLFWwindow* window, int button, int action, int mods);

const unsigned int scrwidth = 1080, scrheight = 720;
float dt = 0.0f, cf = 0.0f, lf = 0.0f;

Camera camera;
float lastx = scrwidth / 2.0f;
float lasty = scrheight / 2.0f;
bool firstmouse = true;

bool leftclick = false;
bool rightclick = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(scrwidth, scrheight, "Lumicraft", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
    glfwSetCursorPosCallback(window, mousecallback);
    glfwSetMouseButtonCallback(window, mousebuttoncallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader shader1("../source/shaders/block.vs", "../source/shaders/block.fs");
    Raycaster raycaster;

    loadblocks();
    glm::vec3 pos1 = glm::vec3(0.0f, 0.0f, 0.0f);
    Chunk chunk1(pos1);
    chunks[chunk1.pos] = chunk1;
    chunks[pos1].blocks[0][0][0] = 1;
    chunks[pos1].blocks[0][1][0] = 1;
    chunks[pos1].blocks[0][3][0] = 1;
    chunks[pos1].blocks[3][0][0] = 1;
    chunks[pos1].blocks[0][0][3] = 1;
    for (int i = 5; i < 11; i++) {
        for (int j = 2; j < 9; j++) {
            for (int k = 7; k < 16; k++) {
                chunks[pos1].blocks[i][j][k] = 1;
            }
        }
    }
    chunks[pos1].generatemesh();
    glm::vec3 pos2 = glm::vec3(0.0f, 0.0f, 1.0f);
    Chunk chunk2(pos2);
    chunks[chunk2.pos] = chunk2;
    chunks[pos2].blocks[0][0][0] = 1;
    chunks[pos2].blocks[0][1][0] = 1;
    chunks[pos2].blocks[0][3][0] = 1;
    chunks[pos2].blocks[3][0][0] = 1;
    chunks[pos2].blocks[0][0][3] = 1;
    for (int i = 5; i < 11; i++) {
        for (int j = 2; j < 9; j++) {
            for (int k = 7; k < 16; k++) {
                chunks[pos2].blocks[i][j][k] = 1;
            }
        }
    }
    chunks[pos2].generatemesh();
    glm::vec3 pos3 = glm::vec3(1.0f, 0.0f, 0.0f);
    Chunk chunk3(pos3);
    chunks[chunk3.pos] = chunk3;
    chunks[pos3].blocks[0][0][0] = 1;
    chunks[pos3].blocks[0][1][0] = 1;
    chunks[pos3].blocks[0][3][0] = 1;
    chunks[pos3].blocks[3][0][0] = 1;
    chunks[pos3].blocks[0][0][3] = 1;
    for (int i = 5; i < 11; i++) {
        for (int j = 2; j < 9; j++) {
            for (int k = 7; k < 16; k++) {
                chunks[pos3].blocks[i][j][k] = 1;
            }
        }
    }
    chunks[pos3].generatemesh();
    glm::vec3 pos4 = glm::vec3(1.0f, 0.0f, 1.0f);
    Chunk chunk4(pos4);
    chunks[chunk4.pos] = chunk4;
    chunks[pos4].blocks[0][0][0] = 1;
    chunks[pos4].blocks[0][1][0] = 1;
    chunks[pos4].blocks[0][3][0] = 1;
    chunks[pos4].blocks[3][0][0] = 1;
    chunks[pos4].blocks[0][0][3] = 1;
    for (int i = 5; i < 11; i++) {
        for (int j = 2; j < 9; j++) {
            for (int k = 7; k < 16; k++) {
                chunks[pos4].blocks[i][j][k] = 1;
            }
        }
    }
    chunks[pos4].generatemesh();

    while (!glfwWindowShouldClose(window)) {
        cf = (float)glfwGetTime();
        dt = cf - lf;
        lf = cf;
        rayhit res;
        raycaster.cast(res, camera.pos, camera.front);
        if (res.hit) {
            if (leftclick) {
                std::cout << "click" << std::endl;
                glm::vec3 b = glm::floor(res.block);
                glm::vec3 c = glm::floor(glm::vec3(b) / 16.0f);
                glm::ivec3 l = b - c * 16.0f;
                std::cout << l.x << " " << l.y << " " << l.z << std::endl;
                chunks[c].blocks[l.x][l.y][l.z] = 0;
                chunks[c].generatemesh();
            }
            if (rightclick) {
                std::cout << "click" << std::endl;
                glm::vec3 placepos = res.block + res.normal;
                if (placepos.y < 0 || placepos.y > 15) {
                    std::cout << "too high" << std::endl;
                } else {
                    glm::vec3 p = glm::floor(placepos);
                    glm::vec3 c = glm::floor(glm::vec3(p) / 16.0f);
                    glm::ivec3 l = p - c * 16.0f;
                    std::cout << l.x << " " << l.y << " " << l.z << std::endl;
                    chunks[c].blocks[l.x][l.y][l.z] = 1;
                    chunks[c].generatemesh();
                }
            }
        }
        processinput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model = glm::mat4(1.0f);
        shader1.setmat4("model", model);
        glm::mat4 view = camera.getviewmatrix();
        shader1.setmat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)scrwidth / (float)scrheight, 0.1f, 100.0f);
        shader1.setmat4("projection", projection);
        shader1.use();
        for (auto &chunk : chunks) {
            chunk.second.mesh.draw(shader1);
        }
        //chunk1.mesh.draw(shader1);
        //blockmeshes[0].draw(shader1);
        leftclick = false;
        rightclick = false;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffersizecallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mousecallback(GLFWwindow* window, double x, double y) {
    float nx = (float)x;
    float ny = (float)y;
    if (firstmouse) {
        lastx = nx;
        lasty = ny;
        firstmouse = false;
    }
    float xos = nx - lastx;
    float yos = lasty - ny;
    lastx = nx;
    lasty = ny;
    camera.processmouse(xos, yos);
}

void processinput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processkeyboard(FORWARDS, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processkeyboard(BACKWARDS, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processkeyboard(LEFT, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processkeyboard(RIGHT, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processkeyboard(UP, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.processkeyboard(DOWN, dt);
    }
}

void mousebuttoncallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftclick = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        rightclick = true;
    }
}