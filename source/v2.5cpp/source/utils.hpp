#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <algorithm>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
    float tex_id;
};

struct ivec3_compare {
    bool operator()(const glm::ivec3 &a, const glm::ivec3 &b) const {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        if (a.y != b.y) {
            return a.y < b.y;
        }
        return a.z < b.z;
    }
};

inline bool aabb_intersect(glm::vec3 a_min, glm::vec3 a_max,
                    glm::vec3 b_min, glm::vec3 b_max) {
    return (a_min.x < b_max.x && a_max.x > b_min.x &&
            a_min.y < b_max.y && a_max.y > b_min.y &&
            a_min.z < b_max.z && a_max.z > b_min.z);
}

inline bool swept_aabb_intersect(glm::vec3 a_min, glm::vec3 a_max,
                          glm::vec3 b_min, glm::vec3 b_max,
                          glm::vec3 velocity,
                          float &coll_time,
                          glm::vec3 &normal) {
    coll_time = 1.0f;

    normal = glm::vec3(0.0f);

    if (velocity == glm::vec3(0.0f)) {
        if (aabb_intersect(a_min, a_max, b_min, b_max)) {
            coll_time = 0.0f;

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

    glm::vec3 t_entry;
    glm::vec3 t_exit;

    if (velocity.x >= 0) {
        t_entry.x = (b_min.x - a_max.x) / (velocity.x != 0 ? velocity.x : 0.0001f);
        t_exit.x = (b_max.x - a_min.x) / (velocity.x != 0 ? velocity.x : 0.0001f);
    } else {
        t_entry.x = (b_max.x - a_min.x) / velocity.x;
        t_exit.x = (b_min.x - a_max.x) / velocity.x;
    }

    if (velocity.y >= 0) {
        t_entry.y = (b_min.y - a_max.y) / (velocity.y != 0 ? velocity.y : 0.0001f);
        t_exit.y = (b_max.y - a_min.y) / (velocity.y != 0 ? velocity.y : 0.0001f);
    } else {
        t_entry.y = (b_max.y - a_min.y) / velocity.y;
        t_exit.y = (b_min.y - a_max.y) / velocity.y;
    }

    if (velocity.z >= 0) {
        t_entry.z = (b_min.z - a_max.z) / (velocity.z != 0 ? velocity.z : 0.0001f);
        t_exit.z = (b_max.z - a_min.z) / (velocity.z != 0 ? velocity.z : 0.0001f);
    } else {
        t_entry.z = (b_max.z - a_min.z) / velocity.z;
        t_exit.z = (b_min.z - a_max.z) / velocity.z;
    }

    float entry_time = std::max({t_entry.x, t_entry.y, t_entry.z});
    float exit_time = std::min({t_exit.x, t_exit.y, t_exit.z});

    if (entry_time > exit_time || entry_time < 0.0f || entry_time > 1.0f) {
        return false;
    }

    if (entry_time == t_entry.x) {
        normal = glm::vec3(velocity.x >= 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f);
    } else if (entry_time == t_entry.y) {
        normal = glm::vec3(0.0f, velocity.y >= 0.0f ? -1.0f : 1.0f, 0.0f);
    } else if (entry_time == t_entry.z) {
        normal = glm::vec3(0.0f, 0.0f, velocity.z >= 0.0f ? -1.0f : 1.0f);
    }

    coll_time = entry_time;
    return true;
}

#endif