#ifndef RAYCAST_HPP
#define RAYCAST_HPP

#include <glm/glm.hpp>
#include <blocks.hpp>
#include <chunks.hpp>
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

    glm::vec3 local_pos = origin - glm::vec3(chunk_pos * chunk_size);

    int x = floor(local_pos.x);
    int y = floor(local_pos.y);
    int z = floor(local_pos.z);

    int step_x = direction.x > 0.0f ? 1 : -1;
    int step_y = direction.y > 0.0f ? 1 : -1;
    int step_z = direction.z > 0.0f ? 1 : -1;

    float t_delta_x = direction.x != 0.0f ? abs(1.0f / direction.x) : FLT_MAX;
    float t_delta_y = direction.y != 0.0f ? abs(1.0f / direction.y) : FLT_MAX;
    float t_delta_z = direction.z != 0.0f ? abs(1.0f / direction.z) : FLT_MAX;

    float t_max_x = direction.x > 0.0f ? (x + 1.0f - local_pos.x) / direction.x : (local_pos.x - x) / -direction.x;
    float t_max_y = direction.y > 0.0f ? (y + 1.0f - local_pos.y) / direction.y : (local_pos.y - y) / -direction.y;
    float t_max_z = direction.z > 0.0f ? (z + 1.0f - local_pos.z) / direction.z : (local_pos.z - z) / -direction.z;

    float traveled = 0.0f;

    while (traveled < reach_dist) {
        if (x == -1 || x == chunk_size ||
            y == -1 || y == chunk_size ||
            z == -1 || z == chunk_size) {
            bool changed = false;

            if (x == -1) {
                x = chunk_size - 1;
                chunk_pos.x--;

                changed = true;
            } else if (x == chunk_size) {
                x = 0;
                chunk_pos.x++;

                changed = true;
            }

            if (y == -1) {
                y = chunk_size - 1;
                chunk_pos.y--;

                changed = true;
            } else if (y == chunk_size) {
                y = 0;
                chunk_pos.y++;

                changed = true;
            }

            if (z == -1) {
                z = chunk_size - 1;
                chunk_pos.z--;

                changed = true;
            } else if (z == chunk_size) {
                z = 0;
                chunk_pos.z++;

                changed = true;
            }

            if (changed) {
                if (!chunks.count(chunk_pos)) {
                    break;
                }
            }
        } else {
            if (chunks[chunk_pos].blocks[x][y][z] != AIR) {
                result.hit = true;

                result.block = glm::ivec3(x, y, z);
                result.block += chunk_pos * chunk_size;

                break;
            }

            if (t_max_x < t_max_y && t_max_x < t_max_z) {
                x += step_x;
                traveled = t_max_x;
                t_max_x += t_delta_x;

                result.normal = glm::vec3(-step_x, 0.0f, 0.0f);
            } else if (t_max_y < t_max_z) {
                y += step_y;
                traveled = t_max_y;
                t_max_y += t_delta_y;

                result.normal = glm::vec3(0.0f, -step_y, 0.0f);
            } else {
                z += step_z;
                traveled = t_max_z;
                t_max_z += t_delta_z;

                result.normal = glm::vec3(0.0f, 0.0f, -step_z);
            }
        }
    }
}

#endif