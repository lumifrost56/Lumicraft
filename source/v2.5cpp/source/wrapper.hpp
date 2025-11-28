#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <settings.hpp>
#include <utils.hpp>
#include <vector>

void init_glfw();
GLFWwindow *create_window(int width,
                          int height,
                          const char *name);
void make_context_current(GLFWwindow *window);
void disable_cursor(GLFWwindow *window);
void enable_cursor(GLFWwindow *window);
void enable_vsync();
void load_glad();
void enable_depth_test();
void disable_depth_test();
void enable_face_cull();
void disable_face_cull();
void set_front_CW();
void set_front_CCW();
void set_polygon_mode_line();
void set_polygon_mode_fill();
void set_line_width(float width);
void set_frame_buffer_size_callback(GLFWwindow *window);
void clear_buffers();
void swap_buffers(GLFWwindow *window);
void key_callback(GLFWwindow *window,
                  int key,
                  int scancode,
                  int action,
                  int mods);
void set_key_callback(GLFWwindow *window);
void cursor_pos_callback(GLFWwindow *window, double x, double y);
void set_cursor_pos_callback(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window,
                           int button,
                           int action,
                           int mods);
void set_mouse_button_callback(GLFWwindow *window);
void process_input(GLFWwindow *window);
void poll_events();
void glfw_terminate();
void set_window_should_close(GLFWwindow *window, bool should);
bool window_should_close(GLFWwindow *window);
bool pressed_escape(GLFWwindow *window, int key, int action);
bool pressed_hold_w(GLFWwindow *window);
bool pressed_hold_s(GLFWwindow *window);
bool pressed_hold_a(GLFWwindow *window);
bool pressed_hold_d(GLFWwindow *window);
bool pressed_hold_space(GLFWwindow *window);
bool pressed_hold_left_shift(GLFWwindow *window);
bool pressed_1(GLFWwindow *window, int key, int action);
bool pressed_2(GLFWwindow *window, int key, int action);
bool pressed_3(GLFWwindow *window, int key, int action);
bool pressed_4(GLFWwindow *window, int key, int action);
bool pressed_5(GLFWwindow *window, int key, int action);
bool pressed_6(GLFWwindow *window, int key, int action);
bool pressed_7(GLFWwindow *window, int key, int action);
bool pressed_8(GLFWwindow *window, int key, int action);
bool pressed_9(GLFWwindow *window, int key, int action);
bool pressed_b(GLFWwindow *window, int key, int action);
bool pressed_v(GLFWwindow *window, int key, int action);
bool pressed_mouse_left(GLFWwindow *window,
                        int button,
                        int action);
bool pressed_mouse_right(GLFWwindow *window,
                         int button,
                         int action);
float get_time();

void create_vertex_shader(unsigned int &vertex,
                          const char *vertex_code);
void create_fragment_shader(unsigned int &fragment,
                            const char *fragment_code);
void create_shader_program(unsigned int &id);
void attach_shader(unsigned int &id, unsigned int &shader);
void link_shader_program(unsigned int &id);
void delete_shader(unsigned int &shader);
void use_shader_program(unsigned int &id);
void set_uniform_1i(unsigned int &id, const char *name, int value);
void set_uniform_1f(unsigned int &id, const char *name, float value);
void set_uniform_2fv(unsigned int &id,
                     const char *name,
                     glm::vec2 value);
void set_uniform_3fv(unsigned int &id,
                     const char *name,
                     glm::vec3 value);
void set_uniform_4fv(unsigned int &id,
                     const char *name,
                     glm::vec4 value);
void set_uniform_matrix2fv(unsigned int &id,
                           const char *name,
                           glm::mat2 value);
void set_uniform_matrix3fv(unsigned int &id,
                           const char *name,
                           glm::mat3 value);
void set_uniform_matrix4fv(unsigned int &id,
                           const char *name,
                           glm::mat4 value);
void set_active_texture(int n);
void bind_tex_array(unsigned int &tex_array);
void bind_vertex_array(unsigned int &VAO);
void draw_triangles(int n);
void draw_line(int i, int n);
void delete_buffer(unsigned int &buffer);
void delete_vertex_array(unsigned int &VAO);
void generate_vertex_array(unsigned int &VAO);
void generate_buffer(unsigned int &buffer);
void bind_array_buffer(unsigned int &VBO);
void set_array_buffer_data(std::vector<Vertex> &vertices);
void bind_element_array_buffer(unsigned int &EBO);
void set_element_array_buffer_data(std::vector<unsigned int> &indices);
void enable_vertex_attrib_array(int n);
void set_vertex_attrib_pointer(int a,
                               int b,
                               int offset);
void generate_texture(unsigned int &texture);
void set_tex_image_3D(int width,
                      int height,
                      int layers);
void set_tex_sub_image_3D(int i,
                          int width,
                          int height,
                          unsigned char *data);
void set_tex_array_min_filter_near_lin();
void set_tex_array_mag_filter_near();
void set_tex_array_wrap_s_repeat();
void set_tex_array_wrap_t_repeat();
void generate_mipmap_tex_array();

#endif