#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <camera.hpp>
#include <block.hpp>
#include <raycast.hpp>

class Player {
public:
    Camera camera;

    unsigned int selected_block_id = STONE;

    Player() {}

    void destroy_block() {
        Rayhit result;
        raycast(result, camera.pos, camera.front);

        if (result.hit) {
            glm::ivec3 block_pos = result.block;

            update_block(block_pos, AIR);
        }
    }

    void place_block() {
        Rayhit result;
        raycast(result, camera.pos, camera.front);

        if (result.hit) {
            if (result.normal == glm::vec3(0)) {
                return;
            }

            glm::ivec3 block_pos = result.block + glm::ivec3(result.normal);

            unsigned int selected_block_id_temp = selected_block_id;

            if (orientation3.count(selected_block_id_temp)) {
                if (result.normal.z == 1.0f || result.normal.z == -1.0f) {
                    selected_block_id_temp += 1;
                }

                if (result.normal.x == 1.0f || result.normal.x == -1.0f) {
                    selected_block_id_temp += 2;
                }
            }

            update_block(block_pos, selected_block_id_temp);
        }
    }
};

#endif