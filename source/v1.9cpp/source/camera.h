#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <raycaster.h>

enum cameramovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 pos, front, right, wup, up;
    float yaw, pitch, fov, speed, sens;

    int blockselectedid = 1;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 3.0f, 0.0f)) {
        this->pos = pos;

        front = glm::vec3(0.0f, 0.0f, -1.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        wup = glm::vec3(0.0f, 1.0f, 0.0f);
        up = wup;

        yaw = -90.0f;
        pitch = 0.0f;
        fov = 80.0f;
        
        speed = 3.0f;
        sens = 0.1f;
    }

    glm::mat4 getviewmatrix() {
        return glm::lookAt(pos, pos + front, wup);
    }

    void processkeyboard(cameramovement dir, float dt) {
        float velocity = speed * dt;

        glm::vec3 dpos(0.0f);

        glm::vec3 frontxz = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 rightxz = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

        if (dir == FORWARD) {
            dpos += frontxz;
        }
        if (dir == BACKWARD) {
            dpos -= frontxz;
        }
        if (dir == LEFT) {
            dpos -= rightxz;
        }
        if (dir == RIGHT) {
            dpos += rightxz;
        }
        if (dir == UP) {
            dpos += wup;
        }
        if (dir == DOWN) {
            dpos -= wup;
        }

        if (dpos != glm::vec3(0.0f)) {
            dpos = glm::normalize(dpos);
            pos += dpos * velocity;
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

        updatevectors();
    }

    void destroyblock() {
        rayhit result;

        raycaster.cast(result, pos, front);

        if (result.hit) {
            glm::vec3 blockpos = glm::floor(result.block);

            glm::vec3 chunkpos = glm::floor(glm::vec3(blockpos) / glm::vec3(chunkxz, chunky, chunkxz));

            glm::ivec3 lblockpos = blockpos - chunkpos * glm::vec3(chunkxz, chunky, chunkxz);

            chunks[chunkpos].blocks[lblockpos.x][lblockpos.y][lblockpos.z] = 0;

            chunks[chunkpos].generatemesh();

            if (lblockpos.x == 0 && chunks.count(chunkpos + glm::vec3(-1,0,0))) {
                chunks[chunkpos + glm::vec3(-1,0,0)].generatemesh();
            }
            if (lblockpos.x == chunkxz-1 && chunks.count(chunkpos + glm::vec3(1,0,0))) {
                chunks[chunkpos + glm::vec3(1,0,0)].generatemesh();
            }
            if (lblockpos.z == 0 && chunks.count(chunkpos + glm::vec3(0,0,-1))) {
                chunks[chunkpos + glm::vec3(0,0,-1)].generatemesh();
            }
            if (lblockpos.z == chunkxz-1 && chunks.count(chunkpos + glm::vec3(0,0,1))) {
                chunks[chunkpos + glm::vec3(0,0,1)].generatemesh();
            }
        }
    }

    void placeblock() {
        rayhit result;

        raycaster.cast(result, pos, front);

        if (result.hit) {
            glm::vec3 placepos = glm::floor(result.block + result.normal);

            if (placepos.y < 0 || placepos.y > chunky-1) {
                return;
            }

            glm::vec3 chunkpos = glm::floor(glm::vec3(placepos) / glm::vec3(chunkxz, chunky, chunkxz));

            glm::ivec3 lblockpos = placepos - chunkpos * glm::vec3(chunkxz, chunky, chunkxz);

            if (chunks[chunkpos].blocks[lblockpos.x][lblockpos.y][lblockpos.z] != 0) {
                return;
            }

            int tempblockselectedid = blockselectedid;

            if (blockselectedid == 1) {
                if (result.normal == glm::vec3(0.0f, 1.0f, 0.0f)) {
                    tempblockselectedid = 1;
                } else if (result.normal == glm::vec3(0.0f, -1.0f, 0.0f)) {
                    tempblockselectedid = 1;
                } else if (result.normal == glm::vec3(0.0f, 0.0f, 1.0f)) {
                    tempblockselectedid = 2;
                } else if (result.normal == glm::vec3(0.0f, 0.0f, -1.0f)) {
                    tempblockselectedid = 2;
                } else if (result.normal == glm::vec3(1.0f, 0.0f, 0.0f)) {
                    tempblockselectedid = 3;
                } else if (result.normal == glm::vec3(-1.0f, 0.0f, 0.0f)) {
                    tempblockselectedid = 3;
                }
            }

            chunks[chunkpos].blocks[lblockpos.x][lblockpos.y][lblockpos.z] = tempblockselectedid;

            chunks[chunkpos].generatemesh();

            if (lblockpos.x == 0 && chunks.count(chunkpos + glm::vec3(-1,0,0))) {
                chunks[chunkpos + glm::vec3(-1,0,0)].generatemesh();
            }
            if (lblockpos.x == chunkxz-1 && chunks.count(chunkpos + glm::vec3(1,0,0))) {
                chunks[chunkpos + glm::vec3(1,0,0)].generatemesh();
            }
            if (lblockpos.z == 0 && chunks.count(chunkpos + glm::vec3(0,0,-1))) {
                chunks[chunkpos + glm::vec3(0,0,-1)].generatemesh();
            }
            if (lblockpos.z == chunkxz-1 && chunks.count(chunkpos + glm::vec3(0,0,1))) {
                chunks[chunkpos + glm::vec3(0,0,1)].generatemesh();
            }
        }
    }
private:
    void updatevectors() {
        glm::vec3 nfront;
        nfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        nfront.y = sin(glm::radians(pitch));
        nfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        front = glm::normalize(nfront);
        right = glm::normalize(glm::cross(front, wup));
        up = glm::normalize(glm::cross(right, front));
    }
};

Camera camera;
#endif