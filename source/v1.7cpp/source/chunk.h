#ifndef CHUNK_H
#define CHUNK_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mesh.h>
#include <block.h>
#include <vector>
#include <map>

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
    glm::vec3 pos;
    unsigned int blocks[16][16][16] = {0};
    Mesh mesh;
    Chunk() {
        pos = {};
    }
    Chunk(glm::vec3 pos) {
        this->pos = pos;
    }
    void generatemesh() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int blockid, neighbourid, nblocks = 0, start;
        int ni, nj, nk;
        Vertex tempv;
        Texture tempt;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                for (int k = 0; k < 16; k++) {
                    blockid = blocks[i][j][k];
                    if (blockid == 0) {
                        continue;
                    }
                    for (int f = 0; f < 6; f++) {
                        start = vertices.size();
                        ni = i+neighbours[f][0];
                        nj = j+neighbours[f][1];
                        nk = k+neighbours[f][2];
                        if (!(ni == -1 || nj == -1 || nk == -1 || ni == 16 || nj == 16 || nk == 16)) {
                            neighbourid = blocks[ni][nj][nk];
                        } else {
                            neighbourid = 0;
                        }
                        if (neighbourid == 0) {
                            for (int l = 0; l < 4; l++) {
                                tempv = blockmeshes[blockid-1].vertices[f*4+l];
                                tempv.pos += glm::vec3((float)i, float(j), float(k)) + pos * 16.0f;
                                //tempv.texid += ntextures;
                                vertices.push_back(tempv);
                            }
                            for (int l = 0; l < 6; l++) {
                                indices.push_back(start + blockmeshes[blockid-1].indices[l]);
                            }
                        }
                    }
                    nblocks++;
                }
            }
        }
        /*for (int l = 0; l < blockmeshes[blockid-1].textures.size(); l++) {
            tempt = blockmeshes[blockid-1].textures[l];
            //tempt.id += ntextures;
            textures.push_back(tempt);
        }*/
       for (int l = 0; l < blockmeshes[0].textures.size(); l++) {
            tempt = blockmeshes[0].textures[l];
            //tempt.id += ntextures;
            textures.push_back(tempt);
        }
        mesh = Mesh(vertices, indices, textures, false);
    }
};

struct vec3compare {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

std::map<glm::vec3, Chunk, vec3compare> chunks;
#endif