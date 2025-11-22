#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;
    float fov;

    float speed;
    float sensitivity;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 3.0f, 0.0f)) {
        this->pos = pos;

        yaw = -90.0f;
        pitch = 0.0f;
        fov = 80.0f;

        speed = 3.0f;
        sensitivity = 0.1f;

        update_vectors();
    }

    glm::mat4 get_view_mat() {
        return glm::lookAt(pos, pos + front, world_up);
    }

    void process_keyboard(camera_movement direction, float dt) {
        float velocity = speed * dt;

        glm::vec3 dpos(0.0f);

        glm::vec3 front_xz = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 right_xz = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

        if (direction == FORWARD) {
            dpos += front_xz;
        }
        if (direction == BACKWARD) {
            dpos -= front_xz;
        }
        if (direction == LEFT) {
            dpos -= right_xz;
        }
        if (direction == RIGHT) {
            dpos += right_xz;
        }
        if (direction == UP) {
            dpos += world_up;
        }
        if (direction == DOWN) {
            dpos -= world_up;
        }

        if (dpos != glm::vec3(0.0f)) {
            dpos = glm::normalize(dpos);
            pos += dpos * velocity;
        }
    }

    void process_mouse(float x_offset, float y_offset) {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        update_vectors();
    }

private:
    void update_vectors() {
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(new_front);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif