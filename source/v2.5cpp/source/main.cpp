#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <wrapper.hpp>
#include <settings.hpp>
#include <userinterface.hpp>
#include <blockmesh.hpp>
#include <blocks.hpp>
#include <chunks.hpp>
#include <player.hpp>
#include <iostream>

int screen_width = 1500;
int screen_height = 900;
float aspect_ratio = (float)screen_width / (float)screen_height;
std::string window_name = "Lumicraft";
GLFWwindow *window;

UI ui;
Player player;

float dt = 0.0f;
float cf = 0.0f;
float lf = 0.0f;

void update_dt() {
    cf = get_time();
    dt = cf - lf;
    lf = cf;
}

int main() {
    init_glfw();

    window = create_window(screen_width, screen_height, window_name.c_str());
    make_context_current(window);
    disable_cursor(window);
    enable_vsync();

    load_glad();

    enable_depth_test();
    enable_face_cull();
    set_front_CW();

    set_frame_buffer_size_callback(window);
    set_key_callback(window);
    set_cursor_pos_callback(window);
    set_mouse_button_callback(window);

    create_ui_shaders();
    create_blockmesh_shader();

    load_block_textures();
    create_block_meshes();

    ui.create_self();

    render_dist = 5;

    init_chunk_workers();

    while (!window_should_close(window)) {
        update_dt();

        process_input(window);

        update_chunks(player.pos);

        clear_buffers();

        draw_chunks(player.camera);

        ui.draw();

        swap_buffers(window);

        poll_events();
    }

    cleanup_chunk_workers();

    glfw_terminate();
    return 0;
}

bool collision = true;

void process_input(GLFWwindow *window) {
    player.camera.velocity = glm::vec3(0.0f);

    if (pressed_hold_w(window)) {
        player.camera.process_keyboard(FORWARD, !collision);
    }
    if (pressed_hold_s(window)) {
        player.camera.process_keyboard(BACKWARD, !collision);
    }
    if (pressed_hold_a(window)) {
        player.camera.process_keyboard(LEFT, !collision);
    }
    if (pressed_hold_d(window)) {
        player.camera.process_keyboard(RIGHT, !collision);
    }
    if (pressed_hold_space(window)) {
        player.camera.process_keyboard(UP, !collision);
    }
    if (pressed_hold_left_shift(window)) {
        player.camera.process_keyboard(DOWN, !collision);
    }

    if (player.camera.velocity == glm::vec3(0.0f)) {
        return;
    }

    player.camera.velocity = glm::normalize(player.camera.velocity) * player.camera.speed * dt;

    if (collision) {
        player.swept_aabb_blocks_collision();
    } else {
        player.camera.pos += player.camera.velocity;
    }
}

bool debug = false;

void key_callback(GLFWwindow *window,
                  int key,
                  int scancode,
                  int action,
                  int mods) {
    if (pressed_escape(window, key, action)) {
        set_window_should_close(window, true);
    }

    if (pressed_1(window, key, action)) {
        player.selected_block_id = STONE;
    }
    if (pressed_2(window, key, action)) {
        player.selected_block_id = TUFF;
    }
    if (pressed_3(window, key, action)) {
        player.selected_block_id = ICE;
    }
    if (pressed_4(window, key, action)) {
        player.selected_block_id = OAKLOG;
    }

    if (pressed_b(window, key, action)) {
        debug = !debug;

        if (debug) {
            set_polygon_mode_line();
        } else if (!debug) {
            set_polygon_mode_fill();
        }
    }

    if (pressed_v(window, key, action)) {
        collision = !collision;
    }
}

float last_x = screen_width / 2.0f;
float last_y = screen_height / 2.0f;

bool first_mouse = true;

void cursor_pos_callback(GLFWwindow *window, double x, double y) {
    float new_x = (float)x;
    float new_y = (float)y;

    if (first_mouse) {
        last_x = new_x;
        last_y = new_y;

        first_mouse = false;
    }

    float x_offset = new_x - last_x;
    float y_offset = last_y - new_y;

    last_x = new_x;
    last_y = new_y;

    player.camera.process_mouse(x_offset, y_offset);
}

void mouse_button_callback(GLFWwindow *window,
                           int button,
                           int action,
                           int mods) {
    if (pressed_mouse_left(window, button, action)) {
        player.destroy_block();
    }
    if (pressed_mouse_right(window, button, action)) {
        player.place_block();
    }
}