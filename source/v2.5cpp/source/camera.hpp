#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <settings.hpp>

struct Frustum {
    glm::vec4 planes[6];

    void update(glm::mat4 view, float fov) {
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);

        glm::mat4 view_projection = projection * view;

        planes[0] = glm::vec4(
            view_projection[0][3] + view_projection[0][0],
            view_projection[1][3] + view_projection[1][0],
            view_projection[2][3] + view_projection[2][0],
            view_projection[3][3] + view_projection[3][0]
        );

        planes[1] = glm::vec4(
            view_projection[0][3] - view_projection[0][0],
            view_projection[1][3] - view_projection[1][0],
            view_projection[2][3] - view_projection[2][0],
            view_projection[3][3] - view_projection[3][0]
        );

        planes[2] = glm::vec4(
            view_projection[0][3] + view_projection[0][1],
            view_projection[1][3] + view_projection[1][1],
            view_projection[2][3] + view_projection[2][1],
            view_projection[3][3] + view_projection[3][1]
        );

        planes[3] = glm::vec4(
            view_projection[0][3] - view_projection[0][1],
            view_projection[1][3] - view_projection[1][1],
            view_projection[2][3] - view_projection[2][1],
            view_projection[3][3] - view_projection[3][1]
        );

        planes[4] = glm::vec4(
            view_projection[0][3] + view_projection[0][2],
            view_projection[1][3] + view_projection[1][2],
            view_projection[2][3] + view_projection[2][2],
            view_projection[3][3] + view_projection[3][2]
        );

        planes[5] = glm::vec4(
            view_projection[0][3] - view_projection[0][2],
            view_projection[1][3] - view_projection[1][2],
            view_projection[2][3] - view_projection[2][2],
            view_projection[3][3] - view_projection[3][2]
        );

        for (int i = 0; i < 6; i++) {
            float length = glm::length(glm::vec3(planes[i]));

            planes[i] /= length;
        }
    }

    bool is_aabb_visible(glm::vec3 minv, glm::vec3 maxv) {
        for (int i = 0; i < 6; i++) {
            glm::vec3 positive_vertex = minv;

            if (planes[i].x >= 0) {
                positive_vertex.x = maxv.x;
            }
            if (planes[i].y >= 0) {
                positive_vertex.y = maxv.y;
            }
            if (planes[i].z >= 0) {
                positive_vertex.z = maxv.z;
            }

            if (glm::dot(glm::vec3(planes[i]), positive_vertex) + planes[i].w < 0) {
                return false;
            }
        }

        return true;
    }
};

enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

struct Camera {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 80.0f;

    float speed = 5.0f;
    float sensitivity = 0.1f;

    glm::vec3 velocity = glm::vec3(0.0f);

    Frustum frustum;

    Camera() {
        update_vectors();
    }

    glm::mat4 get_view_mat() {
        return glm::lookAt(pos, pos + front, world_up);
    }

    void process_keyboard(camera_movement direction, bool update = true) {
        glm::vec3 dpos = glm::vec3(0.0f);

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

            velocity += dpos;
        }

        frustum.update(get_view_mat(), fov);
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

        frustum.update(get_view_mat(), fov);
    }

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