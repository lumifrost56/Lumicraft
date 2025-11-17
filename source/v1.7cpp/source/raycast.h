#ifndef RAYCAST_H
#define RAYCAST_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <camera.h>
#include <chunk.h>
#include <vector>

struct rayhit {
    bool hit = false;
    glm::vec3 block;
    glm::vec3 normal;
};

class Raycaster {
public:
    void cast(rayhit &result, glm::vec3 origin, glm::vec3 direction) {
        direction = glm::normalize(direction);
        glm::vec3 chunkpos = glm::floor(origin / 16.0f);
        glm::vec3 pos = origin - glm::vec3(chunkpos * 16.0f);
        Chunk chunk = chunks[chunkpos];
        int x = floor(pos.x);
        int y = floor(pos.y);
        int z = floor(pos.z);
        int stepx = direction.x > 0 ? 1 : -1;
        int stepy = direction.y > 0 ? 1 : -1;
        int stepz = direction.z > 0 ? 1 : -1;
        float tDeltaX = direction.x != 0 ? abs(1.0f / direction.x) : FLT_MAX;
        float tDeltaY = direction.y != 0 ? abs(1.0f / direction.y) : FLT_MAX;
        float tDeltaZ = direction.z != 0 ? abs(1.0f / direction.z) : FLT_MAX;
        float tMaxX = (direction.x > 0) ? ((x + 1 - pos.x) / direction.x) : ((pos.x - x) / -direction.x);
        float tMaxY = (direction.y > 0) ? ((y + 1 - pos.y) / direction.y) : ((pos.y - y) / -direction.y);
        float tMaxZ = (direction.z > 0) ? ((z + 1 - pos.z) / direction.z) : ((pos.z - z) / -direction.z);
        float traveled = 0.0f;
        while (traveled < 8.0f) {
            if (x >= 0 && x < 16 && y >= 0 && y < 16 && z >= 0 && z < 16) {
                if (chunk.blocks[x][y][z] != 0) {
                    result.hit = true;
                    result.block = glm::vec3(x, y, z) + glm::vec3(chunkpos * 16.0f);
                    return;
                }
            }
            if (tMaxX < tMaxY && tMaxX < tMaxZ) {
                x += stepx;
                traveled = tMaxX;
                tMaxX += tDeltaX;
                result.normal = glm::vec3(-stepx, 0, 0);
            } else if (tMaxY < tMaxZ) {
                y += stepy;
                traveled = tMaxY;
                tMaxY += tDeltaY;
                result.normal = glm::vec3(0, -stepy, 0);
            } else {
                z += stepz;
                traveled = tMaxZ;
                tMaxZ += tDeltaZ;
                result.normal = glm::vec3(0, 0, -stepz);
            }
            bool changed = false;
            if (x < 0) { x = 15; chunkpos.x--; changed = true; }
            else if (x > 15) { x = 0; chunkpos.x++; changed = true; }
            if (y < 0) { y = 15; chunkpos.y--; changed = true; }
            else if (y > 15) { y = 0; chunkpos.y++; changed = true; }
            if (z < 0) { z = 15; chunkpos.z--; changed = true; }
            else if (z > 15) { z = 0; chunkpos.z++; changed = true; }
            if (changed) {
                if (!chunks.count(chunkpos)) return;
                chunk = chunks[chunkpos];
            }
        }
    }
};
#endif