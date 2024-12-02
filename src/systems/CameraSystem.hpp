#pragma once

namespace CameraSystem {
    inline void update_desired_camera_position() {
        Registry& registry = MapManager::get_instance().get_active_registry();
        Motion& player_motion = registry.motions.get(registry.player);

        glm::vec2 player_dir = glm::vec2(cos(player_motion.angle), sin(player_motion.angle));
        glm::vec2 ortho_player_dir = glm::normalize(glm::vec2(glm::cross(glm::vec3(player_dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))));

        glm::vec2 desired_camera_pos = player_motion.position - (player_dir * 3.0f) + (1.2f * ortho_player_dir);

        for (auto& e : registry.near_cameras.entities) {
            if (registry.walls.has(e) || (registry.static_objects.has(e) && registry.static_objects.get(e).type == STATIC_OBJECT_TYPE::PORTAL)) {
                auto& pos = registry.motions.get(e).position;
                auto& bound = registry.collision_bounds.get(e).wall->aabb;
                glm::vec2 min = pos + bound.min;
                glm::vec2 max = pos + bound.max;
                bool done = false;
                for (int i = 30; i >= 0; i--) {
                    float portion = i / 30.0f;
                    glm::vec2 point = (player_motion.position - desired_camera_pos) * portion + desired_camera_pos;
                    while (point.x > min.x && point.x < max.x &&
                        point.y > min.y && point.y < max.y) {
                        desired_camera_pos = point + player_dir * 0.1f;
                        done = true;
                        break;
                        }
                    if (done) break;
                }
            } else if (registry.static_objects.has(e)) {
                auto& static_obj = registry.static_objects.get(e);
                if (static_obj.type == STATIC_OBJECT_TYPE::TREE || static_obj.type == STATIC_OBJECT_TYPE::ROCK) { // add other stuff you want camera collision for
                    auto& pos = registry.motions.get(e).position;
                    float radius = registry.collision_bounds.get(e).circle.radius;
                    bool done = false;
                    for (int i = 30; i >= 0; i--) {
                        float portion = i / 30.0f;
                        glm::vec2 point = (player_motion.position - desired_camera_pos) * portion + desired_camera_pos;
                        while (glm::distance(point, pos) < radius) {
                            desired_camera_pos = point + player_dir * 0.1f;
                            done = true;
                            break;
                            }
                        if (done) break;
                    }
                }
            }
        }

        Globals::desired_camera_position = desired_camera_pos;
    }
};