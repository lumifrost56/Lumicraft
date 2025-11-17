#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <camera.h>
#include <blockmesh.h>
#include <chunk.h>
#include <raycaster.h>
#include <string>
#include <vector>

void framebuffersizecallback(GLFWwindow *window, int width, int height);
void mousebuttoncallback(GLFWwindow *window, int button, int action, int mods);
void cursorposcallback(GLFWwindow *window, double x, double y);
void processinput(GLFWwindow *window);

const unsigned int scrwidth = 1080;
const unsigned int scrheight = 720;

float dt = 0.0f;
float cf = 0.0f;
float lf = 0.0f;

float lastx = scrwidth / 2.0f;
float lasty = scrheight / 2.0f;
bool firstmouse = true;

bool leftclick = false;
bool rightclick = true;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(scrwidth, scrheight, "Lumicraft", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
    glfwSetMouseButtonCallback(window, mousebuttoncallback);
    glfwSetCursorPosCallback(window, cursorposcallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    const char *blockvspath = "../source/shaders/block.vs";
    const char *blockfspath = "../source/shaders/block.fs";
    Shader blockshader(blockvspath, blockfspath);

    std::vector<std::string> texturepaths = {
        "../resources/images/oaklogside.png",
        "../resources/images/oaklogtop.png",
        "../resources/images/grassblockside.png"
    };

    loadblocktextures(texturepaths);
    loadblockmeshes();

    while (!glfwWindowShouldClose(window)) {
        cf = (float)glfwGetTime();
        dt = cf - lf;
        lf = cf;

        processinput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        blockshader.use();

        glm::mat4 model = glm::mat4(1.0f);
        blockshader.setmat4("model", model);

        glm::mat4 view = camera.getviewmatrix();
        blockshader.setmat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)scrwidth / (float)scrheight, 0.1f, 100.0f);
        blockshader.setmat4("projection", projection);

        updatechunks(camera.pos);

        //std::cout << chunks.size() << std::endl;

        for (auto &[chunkpos, chunk] : chunks) {
            chunk.mesh.draw(blockshader);
        }

        leftclick = false;
        rightclick = false;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void framebuffersizecallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mousebuttoncallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        camera.destroyblock();
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        camera.placeblock();
    }
}

void cursorposcallback(GLFWwindow *window, double x, double y) {
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

void processinput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processkeyboard(FORWARD, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processkeyboard(BACKWARD, dt);
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