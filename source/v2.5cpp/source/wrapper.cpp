#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <wrapper.hpp>
#include <settings.hpp>
#include <utils.hpp>
#include <vector>

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *create_window(int width,
                          int height,
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

void set_front_CCW() {
    glFrontFace(GL_CCW);
}

void set_polygon_mode_line() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void set_polygon_mode_fill() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void set_line_width(float width) {
    glLineWidth(width);
}

void frame_buffer_size_callback(GLFWwindow *window,
                                int width, int height) {
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
    aspect_ratio = (float)width / (float)height;
}

void clear_buffers() {
    glClearColor(0.4f, 0.7f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void set_frame_buffer_size_callback(GLFWwindow *window) {
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void set_key_callback(GLFWwindow *window) {
    glfwSetKeyCallback(window, key_callback);
}

void set_cursor_pos_callback(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, cursor_pos_callback);
}

void set_mouse_button_callback(GLFWwindow *window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void poll_events() {
    glfwPollEvents();
}

void glfw_terminate() {
    glfwTerminate();
}

void set_window_should_close(GLFWwindow *window, bool should) {
    glfwSetWindowShouldClose(window, should);
}

bool window_should_close(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

bool pressed_escape(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_ESCAPE && action == GLFW_PRESS;
}

bool pressed_hold_w(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
}

bool pressed_hold_s(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
}

bool pressed_hold_a(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
}

bool pressed_hold_d(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool pressed_hold_space(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
}

bool pressed_hold_left_shift(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

bool pressed_1(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_1 && action == GLFW_PRESS;
}

bool pressed_2(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_2 && action == GLFW_PRESS;
}

bool pressed_3(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_3 && action == GLFW_PRESS;
}

bool pressed_4(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_4 && action == GLFW_PRESS;
}

bool pressed_5(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_5 && action == GLFW_PRESS;
}

bool pressed_6(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_6 && action == GLFW_PRESS;
}

bool pressed_7(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_7 && action == GLFW_PRESS;
}

bool pressed_8(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_8 && action == GLFW_PRESS;
}

bool pressed_9(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_9 && action == GLFW_PRESS;
}

bool pressed_b(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_B && action == GLFW_PRESS;
}

bool pressed_v(GLFWwindow *window, int key, int action) {
    return key == GLFW_KEY_V && action == GLFW_PRESS;
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

float get_time() {
    return (float)glfwGetTime();
}

void create_vertex_shader(unsigned int &vertex,
                          const char *vertex_code) {
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glCompileShader(vertex);
}

void create_fragment_shader(unsigned int &fragment,
                            const char *fragment_code) {
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, NULL);
    glCompileShader(fragment);
}

void create_shader_program(unsigned int &id) {
    id = glCreateProgram();
}

void attach_shader(unsigned int &id, unsigned int &shader) {
    glAttachShader(id, shader);
}

void link_shader_program(unsigned int &id) {
    glLinkProgram(id);
}

void delete_shader(unsigned int &shader) {
    glDeleteShader(shader);
}

void use_shader_program(unsigned int &id) {
    glUseProgram(id);
}

void set_uniform_1i(unsigned int &id, const char *name, int value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void set_uniform_1f(unsigned int &id, const char *name, float value) {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void set_uniform_2fv(unsigned int &id,
                     const char *name,
                     glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void set_uniform_3fv(unsigned int &id,
                     const char *name,
                     glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void set_uniform_4fv(unsigned int &id,
                     const char *name,
                     glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void set_uniform_matrix2fv(unsigned int &id,
                           const char *name,
                           glm::mat2 value) {
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}

void set_uniform_matrix3fv(unsigned int &id,
                           const char *name,
                           glm::mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}

void set_uniform_matrix4fv(unsigned int &id,
                           const char *name,
                           glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}

void set_active_texture(int n) {
    glActiveTexture(GL_TEXTURE0 + n);
}

void bind_tex_array(unsigned int &tex_array) {
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex_array);
}

void bind_vertex_array(unsigned int &VAO) {
    glBindVertexArray(VAO);
}

void draw_triangles(int n) {
    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
}

void draw_line(int i, int n) {
    glDrawArrays(GL_LINES, i, n);
}

void delete_buffer(unsigned int &buffer) {
    glDeleteBuffers(1, &buffer);
}

void delete_vertex_array(unsigned int &VAO) {
    glDeleteVertexArrays(1, &VAO);
}

void generate_vertex_array(unsigned int &VAO) {
    glGenVertexArrays(1, &VAO);
}

void generate_buffer(unsigned int &buffer) {
    glGenBuffers(1, &buffer);
}

void bind_array_buffer(unsigned int &VBO) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void set_array_buffer_data(std::vector<Vertex> &vertices) {
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void bind_element_array_buffer(unsigned int &EBO) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void set_element_array_buffer_data(std::vector<unsigned int> &indices) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void enable_vertex_attrib_array(int n) {
    glEnableVertexAttribArray(n);
}

void set_vertex_attrib_pointer(int a,
                               int b,
                               int offset) {
    glVertexAttribPointer(a, b, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(intptr_t)offset);
}

void generate_texture(unsigned int &texture) {
    glGenTextures(1, &texture);
}

void set_tex_image_3D(int width,
                      int height,
                      int layers) {
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void set_tex_sub_image_3D(int i,
                          int width,
                          int height,
                          unsigned char *data) {
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void set_tex_array_min_filter_near_lin() {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
}

void set_tex_array_mag_filter_near() {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void set_tex_array_wrap_s_repeat() {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void set_tex_array_wrap_t_repeat() {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void generate_mipmap_tex_array() {
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}