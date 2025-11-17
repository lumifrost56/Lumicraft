#ifndef CHUNK_H
#define CHUNK_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <blockmesh.h>
#include <camera.h>
#include <vector>
#include <map>
#include <set>

const unsigned int chunkxz = 16, chunky = 64;
int renderdist = 6;

int neighbours[6][3] = {
    {0, 0, 1},
    {0, 0, -1},
    {-1, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, -1, 0}
};

class Chunk {
public:
    unsigned int blocks[chunkxz][chunky][chunkxz] = {0};
    glm::vec3 pos;

    Blockmesh mesh;

    Chunk() {}

    Chunk(glm::vec3 pos) {
        this->pos = pos;
    }

    void generatemesh() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int blockid, neighbourid, start;

        int ni, nj, nk;

        Vertex tempv;

        for (int i = 0; i < chunkxz; i++) {
            for (int j = 0; j < chunky; j++) {
                for (int k = 0; k < chunkxz; k++) {
                    blockid = blocks[i][j][k];

                    if (blockid == 0) {
                        continue;
                    }

                    for (int f = 0; f < 6; f++) {
                        start = vertices.size();

                        ni = i+neighbours[f][0];
                        nj = j+neighbours[f][1];
                        nk = k+neighbours[f][2];

                        if (ni == -1 || nj == -1 || nk == -1 || ni == chunkxz || nj == chunky || nk == chunkxz) {
                            neighbourid = 0;
                        } else {
                            neighbourid = blocks[ni][nj][nk];
                        }

                        if (neighbourid == 0) {
                            for (int l = 0; l < 4; l++) {
                                tempv = blockmeshes[blockid-1].vertices[f*4+l];
                                tempv.pos += glm::vec3((float)i, (float)j, (float)k) + pos * (float)chunkxz;
                                
                                vertices.push_back(tempv);
                            }

                            for (int l = 0; l < 6; l++) {
                                indices.push_back(start + blockmeshes[blockid-1].indices[l]);
                            }
                        }
                    }
                }
            }
        }
        mesh = Blockmesh(vertices, indices);
    }
};

struct vec3compare {
    bool operator()(const glm::vec3 &a, const glm::vec3 &b) const {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

std::map<glm::vec3, Chunk, vec3compare> chunks;

void updatechunks(glm::vec3 pos) {
    glm::vec3 camerachunkpos = glm::floor(pos / glm::vec3(chunkxz, chunky, chunkxz));

    camerachunkpos = glm::vec3(camerachunkpos.x, 0.0f, camerachunkpos.y);

    std::set<glm::vec3, vec3compare> required;

    for (int x = -renderdist; x <= renderdist; x++) {
        for (int z = -renderdist; z <= renderdist; z++) {
            required.insert(camerachunkpos + glm::vec3(x, 0.0f, z));
        }
    }

    for (auto it = chunks.begin(); it != chunks.end();) {
        if (!required.count(it->first)) {
            it = chunks.erase(it);
        } else {
            it++;
        }
    }

    for (auto &chunkpos : required) {
        if(!chunks.count(chunkpos)) {
            Chunk chunk(chunkpos);

            chunk.blocks[0][0][0] = 1;

            chunk.generatemesh();

            chunks[chunkpos] = chunk;
        }
    }
}
#endif