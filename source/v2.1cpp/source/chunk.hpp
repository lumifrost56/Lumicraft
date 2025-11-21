#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <mesh.hpp>
#include <block.hpp>
#include <vector>
#include <map>
#include <set>

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

const unsigned int chunk_size = 16;
int render_dist = 3;

int neighbours[6][3] = {
    {0, 0, 1},
    {0, 0, -1},
    {-1, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, -1, 0}
};

glm::ivec3 neighbouring_chunks[6] {
    glm::ivec3(0, 0, 1),
    glm::ivec3(0, 0, -1),
    glm::ivec3(-1, 0, 0),
    glm::ivec3(1, 0, 0),
    glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0)
};

class Chunk;

struct ivec3_compare {
    bool operator()(const glm::ivec3 &a, const glm::ivec3 &b) const {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

std::map<glm::ivec3, Chunk, ivec3_compare> chunks;

class Chunk {
public:
    glm::ivec3 pos;

    unsigned int blocks[chunk_size][chunk_size][chunk_size] = {0};

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh mesh;

    Chunk() {}

    Chunk(glm::ivec3 pos) {
        this->pos = pos;
    }

    void generate_mesh_data() {
        vertices.clear();
        indices.clear();

        unsigned int block_id, neighbour_id;

        Vertex temp_vertex;

        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                for (int k = 0; k < chunk_size; k++) {
                    block_id = blocks[i][j][k];

                    if (block_id == 0) {
                        continue;
                    }

                    for (int f = 0; f < 6; f++) {
                        int ni = i+neighbours[f][0];
                        int nj = j+neighbours[f][1];
                        int nk = k+neighbours[f][2];

                        if (ni == -1 || ni == chunk_size ||
                            nj == -1 || nj == chunk_size ||
                            nk == -1 || nk == chunk_size) {
                            glm::ivec3 new_chunk_pos = pos;

                            if (ni == -1) {
                                new_chunk_pos.x -= 1;
                                ni = chunk_size - 1;
                            }
                            if (ni == chunk_size) {
                                new_chunk_pos.x += 1;
                                ni = 0;
                            }

                            if (nj == -1) {
                                new_chunk_pos.y -= 1;
                                nj = chunk_size - 1;
                            }
                            if (nj == chunk_size) {
                                new_chunk_pos.y += 1;
                                nj = 0;
                            }

                            if (nk == -1) {
                                new_chunk_pos.z -= 1;
                                nk = chunk_size - 1;
                            }
                            if (nk == chunk_size) {
                                new_chunk_pos.z += 1;
                                nk = 0;
                            }

                            if (chunks.count(new_chunk_pos)) {
                                neighbour_id = chunks[new_chunk_pos].blocks[ni][nj][nk];
                            } else {
                                neighbour_id = 0;
                            }
                        } else {
                            neighbour_id = blocks[ni][nj][nk];
                        }

                        unsigned int start = vertices.size();

                        if (neighbour_id == 0) {
                            for (int l = 0; l < 4; l++) {
                                temp_vertex = block_meshes[block_id-1].vertices[f*4+l];
                                temp_vertex.pos += glm::ivec3(i, j, k);

                                vertices.push_back(temp_vertex);
                            }

                            for (int l = 0; l < 6; l++) {
                                indices.push_back(start + block_meshes[block_id-1].indices[l]);
                            }
                        }
                    }
                }
            }
        }
    }

    void create_mesh() {
        mesh = Mesh(vertices, indices);
    }
};

void update_block(glm::ivec3 block_pos, unsigned int block_id) {
    glm::ivec3 chunk_pos = glm::floor(glm::vec3(block_pos) / (float)chunk_size);

    if (!chunks.count(chunk_pos)) {
        return;
    }

    glm::ivec3 local_block_pos = block_pos - chunk_pos * (int)chunk_size;

    int x = local_block_pos.x;
    int y = local_block_pos.y;
    int z = local_block_pos.z;

    chunks[chunk_pos].blocks[x][y][z] = block_id;

    chunks[chunk_pos].generate_mesh_data();
    chunks[chunk_pos].create_mesh();

    if (z == chunk_size-1) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[0];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
    if (z == 0) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[1];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
    if (x == 0) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[2];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
    if (x == chunk_size-1) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[3];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
    if (y == chunk_size-1) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[4];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
    if (y == 0) {
        glm::ivec3 new_chunk_pos = chunk_pos + neighbouring_chunks[5];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
}

std::queue<glm::ivec3> chunks_to_generate;
std::set<glm::ivec3, ivec3_compare> chunks_to_generate_set;
std::queue<Chunk> chunks_to_load;

std::mutex queue_mutex;
std::condition_variable queue_cv;

bool chunkloader_worker_running = true;

void chunkloader_worker() {
    while (chunkloader_worker_running) {
        glm::ivec3 chunk_pos;

        {
            std::unique_lock lock(queue_mutex);
            
            queue_cv.wait(lock, [] {
                return !chunks_to_generate.empty() || !chunkloader_worker_running;
            });

            if (!chunkloader_worker_running) {
                return;
            }

            chunk_pos = chunks_to_generate.front();

            chunks_to_generate.pop();

            chunks_to_generate_set.erase(chunk_pos);
        }

        Chunk chunk(chunk_pos);

        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                chunk.blocks[i][0][j] = 1;
            }
        }
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                chunk.blocks[i][j][0] = 2;
            }
        }
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                chunk.blocks[0][i][j] = 3;
            }
        }

        chunk.generate_mesh_data();

        {
            std::lock_guard lock(queue_mutex);

            chunks_to_load.push(chunk);
        }
    }
}

void update_chunks(glm::vec3 camera_pos) {
    glm::ivec3 camera_chunk_pos = glm::floor(camera_pos / (float)chunk_size);

    std::set<glm::ivec3, ivec3_compare> required;

    for (int x = -render_dist; x <= render_dist; x++) {
        for (int y = -render_dist; y <= render_dist; y++) {
            for (int z = -render_dist; z <= render_dist; z++) {
                required.insert(camera_chunk_pos + glm::ivec3(x, y, z));
            }
        }
    }

    for (auto it = chunks.begin(); it != chunks.end();) {
        if (!required.count(it->first)) {
            it = chunks.erase(it);
        } else {
            it++;
        }
    }

    for (auto &chunk_pos : required) {
        if (!chunks.count(chunk_pos) && !chunks_to_generate_set.count(chunk_pos)) {
            {
                std::lock_guard lock(queue_mutex);

                chunks_to_generate.push(chunk_pos);

                chunks_to_generate_set.insert(chunk_pos);
            }

            queue_cv.notify_one();
        }
    }

    std::lock_guard lock(queue_mutex);

    while (!chunks_to_load.empty()) {
        Chunk &chunk = chunks_to_load.front();

        glm::ivec3 chunk_pos = chunk.pos;

        chunks[chunk_pos] = chunk;

        chunks[chunk_pos].create_mesh();

        chunks_to_load.pop();

        /*for (int i = 0; i < 6; i++) {
            glm::ivec3 neighbouring_chunk_pos = chunk_pos + neighbouring_chunks[i];

            if (chunks.count(neighbouring_chunk_pos)) {
                chunks[neighbouring_chunk_pos].generate_mesh_data();
                chunks[neighbouring_chunk_pos].create_mesh();
            }
        }*/
    }
}

void draw_chunks(Shader &shader, Camera &camera, float aspect_ratio) {
    shader.use();

    glm::mat4 view = camera.get_view_mat();
    shader.set_mat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspect_ratio, 0.1f, 200.0f);
    shader.set_mat4("projection", projection);

    for (auto &[chunk_pos, chunk] : chunks) {
        if (chunk.vertices.empty()) {
            continue;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(chunk_pos * (int)chunk_size));
        shader.set_mat4("model", model);

        chunk.mesh.draw(shader);
    }
}

#endif