#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum cameramovement {
    FORWARDS,
    BACKWARDS,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 pos, front, right, up, wup;
    float yaw, pitch, fov, speed, sens;
    Camera(glm::vec3 spos = glm::vec3(0.0f)) {
        pos = spos;
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        wup = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        fov = 80.0f;
        speed = 2.5f;
        sens = 0.1f;
        update();
    }
    glm::mat4 getviewmatrix() {
        return glm::lookAt(pos, pos + front, wup);
    }
    void processkeyboard(cameramovement direction, float dt) {
        float velocity = speed * dt;
        glm::vec3 change(0.0f);
        glm::vec3 frontxz = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 rightxz = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
        if (direction == FORWARDS) {
            change += frontxz;
        }
        if (direction == BACKWARDS) {
            change -= frontxz;
        }
        if (direction == LEFT) {
            change -= rightxz;
        }
        if (direction == RIGHT) {
            change += rightxz;
        }
        if (direction == UP) {
            change += wup;
        }
        if (direction == DOWN) {
            change -= wup;
        }
        if (change != glm::vec3(0.0f)) {
            change = glm::normalize(change);
            pos += change * velocity;
        }
    }
    void processmouse(float xos, float yos) {
        xos *= sens;
        yos *= sens;
        yaw += xos;
        pitch += yos;
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
        update();
    }
private:
    void update() {
        glm::vec3 nfront;
        nfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        nfront.y = sin(glm::radians(pitch));
        nfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(nfront);
        right = glm::normalize(glm::cross(front, wup));
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif