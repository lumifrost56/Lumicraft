#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgloader/stb_image.h>
#include <shader.h>
#include <camera.h>

void framebuffersizecallback(GLFWwindow* window, int width, int height);
void mousecallback(GLFWwindow* window, double x, double y);
void processinput(GLFWwindow* window);

const unsigned int scrwidth = 1080, scrheight = 720;
float dt = 0.0f, cf = 0.0f, lf = 0.0f;

Camera camera;
float lastx = scrwidth / 2.0f;
float lasty = scrheight / 2.0f;
bool firstmouse = true;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        cf = (float)glfwGetTime();
        dt = cf - lf;
        lf = cf;
        processinput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    float yos = ny - lasty;
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