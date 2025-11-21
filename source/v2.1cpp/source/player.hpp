#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <camera.hpp>
#include <raycast.hpp>

class Player {
public:
    Camera camera;

    unsigned int selected_block_id = 3;

    Player() {}

    void destroy_block() {
        Rayhit result;
        raycast(result, camera.pos, camera.front);

        if (result.hit) {
            glm::ivec3 block_pos = result.block;

            update_block(block_pos, 0);
        }
    }

    void place_block() {
        Rayhit result;
        raycast(result, camera.pos, camera.front);

        if (result.hit) {
            glm::ivec3 block_pos = result.block + glm::ivec3(result.normal);

            update_block(block_pos, selected_block_id);
        }
    }
};

#endif