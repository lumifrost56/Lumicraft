#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>
#include <block.hpp>
#include <chunk.hpp>
#include <vector>

float reach_dist = 5.0f;

struct Rayhit {
    bool hit = false;
    glm::ivec3 block;
    glm::vec3 normal = glm::vec3(0.0f);
};

void raycast(Rayhit &result,
             glm::vec3 origin,
             glm::vec3 direction) {
    direction = glm::normalize(direction);

    glm::ivec3 chunk_pos = glm::floor(origin / (float)chunk_size);

    glm::vec3 pos = origin - glm::vec3(chunk_pos * (int)chunk_size);

    Chunk chunk = chunks[chunk_pos];

    int x = floor(pos.x);
    int y = floor(pos.y);
    int z = floor(pos.z);

    int step_x = direction.x > 0 ? 1 : -1;
    int step_y = direction.y > 0 ? 1 : -1;
    int step_z = direction.z > 0 ? 1 : -1;

    float t_delta_x = direction.x != 0 ? abs(1.0f / direction.x) : FLT_MAX;
    float t_delta_y = direction.y != 0 ? abs(1.0f / direction.y) : FLT_MAX;
    float t_delta_z = direction.z != 0 ? abs(1.0f / direction.z) : FLT_MAX;

    float t_max_x = (direction.x > 0) ? ((x + 1 - pos.x) / direction.x) : ((pos.x - x) / -direction.x);
    float t_max_y = (direction.y > 0) ? ((y + 1 - pos.y) / direction.y) : ((pos.y - y) / -direction.y);
    float t_max_z = (direction.z > 0) ? ((z + 1 - pos.z) / direction.z) : ((pos.z - z) / -direction.z);

    float traveled = 0.0f;

    while (traveled < reach_dist) {
        if (x >= 0 && x < chunk_size &&
            y >= 0 && y < chunk_size &&
            z >= 0 && z < chunk_size) {
            if (chunk.blocks[x][y][z] != AIR) {
                result.hit = true;

                result.block = glm::ivec3(x, y, z);
                result.block += chunk_pos * (int)chunk_size;

                break;
            }

            if (t_max_x < t_max_y && t_max_x < t_max_z) {
                x += step_x;
                traveled = t_max_x;
                t_max_x += t_delta_x;

                result.normal = glm::vec3(-step_x, 0, 0);
            } else if (t_max_y < t_max_z) {
                y += step_y;
                traveled = t_max_y;
                t_max_y += t_delta_y;

                result.normal = glm::vec3(0, -step_y, 0);
            } else {
                z += step_z;
                traveled = t_max_z;
                t_max_z += t_delta_z;

                result.normal = glm::vec3(0, 0, -step_z);
            }

            bool changed = false;

            if (x < 0) {
                x = chunk_size-1;
                chunk_pos.x--;
                changed = true;
            } else if (x > chunk_size-1) {
                x = 0;
                chunk_pos.x++;
                changed = true;
            }

            if (y < 0) {
                y = chunk_size-1;
                chunk_pos.y--;
                changed = true;
            } else if (y > chunk_size-1) {
                y = 0;
                chunk_pos.y++;
                changed = true;
            }

            if (z < 0) {
                z = chunk_size-1;
                chunk_pos.z--;
                changed = true;
            } else if (z > chunk_size-1) {
                z = 0;
                chunk_pos.z++;
                changed = true;
            }

            if (changed) {
                if (!chunks.count(chunk_pos)) {
                    break;
                }

                chunk = chunks[chunk_pos];
            }
        }
    }
}

#endif