#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wrapper.hpp>
#include <shader.hpp>
#include <block.hpp>
#include <chunk.hpp>
#include <raycast.hpp>
#include <player.hpp>
#include <crosshair.hpp>
#include <iostream>

const unsigned int width = 1500;
const unsigned int height = 900;
float aspect_ratio = (float)width / (float)height;
const char *name = "Lumicraft";
GLFWwindow *window;

Shader block_shader;
Shader crosshair_shader;

void load_shaders() {
    const char *block_vrs_path = "../source/shaders/block.vrs";
    const char *block_frs_path = "../source/shaders/block.frs";
    block_shader.load(block_vrs_path, block_frs_path);

    const char *crosshair_vrs_path = "../source/shaders/crosshair.vrs";
    const char *crosshair_frs_path = "../source/shaders/crosshair.frs";
    crosshair_shader.load(crosshair_vrs_path, crosshair_frs_path);
}

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
    window = create_window(width, height, name);
    make_context_current(window);
    disable_cursor(window);
    enable_vsync();
    load_glad();
    enable_depth_test();
    enable_face_cull();
    set_front_CW();
    set_frame_buffer_size_callback(window);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    load_shaders();
    load_block_textures();
    load_block_meshes();

    set_cursor_pos_callback(window);
    set_mouse_button_callback(window);

    Crosshair crosshair((int)width, (int)height);

    std::thread worker(chunkloader_worker);

    while (!window_should_close(window)) {
        update_dt();
        process_input(window);
        update_chunks(player.camera.pos);
        clear_window();
        draw_chunks(block_shader, player.camera, aspect_ratio);
        crosshair.draw(crosshair_shader);
        swap_buffers(window);
        poll_events();
    }

    chunkloader_worker_running = false;
    queue_cv.notify_all();
    worker.join();

    terminate();
    return 0;
}

void process_input(GLFWwindow *window) {
    if (pressed_escape(window)) {
        set_window_should_close(window, true);
    }

    if (pressed_w(window)) {
        player.camera.process_keyboard(FORWARD, dt);
    }
    if (pressed_s(window)) {
        player.camera.process_keyboard(BACKWARD, dt);
    }
    if (pressed_a(window)) {
        player.camera.process_keyboard(LEFT, dt);
    }
    if (pressed_d(window)) {
        player.camera.process_keyboard(RIGHT, dt);
    }
    if (pressed_space(window)) {
        player.camera.process_keyboard(UP, dt);
    }
    if (pressed_left_shift(window)) {
        player.camera.process_keyboard(DOWN, dt);
    }

    if (pressed_1(window)) {
        player.selected_block_id = STONE;
    }
    if (pressed_2(window)) {
        player.selected_block_id = ICE;
    }
    if (pressed_3(window)) {
        player.selected_block_id = TUFF;
    }
    if (pressed_4(window)) {
        player.selected_block_id = OAKLOG;
    }
}

float last_x = width / 2.0f;
float last_y = height / 2.0f;
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