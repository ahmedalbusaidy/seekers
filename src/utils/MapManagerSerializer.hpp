#pragma once
#include "app/MapManager.hpp"
#include "RegistrySerializer.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MapManagerSerializer {
public:
    static json serialize_map_manager(MapManager& map_manager) {
        json map_data;

        // Serialize flags and state
        map_data["return_open_world_flag"] = map_manager.return_open_world_flag;
        map_data["enter_dungeon_flag"] = map_manager.enter_dungeon_flag;
        map_data["dungeon_difficulty"] = map_manager.dungeon_difficulty;
        map_data["sky_texture_name"] = map_manager.sky_texture_name;
        map_data["wall_texture_name"] = map_manager.wall_texture_name;
        map_data["floor_texture_name"] = map_manager.floor_texture_name;

        // Save which registry is active
        if (map_manager.active_registry == map_manager.open_world_registry.get()) {
            map_data["active_registry"] = "open_world";
        } else if (map_manager.active_registry == map_manager.dungeon_registry.get()) {
            map_data["active_registry"] = "dungeon";
        }

        // Serialize registries
        if (map_manager.open_world_registry) {
            map_data["open_world_registry"] = RegistrySerializer::serialize_registry(*map_manager.open_world_registry);
        }
        if (map_manager.dungeon_registry) {
            map_data["dungeon_registry"] = RegistrySerializer::serialize_registry(*map_manager.dungeon_registry);
        }
        if (map_manager.saved_world_registry) {
            map_data["saved_world_registry"] = RegistrySerializer::serialize_registry(*map_manager.saved_world_registry);
        }

        return map_data;
    }

    static void deserialize_map_manager(MapManager& map_manager, const json& map_data) {
        // Deserialize flags and state
        map_manager.return_open_world_flag = map_data["return_open_world_flag"];
        map_manager.enter_dungeon_flag = map_data["enter_dungeon_flag"];
        map_manager.dungeon_difficulty = map_data["dungeon_difficulty"];
        map_manager.sky_texture_name = map_data["sky_texture_name"];
        map_manager.wall_texture_name = map_data["wall_texture_name"];
        map_manager.floor_texture_name = map_data["floor_texture_name"];

        // Deserialize registries
        if (map_data.contains("open_world_registry")) {
            if (!map_manager.open_world_registry) {
                map_manager.open_world_registry = std::make_unique<Registry>();
            }
            RegistrySerializer::deserialize_registry(*map_manager.open_world_registry, map_data["open_world_registry"]);
        }

        if (map_data.contains("dungeon_registry")) {
            if (!map_manager.dungeon_registry) {
                map_manager.dungeon_registry = std::make_unique<Registry>();
            }
            RegistrySerializer::deserialize_registry(*map_manager.dungeon_registry, map_data["dungeon_registry"]);
        }

        if (map_data.contains("saved_world_registry")) {
            if (!map_manager.saved_world_registry) {
                map_manager.saved_world_registry = std::make_unique<Registry>();
            }
            RegistrySerializer::deserialize_registry(*map_manager.saved_world_registry, map_data["saved_world_registry"]);
        }

        // Restore active registry pointer
        if (map_data.contains("active_registry")) {
            std::string active_reg = map_data["active_registry"];
            if (active_reg == "open_world") {
                map_manager.active_registry = map_manager.open_world_registry.get();
            } else if (active_reg == "dungeon") {
                map_manager.active_registry = map_manager.dungeon_registry.get();
            }
        } else {
            // Default to open world if not specified
            map_manager.active_registry = map_manager.open_world_registry.get();
        }

        // Make sure to set the theme based on which registry is active
        if (map_manager.active_registry == map_manager.open_world_registry.get()) {
            map_manager.set_theme("OpenWorld");
        } else if (map_manager.active_registry == map_manager.dungeon_registry.get()) {
            map_manager.set_theme("Dungeon");
        }

        // Force renderer restart to apply theme changes
        Globals::restart_renderer = true;
    }

private:
    // Private constructor to prevent instantiation
    MapManagerSerializer() = default;
}; 