#ifndef RAYCASTER_H
#define RAYCASTER_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

        glm::vec3 chunkpos = glm::floor(origin / glm::vec3(chunkxz, chunky, chunkxz));

        glm::vec3 pos = origin - glm::vec3(chunkpos * glm::vec3(chunkxz, chunky, chunkxz));

        Chunk chunk = chunks[chunkpos];

        int x = floor(pos.x);
        int y = floor(pos.y);
        int z = floor(pos.z);

        int stepx = direction.x > 0 ? 1 : -1;
        int stepy = direction.y > 0 ? 1 : -1;
        int stepz = direction.z > 0 ? 1 : -1;

        float tdeltax = direction.x != 0 ? abs(1.0f / direction.x) : FLT_MAX;
        float tdeltay = direction.y != 0 ? abs(1.0f / direction.y) : FLT_MAX;
        float tdeltaz = direction.z != 0 ? abs(1.0f / direction.z) : FLT_MAX;

        float tmaxx = (direction.x > 0) ? ((x + 1 - pos.x) / direction.x) : ((pos.x - x) / -direction.x);
        float tmaxy = (direction.y > 0) ? ((y + 1 - pos.y) / direction.y) : ((pos.y - y) / -direction.y);
        float tmaxz = (direction.z > 0) ? ((z + 1 - pos.z) / direction.z) : ((pos.z - z) / -direction.z);

        float traveled = 0.0f;

        while (traveled < 8.0f) {
            if (x >= 0 && x < chunkxz && y >= 0 && y < chunky && z >= 0 && z < chunkxz) {
                if (chunk.blocks[x][y][z] != 0) {
                    result.hit = true;

                    result.block = glm::vec3(x, y, z) + glm::vec3(chunkpos * glm::vec3(chunkxz, chunky, chunkxz));

                    return;
                }
            }

            if (tmaxx < tmaxy && tmaxx < tmaxz) {
                x += stepx;
                traveled = tmaxx;
                tmaxx += tdeltax;

                result.normal = glm::vec3(-stepx, 0, 0);
            } else if (tmaxy < tmaxz) {
                y += stepy;
                traveled = tmaxy;
                tmaxy += tdeltay;

                result.normal = glm::vec3(0, -stepy, 0);
            } else {
                z += stepz;
                traveled = tmaxz;
                tmaxz += tdeltaz;

                result.normal = glm::vec3(0, 0, -stepz);
            }

            bool changed = false;

            if (x < 0) {
                x = chunkxz-1;
                chunkpos.x--;
                changed = true;
            } else if (x > chunkxz-1) {
                x = 0;
                chunkpos.x++;
                changed = true;
            }

            if (y < 0) {
                y = chunky-1;
                chunkpos.y--;
                changed = true;
            } else if (y > chunky-1) {
                y = 0;
                chunkpos.y++;
                changed = true;
            }

            if (z < 0) {
                z = chunkxz-1;
                chunkpos.z--;
                changed = true;
            } else if (z > chunkxz-1) {
                z = 0;
                chunkpos.z++;
                changed = true;
            }

            if (changed) {
                if (!chunks.count(chunkpos)) {
                    return;
                }

                chunk = chunks[chunkpos];
            }
        }
    }
};

Raycaster raycaster;
#endif