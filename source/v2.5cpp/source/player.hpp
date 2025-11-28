#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include <utils.hpp>
#include <blocks.hpp>
#include <chunks.hpp>
#include <camera.hpp>
#include <raycast.hpp>

struct Player {
    Camera camera;

    glm::vec3 &pos = camera.pos;
    glm::vec3 &velocity = camera.velocity;

    uint8_t selected_block_id = STONE;

    glm::vec3 hitbox_top = glm::vec3(0.3f, 0.2f, 0.3f);
    glm::vec3 hitbox_bottom = -glm::vec3(0.3f, 1.6f, 0.3f);

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
            if (result.normal == glm::vec3(0.0f)) {
                return;
            }

            glm::ivec3 block_pos = result.block + glm::ivec3(result.normal);

            update_block(block_pos, selected_block_id);
        }
    }

    void swept_aabb_blocks_collision() {
        glm::vec3 start_pos = pos;

        if (velocity == glm::vec3(0.0f)) {
            pos = start_pos;
        }

        glm::vec3 end_pos = start_pos + velocity;

        glm::vec3 player_min_start = start_pos + hitbox_bottom;
        glm::vec3 player_max_start = start_pos + hitbox_top;

        glm::vec3 broad_min = glm::min(player_min_start, player_min_start + velocity) - glm::vec3(1.0f);
        glm::vec3 broad_max = glm::max(player_max_start, player_max_start + velocity) + glm::vec3(1.0f);

        glm::ivec3 block_min = glm::floor(broad_min);
        glm::ivec3 block_max = glm::ceil(broad_max);

        float earliest_coll_time = 1.0f;
        
        glm::vec3 coll_normal = glm::vec3(0.0f);

        glm::vec3 remaining_velocity = velocity;

        int n_iterations = 3;

        for (int i = 0; i < n_iterations && glm::length(remaining_velocity) > 0.0001f; i++) {
            float current_earliest_time = 1.0f;

            glm::vec3 current_normal = glm::vec3(0.0f);

            for (int x = block_min.x; x <= block_max.x; x++) {
                for (int y = block_min.y; y <= block_max.y; y++) {
                    for (int z = block_min.z; z <= block_max.z; z++) {
                        glm::ivec3 block_pos = glm::ivec3(x, y, z);

                        uint8_t block_id = get_block_id(block_pos);

                        if (block_id == AIR) {
                            continue;
                        }

                        glm::vec3 block_min_pos = glm::vec3(block_pos);
                        glm::vec3 block_max_pos = glm::vec3(block_pos) + glm::vec3(1.0f);

                        float coll_time;

                        glm::vec3 normal;

                        if (swept_aabb_intersect(player_min_start, player_max_start,
                                                 block_min_pos, block_max_pos,
                                                 remaining_velocity,
                                                 coll_time,
                                                 normal)) {
                            if (coll_time < current_earliest_time) {
                                current_earliest_time = coll_time;

                                current_normal = normal;
                            }
                        }
                    }
                }
            }

            if (current_earliest_time < 1.0f) {
                start_pos += current_normal * 0.001f;

                if (current_normal.x != 0.0f) {
                    start_pos.x += remaining_velocity.x * current_earliest_time;

                    remaining_velocity.x = 0.0f;
                }
                if (current_normal.y != 0.0f) {
                    start_pos.y += remaining_velocity.y * current_earliest_time;

                    remaining_velocity.y = 0.0f;
                }
                if (current_normal.z != 0.0f) {
                    start_pos.z += remaining_velocity.z * current_earliest_time;

                    remaining_velocity.z = 0.0f;
                }

                player_min_start = start_pos + hitbox_bottom;
                player_max_start = start_pos + hitbox_top;
            } else {
                start_pos += remaining_velocity;
                break;
            }
        }

        pos = start_pos;
    }
};

#endif