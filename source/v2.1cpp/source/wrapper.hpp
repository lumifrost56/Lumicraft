#ifndef WRAPPER_H
#define WRAPPER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void init_glfw();
GLFWwindow *create_window(const unsigned int width,
                          const unsigned int height,
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
void set_frame_buffer_size_callback(GLFWwindow *window);
bool window_should_close(GLFWwindow *window);
void set_window_should_close(GLFWwindow *window, bool should);
float get_time();
void process_input(GLFWwindow *window);
void cursor_pos_callback(GLFWwindow *window, double x, double y);
void set_cursor_pos_callback(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window,
                           int button,
                           int action,
                           int mods);
void set_mouse_button_callback(GLFWwindow *window);
void clear_window();
void swap_buffers(GLFWwindow *window);
void poll_events();
void terminate();
bool pressed_escape(GLFWwindow *window);
bool pressed_w(GLFWwindow *window);
bool pressed_s(GLFWwindow *window);
bool pressed_a(GLFWwindow *window);
bool pressed_d(GLFWwindow *window);
bool pressed_space(GLFWwindow *window);
bool pressed_left_shift(GLFWwindow *window);
bool pressed_mouse_left(GLFWwindow *window,
                        int button,
                        int action);
bool pressed_mouse_right(GLFWwindow *window,
                         int button,
                         int action);

#endif