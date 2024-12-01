#pragma once

namespace CameraSystem {
    inline void update_desired_camera_position() {
        Registry& registry = MapManager::get_instance().get_active_registry();
        Motion& player_motion = registry.motions.get(registry.player);

        glm::vec2 player_dir = glm::vec2(cos(player_motion.angle), sin(player_motion.angle));
        glm::vec2 ortho_player_dir = glm::normalize(glm::vec2(glm::cross(glm::vec3(player_dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))));

        glm::vec2 desired_camera_pos = player_motion.position - (player_dir * 3.0f) + (1.2f * ortho_player_dir);

        Globals::desired_camera_position = desired_camera_pos;
    }
};