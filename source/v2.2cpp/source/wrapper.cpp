#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wrapper.hpp>

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *create_window(const unsigned int width,
                          const unsigned int height,
                          const char *name) {
    return glfwCreateWindow(width, height, name, NULL, NULL);
}

void make_context_current(GLFWwindow *window) {
    glfwMakeContextCurrent(window);
}

void disable_cursor(GLFWwindow *window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void enable_cursor(GLFWwindow *window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void enable_vsync() {
    glfwSwapInterval(1);
}

void load_glad() {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void enable_depth_test() {
    glEnable(GL_DEPTH_TEST);
}

void disable_depth_test() {
    glDisable(GL_DEPTH_TEST);
}

void enable_face_cull() {
    glEnable(GL_CULL_FACE);
}

void disable_face_cull() {
    glDisable(GL_CULL_FACE);
}

void set_front_CW() {
    glFrontFace(GL_CW);
}

void frame_buffer_size_callback(GLFWwindow *window,
                                int width, int height) {
    glViewport(0, 0, width, height);
}

void set_frame_buffer_size_callback(GLFWwindow *window) {
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

bool window_should_close(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

void set_window_should_close(GLFWwindow *window, bool should) {
    glfwSetWindowShouldClose(window, should);
}

float get_time() {
    return (float)glfwGetTime();
}

void cursor_pos_callback(GLFWwindow *window, double x, double y);

void set_cursor_pos_callback(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, cursor_pos_callback);
}

void set_mouse_button_callback(GLFWwindow *window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void clear_window() {
    glClearColor(0.4f, 0.7f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void poll_events() {
    glfwPollEvents();
}

void terminate() {
    glfwTerminate();
}

bool pressed_escape(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

bool pressed_w(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
}

bool pressed_s(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
}

bool pressed_a(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
}

bool pressed_d(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool pressed_space(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
}

bool pressed_left_shift(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

bool pressed_1(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
}

bool pressed_2(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
}

bool pressed_3(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
}

bool pressed_4(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
}

bool pressed_5(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS;
}

bool pressed_6(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS;
}

bool pressed_7(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS;
}

bool pressed_8(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS;
}

bool pressed_9(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS;
}

bool pressed_mouse_left(GLFWwindow *window,
                        int button,
                        int action) {
    return button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
}

bool pressed_mouse_right(GLFWwindow *window,
                         int button,
                         int action) {
    return button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
}