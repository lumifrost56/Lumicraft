#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include <glm/glm.hpp>
#include <settings.hpp>
#include <utils.hpp>
#include <blockmesh.hpp>
#include <blocks.hpp>
#include <camera.hpp>
#include <vector>
#include <map>
#include <set>

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

const int chunk_size = 16;
int render_dist = 5;

glm::ivec3 neighbours[6] = {
    glm::ivec3(0, 0, 1),
    glm::ivec3(0, 0, -1),
    glm::ivec3(-1, 0, 0),
    glm::ivec3(1, 0, 0),
    glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0)
};

struct Chunk;

std::map<glm::ivec3, Chunk, ivec3_compare> chunks;

struct Chunk {
    glm::ivec3 pos;

    uint8_t blocks[chunk_size][chunk_size][chunk_size] = {AIR};

    Blockmesh mesh;

    bool has_block_data = false;
    bool has_mesh_data = false;
    bool mesh_created = false;

    Chunk() {}

    bool has_all_neighbours() {
        for (int i = 0; i < 6; i++) {
            glm::ivec3 new_chunk_pos = pos + neighbours[i];

            if (!chunks.count(new_chunk_pos)) {
                return false;
            }

            if (!chunks[new_chunk_pos].has_block_data) {
                return false;
            }
        }

        return true;
    }

    uint8_t get_neighbour_block(int x, int y, int z, int face) {
        int ni = x + neighbours[face].x;
        int nj = y + neighbours[face].y;
        int nk = z + neighbours[face].z;

        if (ni == -1 || ni == chunk_size ||
            nj == -1 || nj == chunk_size ||
            nk == -1 || nk == chunk_size) {
            glm::ivec3 new_chunk_pos = pos;

            if (ni == -1) {
                new_chunk_pos.x -= 1;
                ni = chunk_size - 1;
            } else if (ni == chunk_size) {
                new_chunk_pos.x += 1;
                ni = 0;
            }

            if (nj == -1) {
                new_chunk_pos.y -= 1;
                nj = chunk_size - 1;
            } else if (nj == chunk_size) {
                new_chunk_pos.y += 1;
                nj = 0;
            }

            if (nk == -1) {
                new_chunk_pos.z -= 1;
                nk = chunk_size - 1;
            } else if (nk == chunk_size) {
                new_chunk_pos.z += 1;
                nk = 0;
            }

            if (chunks.count(new_chunk_pos)) {
                return chunks[new_chunk_pos].blocks[ni][nj][nk];
            }
        } else {
            return blocks[ni][nj][nk];
        }

        return AIR;
    }

    void generate_block_data() {
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                blocks[i][0][j] = ICE;
            }
        }
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                blocks[i][j][0] = STONE;
            }
        }
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < chunk_size; j++) {
                blocks[0][i][j] = TUFF;
            }
        }

        has_block_data = true;
    }

    void generate_mesh_data() {
        if (!has_all_neighbours()) {
            return;
        }

        mesh.vertices.clear();
        mesh.indices.clear();

        for (int face = 0; face < 6; face++) {
            bool face_mask[chunk_size][chunk_size][chunk_size] = {false};

            for (int x = 0; x < chunk_size; x++) {
                for (int y = 0; y < chunk_size; y++) {
                    for (int z = 0; z < chunk_size; z++) {
                        uint8_t block_id = blocks[x][y][z];

                        if (block_id == AIR) {
                            continue;
                        }

                        uint8_t neighbour_id = get_neighbour_block(x, y, z, face);

                        if (neighbour_id == AIR) {
                            face_mask[x][y][z] = true;
                        }
                    }
                }
            }

            switch (face) {
                case 0: greedy_mesh_face_z(face, face_mask, true); break;
                case 1: greedy_mesh_face_z(face, face_mask, false); break;
                case 2: greedy_mesh_face_x(face, face_mask, false); break;
                case 3: greedy_mesh_face_x(face, face_mask, true); break;
                case 4: greedy_mesh_face_y(face, face_mask, true); break;
                case 5: greedy_mesh_face_y(face, face_mask, false); break;
            }
        }

        has_mesh_data = true;
    }

    void greedy_mesh_face_z(int face, bool face_mask[chunk_size][chunk_size][chunk_size], bool positive) {
        for (int z = 0; z < chunk_size; z++) {
            bool mask[chunk_size][chunk_size] = {false};

            uint8_t block_ids[chunk_size][chunk_size] = {AIR};

            for (int x = 0; x < chunk_size; x++) {
                for (int y = 0; y < chunk_size; y++) {
                    if (face_mask[x][y][z]) {
                        mask[x][y] = true;

                        block_ids[x][y] = blocks[x][y][z];
                    }
                }
            }

            for (int x = 0; x < chunk_size; x++) {
                for (int y = 0; y < chunk_size; y++) {
                    if (!mask[x][y]) {
                        continue;
                    }

                    uint8_t current_block = block_ids[x][y];

                    int width_x = 1;
                    int height = 1;

                    while (x + width_x < chunk_size &&
                           mask[x + width_x][y] &&
                           block_ids[x + width_x][y] == current_block) {
                        width_x++;
                    }

                    bool can_extend = true;

                    while (y + height < chunk_size && can_extend) {
                        for (int w = 0; w < width_x; w++) {
                            if (!mask[x + w][y + height] ||
                                block_ids[x + w][y + height] != current_block) {
                                can_extend = false;
                                break;
                            }
                        }

                        if (can_extend) {
                            height++;
                        }
                    }

                    for (int w = 0; w < width_x; w++) {
                        for (int h = 0; h < height; h++) {
                            mask[x + w][y + h] = false;
                        }
                    }

                    create_quad(face, x, y, z, width_x, 1, height, current_block);
                }
            }
        }
    }

    void greedy_mesh_face_x(int face, bool face_mask[chunk_size][chunk_size][chunk_size], bool positive) {
        for (int x = 0; x < chunk_size; x++) {
            bool mask[chunk_size][chunk_size] = {false};

            uint8_t block_ids[chunk_size][chunk_size] = {AIR};

            for (int z = 0; z < chunk_size; z++) {
                for (int y = 0; y < chunk_size; y++) {
                    if (face_mask[x][y][z]) {
                        mask[z][y] = true;

                        block_ids[z][y] = blocks[x][y][z];
                    }
                }
            }

            for (int z = 0; z < chunk_size; z++) {
                for (int y = 0; y < chunk_size; y++) {
                    if (!mask[z][y]) {
                        continue;
                    }

                    uint8_t current_block = block_ids[z][y];

                    int width_z = 1;
                    int height = 1;

                    while (z + width_z < chunk_size &&
                           mask[z + width_z][y] &&
                           block_ids[z + width_z][y] == current_block) {
                        width_z++;
                    }

                    bool can_extend = true;

                    while (y + height < chunk_size && can_extend) {
                        for (int w = 0; w < width_z; w++) {
                            if (!mask[z + w][y + height] ||
                                block_ids[z + w][y + height] != current_block) {
                                can_extend = false;
                                break;
                            }
                        }

                        if (can_extend) {
                            height++;
                        }
                    }

                    for (int w = 0; w < width_z; w++) {
                        for (int h = 0; h < height; h++) {
                            mask[z + w][y + h] = false;
                        }
                    }

                    create_quad(face, x, y, z, 1, width_z, height, current_block);
                }
            }
        }
    }

    void greedy_mesh_face_y(int face, bool face_mask[chunk_size][chunk_size][chunk_size], bool positive) {
        for (int y = 0; y < chunk_size; y++) {
            bool mask[chunk_size][chunk_size] = {false};

            uint8_t block_ids[chunk_size][chunk_size] = {AIR};

            for (int x = 0; x < chunk_size; x++) {
                for (int z = 0; z < chunk_size; z++) {
                    if (face_mask[x][y][z]) {
                        mask[x][z] = true;

                        block_ids[x][z] = blocks[x][y][z];
                    }
                }
            }

            for (int x = 0; x < chunk_size; x++) {
                for (int z = 0; z < chunk_size; z++) {
                    if (!mask[x][z]) {
                        continue;
                    }

                    uint8_t current_block = block_ids[x][z];

                    int width_x = 1;
                    int width_z = 1;

                    while (x + width_x < chunk_size &&
                           mask[x + width_x][z] &&
                           block_ids[x + width_x][z] == current_block) {
                        width_x++;
                    }

                    bool can_extend = true;

                    while (z + width_z < chunk_size && can_extend) {
                        for (int w = 0; w < width_x; w++) {
                            if (!mask[x + w][z + width_z] ||
                                block_ids[x + w][z + width_z] != current_block) {
                                can_extend = false;
                                break;
                            }
                        }

                        if (can_extend) {
                            width_z++;
                        }
                    }

                    for (int w = 0; w < width_x; w++) {
                        for (int h = 0; h < width_z; h++) {
                            mask[x + w][z + h] = false;
                        }
                    }

                    create_quad(face, x, y, z, width_x, width_z, 1, current_block);
                }
            }
        }
    }

    void create_quad(int face, int start_x, int start_y, int start_z, int width_x, int width_z, int height, uint8_t block_id) {
        int start_offset = mesh.vertices.size();

        Blockmesh block_mesh = block_meshes[block_id - 1];

        Vertex temp_vertex;

        glm::vec3 corners[4];
        glm::vec2 uvs[4];

        switch (face) {
            case 0:
                corners[0] = glm::vec3(start_x, start_y, start_z + 1);
                corners[1] = glm::vec3(start_x, start_y + height, start_z + 1);
                corners[2] = glm::vec3(start_x + width_x, start_y + height, start_z + 1);
                corners[3] = glm::vec3(start_x + width_x, start_y, start_z + 1);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, height);
                uvs[2] = glm::vec2(width_x, height);
                uvs[3] = glm::vec2(width_x, 0.0f);
                break;
            case 1:
                corners[3] = glm::vec3(start_x, start_y, start_z);
                corners[2] = glm::vec3(start_x, start_y + height, start_z);
                corners[1] = glm::vec3(start_x + width_x, start_y + height, start_z);
                corners[0] = glm::vec3(start_x + width_x, start_y, start_z);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, height);
                uvs[2] = glm::vec2(width_x, height);
                uvs[3] = glm::vec2(width_x, 0.0f);
                break;
            case 2:
                corners[0] = glm::vec3(start_x, start_y, start_z);
                corners[1] = glm::vec3(start_x, start_y + height, start_z);
                corners[2] = glm::vec3(start_x, start_y + height, start_z + width_z);
                corners[3] = glm::vec3(start_x, start_y, start_z + width_z);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, height);
                uvs[2] = glm::vec2(width_z, height);
                uvs[3] = glm::vec2(width_z, 0.0f);
                break;
            case 3:
                corners[3] = glm::vec3(start_x + 1, start_y, start_z);
                corners[2] = glm::vec3(start_x + 1, start_y + height, start_z);
                corners[1] = glm::vec3(start_x + 1, start_y + height, start_z + width_z);
                corners[0] = glm::vec3(start_x + 1, start_y, start_z + width_z);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, height);
                uvs[2] = glm::vec2(width_z, height);
                uvs[3] = glm::vec2(width_z, 0.0f);
                break;
            case 4:
                corners[0] = glm::vec3(start_x, start_y + 1, start_z + width_z);
                corners[1] = glm::vec3(start_x, start_y + 1, start_z);
                corners[2] = glm::vec3(start_x + width_x, start_y + 1, start_z);
                corners[3] = glm::vec3(start_x + width_x, start_y + 1, start_z + width_z);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, width_z);
                uvs[2] = glm::vec2(width_x, width_z);
                uvs[3] = glm::vec2(width_x, 0.0f);
                break;
            case 5:
                corners[0] = glm::vec3(start_x, start_y, start_z);
                corners[1] = glm::vec3(start_x, start_y, start_z + width_z);
                corners[2] = glm::vec3(start_x + width_x, start_y, start_z + width_z);
                corners[3] = glm::vec3(start_x + width_x, start_y, start_z);
                uvs[0] = glm::vec2(0.0f, 0.0f);
                uvs[1] = glm::vec2(0.0f, width_z);
                uvs[2] = glm::vec2(width_x, width_z);
                uvs[3] = glm::vec2(width_x, 0.0f);
                break;
        }

        for (int i = 0; i < 4; i++) {
            temp_vertex.pos = corners[i];
            temp_vertex.normal = block_mesh.vertices[face * 4 + i].normal;
            temp_vertex.uv = uvs[i];
            temp_vertex.tex_id = block_mesh.vertices[face * 4 + i].tex_id;

            mesh.vertices.push_back(temp_vertex);
        }

        for (int i = 0; i < 6; i++) {
            mesh.indices.push_back(start_offset + block_mesh.indices[i]);
        }
    }

    void create_mesh() {
        mesh.delete_self();
        mesh.create_self();

        mesh_created = true;
    }

    void get_aabb(glm::vec3 &minv, glm::vec3 &maxv) {
        glm::vec3 world_pos = glm::vec3(pos) * (float)chunk_size;

        minv = world_pos;
        maxv = world_pos + (float)chunk_size;
    }
};

uint8_t get_block_id(glm::ivec3 world_pos) {
    glm::ivec3 chunk_pos = glm::floor(glm::vec3(world_pos) / (float)chunk_size);

    if (!chunks.count(chunk_pos)) {
        return AIR;
    }

    glm::ivec3 local_pos = world_pos - chunk_pos * chunk_size;

    return chunks[chunk_pos].blocks[local_pos.x][local_pos.y][local_pos.z];
}

void update_block(glm::ivec3 block_pos, uint8_t block_id) {
    glm::ivec3 chunk_pos = glm::floor(glm::vec3(block_pos) / (float)chunk_size);

    if (!chunks.count(chunk_pos)) {
        return;
    }

    glm::ivec3 local_pos = block_pos - chunk_pos * chunk_size;

    int x = local_pos.x;
    int y = local_pos.y;
    int z = local_pos.z;

    chunks[chunk_pos].blocks[x][y][z] = block_id;

    chunks[chunk_pos].generate_mesh_data();
    chunks[chunk_pos].create_mesh();

    glm::ivec3 new_chunk_pos;

    if (z == chunk_size - 1) {
        new_chunk_pos = chunk_pos + neighbours[0];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    } else if (z == 0) {
        new_chunk_pos = chunk_pos + neighbours[1];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }

    if (x == 0) {
        new_chunk_pos = chunk_pos + neighbours[2];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    } else if (x == chunk_size - 1) {
        new_chunk_pos = chunk_pos + neighbours[3];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }

    if (y == chunk_size - 1) {
        new_chunk_pos = chunk_pos + neighbours[4];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    } else if (y == 0) {
        new_chunk_pos = chunk_pos + neighbours[5];

        if (chunks.count(new_chunk_pos)) {
            chunks[new_chunk_pos].generate_mesh_data();
            chunks[new_chunk_pos].create_mesh();
        }
    }
}

std::set<glm::ivec3, ivec3_compare> chunks_pending_blocks;
std::set<glm::ivec3, ivec3_compare> chunks_pending_mesh;

std::queue<glm::ivec3> chunks_to_generate_blocks;
std::queue<glm::ivec3> chunks_to_generate_mesh;

std::mutex blocks_queue_mutex;
std::mutex mesh_queue_mutex;

std::condition_variable blocks_queue_cv;
std::condition_variable mesh_queue_cv;

bool blocks_worker_running = true;
bool mesh_worker_running = true;

void block_generation_worker() {
    while (blocks_worker_running) {
        glm::ivec3 chunk_pos;

        {
            std::unique_lock lock(blocks_queue_mutex);

            blocks_queue_cv.wait(lock, [] {
                return !chunks_to_generate_blocks.empty() || !blocks_worker_running;
            });

            if (!blocks_worker_running) {
                return;
            }

            chunk_pos = chunks_to_generate_blocks.front();

            chunks_to_generate_blocks.pop();
        }

        Chunk chunk;
        chunk.pos = chunk_pos;
        chunk.generate_block_data();

        {
            std::lock_guard lock(blocks_queue_mutex);

            chunks[chunk_pos] = chunk;

            chunks_pending_blocks.erase(chunk_pos);
        }

        {
            std::lock_guard lock(mesh_queue_mutex);

            if (chunks[chunk_pos].has_all_neighbours() &&
                !chunks_pending_mesh.count(chunk_pos)) {
                chunks_pending_mesh.insert(chunk_pos);

                chunks_to_generate_mesh.push(chunk_pos);

                mesh_queue_cv.notify_one();
            }
        }

        for (int i = 0; i < 6; i++) {
            glm::ivec3 new_chunk_pos = chunk_pos + neighbours[i];

            std::lock_guard lock(mesh_queue_mutex);

            if (chunks.count(new_chunk_pos) && !chunks_pending_mesh.count(new_chunk_pos)) {
                if (!chunks[new_chunk_pos].has_mesh_data) {
                    if (chunks[new_chunk_pos].has_all_neighbours()) {
                        chunks_pending_mesh.insert(new_chunk_pos);

                        chunks_to_generate_mesh.push(new_chunk_pos);

                        mesh_queue_cv.notify_one();
                    }
                }
            }
        }
    }
}

void mesh_generation_worker() {
    while (mesh_worker_running) {
        glm::ivec3 chunk_pos;

        {
            std::unique_lock lock(mesh_queue_mutex);

            mesh_queue_cv.wait(lock, [] {
                return !chunks_to_generate_mesh.empty() || !mesh_worker_running;
            });

            if (!mesh_worker_running) {
                return;
            }

            chunk_pos = chunks_to_generate_mesh.front();

            chunks_to_generate_mesh.pop();
        }

        if (!chunks.count(chunk_pos)) {
            std::lock_guard lock(mesh_queue_mutex);

            chunks_pending_mesh.erase(chunk_pos);

            continue;
        }

        chunks[chunk_pos].generate_mesh_data();

        if (chunks[chunk_pos].has_mesh_data) {
            std::lock_guard lock(mesh_queue_mutex);

            chunks_pending_mesh.erase(chunk_pos);
        }
    }
}

void update_chunks(glm::vec3 player_pos) {
    glm::ivec3 player_chunk_pos = glm::floor(player_pos / (float)chunk_size);

    std::set<glm::ivec3, ivec3_compare> required;

    for (int x = -render_dist; x <= render_dist; x++) {
        for (int y = -render_dist; y <= render_dist; y++) {
            for (int z = -render_dist; z <= render_dist; z++) {
                required.insert(player_chunk_pos + glm::ivec3(x, y, z));
            }
        }
    }

    for (auto it = chunks.begin(); it != chunks.end();) {
        if (!required.count(it->first)) {
            it->second.mesh.delete_self();

            {
                std::lock_guard lock1(blocks_queue_mutex);

                chunks_pending_blocks.erase(it->first);
            }
            {
                std::lock_guard lock2(mesh_queue_mutex);

                chunks_pending_mesh.erase(it->first);
            }

            it = chunks.erase(it);
        } else {
            it++;
        }
    }

    for (auto &chunk_pos : required) {
        if (!chunks.count(chunk_pos) && !chunks_pending_blocks.count(chunk_pos)) {
            {
                std::lock_guard lock(blocks_queue_mutex);

                chunks_pending_blocks.insert(chunk_pos);

                chunks_to_generate_blocks.push(chunk_pos);
            }

            blocks_queue_cv.notify_one();
        }
    }

    int counter = 0;

    for (auto &[chunk_pos, chunk] : chunks) {
        if (counter == 5) {
            break;
        }

        if (chunk.has_mesh_data && !chunk.mesh_created) {
            chunk.create_mesh();

            counter++;
        }
    }
}

void init_chunk_workers() {
    std::thread block_worker(block_generation_worker);
    std::thread mesh_worker(mesh_generation_worker);

    block_worker.detach();
    mesh_worker.detach();
}

void cleanup_chunk_workers() {
    {
        std::lock_guard lock(blocks_queue_mutex);

        blocks_worker_running = false;
    }
    {
        std::lock_guard lock(mesh_queue_mutex);

        mesh_worker_running = false;
    }

    blocks_queue_cv.notify_all();
    mesh_queue_cv.notify_all();
}

void draw_chunks(Camera &camera) {
    for (auto &[chunk_pos, chunk] : chunks) {
        if (!chunk.mesh_created || chunk.mesh.vertices.empty()) {
            continue;
        }

        glm::vec3 minv;
        glm::vec3 maxv;

        chunk.get_aabb(minv, maxv);

        if (!camera.frustum.is_aabb_visible(minv, maxv)) {
            continue;
        }

        chunk.mesh.draw(chunk_pos * chunk_size, camera);
    }
}

#endif