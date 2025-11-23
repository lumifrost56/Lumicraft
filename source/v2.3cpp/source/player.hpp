#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <camera.hpp>
#include <block.hpp>
#include <chunk.hpp>
#include <raycast.hpp>
#include <vector>
#include <algorithm>
#include <limits>

//glm::vec3 player_hitbox_size = glm::vec3(0.6f, 1.8f, 0.6f);
glm::vec3 player_hitbox_top = glm::vec3(0.3f, 0.2f, 0.3f);
glm::vec3 player_hitbox_bottom = -glm::vec3(0.3f, 1.6f, 0.3f);

class Player {
public:
    Camera camera;

    unsigned int selected_block_id = STONE;

    float speed = 3.0f;

    Player() {}

    glm::vec3 swept_aabb_collision(glm::vec3 start_pos, glm::vec3 velocity) {
        if (velocity == glm::vec3(0.0f)) {
            return start_pos;
        }
        
        glm::vec3 end_pos = start_pos + velocity;
        
        glm::vec3 player_min_start = start_pos + player_hitbox_bottom;
        glm::vec3 player_max_start = start_pos + player_hitbox_top;
        
        glm::vec3 broad_min = glm::min(player_min_start, player_min_start + velocity) - glm::vec3(1.0f);
        glm::vec3 broad_max = glm::max(player_max_start, player_max_start + velocity) + glm::vec3(1.0f);
        
        glm::ivec3 block_min = glm::floor(broad_min);
        glm::ivec3 block_max = glm::ceil(broad_max);
        
        float earliest_collision_time = 1.0f;
        glm::vec3 collision_normal(0.0f);
        glm::vec3 remaining_velocity = velocity;
        
        const int max_iterations = 3;
        
        for (int iteration = 0; iteration < max_iterations && glm::length(remaining_velocity) > 0.0001f; iteration++) {
            float current_earliest_time = 1.0f;
            glm::vec3 current_normal(0.0f);
            
            for (int x = block_min.x; x <= block_max.x; x++) {
                for (int y = block_min.y; y <= block_max.y; y++) {
                    for (int z = block_min.z; z <= block_max.z; z++) {
                        glm::ivec3 block_pos(x, y, z);
                        unsigned int block_id = get_block(block_pos);
                        
                        if (block_id == AIR) {
                            continue;
                        }
                        
                        glm::vec3 block_min_pos = glm::vec3(block_pos);
                        glm::vec3 block_max_pos = glm::vec3(block_pos) + glm::vec3(1.0f);
                        
                        float collision_time;
                        glm::vec3 normal;
                        
                        if (swept_aabb_vs_aabb(player_min_start, player_max_start, remaining_velocity,
                                               block_min_pos, block_max_pos, collision_time, normal)) {
                            if (collision_time < current_earliest_time) {
                                current_earliest_time = collision_time;
                                current_normal = normal;
                            }
                        }
                    }
                }
            }
            
            if (current_earliest_time < 1.0f) {
                start_pos += remaining_velocity * current_earliest_time;
                start_pos += current_normal * 0.001f;
                
                if (current_normal.x != 0) remaining_velocity.x = 0;
                if (current_normal.y != 0) remaining_velocity.y = 0;
                if (current_normal.z != 0) remaining_velocity.z = 0;
                
                player_min_start = start_pos + player_hitbox_bottom;
                player_max_start = start_pos + player_hitbox_top;
            } else {
                start_pos += remaining_velocity;

                break;
            }
        }
        
        return start_pos;
    }

    bool swept_aabb_vs_aabb(const glm::vec3& a_min, const glm::vec3& a_max, const glm::vec3& vel,
                            const glm::vec3& b_min, const glm::vec3& b_max,
                            float& collision_time, glm::vec3& normal) {
        collision_time = 1.0f;
        normal = glm::vec3(0.0f);
        
        if (vel == glm::vec3(0.0f)) {
            if (a_min.x < b_max.x && a_max.x > b_min.x &&
                a_min.y < b_max.y && a_max.y > b_min.y &&
                a_min.z < b_max.z && a_max.z > b_min.z) {
                collision_time = 0.0f;

                glm::vec3 penetration;
                penetration.x = (a_max.x - b_min.x) < (b_max.x - a_min.x) ? (a_max.x - b_min.x) : (b_max.x - a_min.x);
                penetration.y = (a_max.y - b_min.y) < (b_max.y - a_min.y) ? (a_max.y - b_min.y) : (b_max.y - a_min.y);
                penetration.z = (a_max.z - b_min.z) < (b_max.z - a_min.z) ? (a_max.z - b_min.z) : (b_max.z - a_min.z);
                
                if (penetration.x < penetration.y && penetration.x < penetration.z) {
                    normal.x = (a_max.x - b_min.x) < (b_max.x - a_min.x) ? -1.0f : 1.0f;
                } else if (penetration.y < penetration.z) {
                    normal.y = (a_max.y - b_min.y) < (b_max.y - a_min.y) ? -1.0f : 1.0f;
                } else {
                    normal.z = (a_max.z - b_min.z) < (b_max.z - a_min.z) ? -1.0f : 1.0f;
                }
                return true;
            }
            return false;
        }
        
        glm::vec3 t_entry, t_exit;
        
        if (vel.x >= 0) {
            t_entry.x = (b_min.x - a_max.x) / (vel.x != 0 ? vel.x : 0.0001f);
            t_exit.x = (b_max.x - a_min.x) / (vel.x != 0 ? vel.x : 0.0001f);
        } else {
            t_entry.x = (b_max.x - a_min.x) / vel.x;
            t_exit.x = (b_min.x - a_max.x) / vel.x;
        }
        
        if (vel.y >= 0) {
            t_entry.y = (b_min.y - a_max.y) / (vel.y != 0 ? vel.y : 0.0001f);
            t_exit.y = (b_max.y - a_min.y) / (vel.y != 0 ? vel.y : 0.0001f);
        } else {
            t_entry.y = (b_max.y - a_min.y) / vel.y;
            t_exit.y = (b_min.y - a_max.y) / vel.y;
        }
        
        if (vel.z >= 0) {
            t_entry.z = (b_min.z - a_max.z) / (vel.z != 0 ? vel.z : 0.0001f);
            t_exit.z = (b_max.z - a_min.z) / (vel.z != 0 ? vel.z : 0.0001f);
        } else {
            t_entry.z = (b_max.z - a_min.z) / vel.z;
            t_exit.z = (b_min.z - a_max.z) / vel.z;
        }
        
        float entry_time = glm::max(glm::max(t_entry.x, t_entry.y), t_entry.z);
        float exit_time = glm::min(glm::min(t_exit.x, t_exit.y), t_exit.z);
        
        if (entry_time > exit_time || entry_time < 0 || entry_time > 1.0f) {
            return false;
        }
        
        if (entry_time == t_entry.x) {
            normal = glm::vec3(vel.x >= 0 ? -1.0f : 1.0f, 0.0f, 0.0f);
        } else if (entry_time == t_entry.y) {
            normal = glm::vec3(0.0f, vel.y >= 0 ? -1.0f : 1.0f, 0.0f);
        } else {
            normal = glm::vec3(0.0f, 0.0f, vel.z >= 0 ? -1.0f : 1.0f);
        }
        
        collision_time = entry_time;
        return true;
    }
    
    bool aabb_vs_aabb(const glm::vec3& a_min, const glm::vec3& a_max,
                      const glm::vec3& b_min, const glm::vec3& b_max) {
        return (a_min.x <= b_max.x && a_max.x >= b_min.x &&
                a_min.y <= b_max.y && a_max.y >= b_min.y &&
                a_min.z <= b_max.z && a_max.z >= b_min.z);
    }

    void process_keyboard(camera_movement direction, float dt) {
        float velocity = speed * dt;

        glm::vec3 dpos(0.0f);

        glm::vec3 front_xz = glm::normalize(glm::vec3(camera.front.x, 0.0f, camera.front.z));
        glm::vec3 right_xz = glm::normalize(glm::vec3(camera.right.x, 0.0f, camera.right.z));

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
            dpos = glm::normalize(dpos) * velocity;
            
            glm::vec3 new_pos = swept_aabb_collision(camera.pos, dpos);

            camera.pos = new_pos;
        }
    }

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