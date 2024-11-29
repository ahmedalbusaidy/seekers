#pragma once
#include "utils/RegistrySerializer.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include "app/MapManager.hpp"

#ifdef _WIN32
    #include <direct.h>
#endif

using json = nlohmann::json;

struct SaveSlot {
    int id;                     // Unique incremental ID
    std::string name;           // User-given name
    std::string filename;       // Actual file name on disk
    std::time_t timestamp;      // When save was created
    std::string version;        // Game version
};

class SaveLoadSystem {
public:
    static SaveLoadSystem& get_instance() {
        static SaveLoadSystem instance;
        return instance;
    }

    std::vector<SaveSlot> list_save_slots() {
        return save_slots;
    }

    SaveSlot create_new_slot(const std::string& name) {
        SaveSlot slot;
        slot.id = next_slot_id++;
        slot.name = name;
        slot.filename = "save_" + std::to_string(slot.id) + ".json";
        slot.timestamp = std::time(nullptr);
        slot.version = "1.0.0";
        save_slots.push_back(slot);
        save_index_file();
        return slot;
    }

    bool save_game_to_slot(const SaveSlot& slot) {
        try {
            json save_data;
            save_data["version"] = slot.version;
            save_data["timestamp"] = slot.timestamp;
            
            // Save MapManager state
            save_data["map_manager"] = serialize_map_manager();
            
            #ifdef _WIN32
                _mkdir("saves");
            #else
                mkdir("saves", 0777);
            #endif
            
            std::ofstream file("saves/" + slot.filename);
            file << std::setw(4) << save_data << std::endl;
            return true;
        } catch (const std::exception& e) {
            Log::log_warning("Save failed: " + std::string(e.what()), __FILE__, __LINE__);
            return false;
        }
    }

    bool load_game_from_slot(const SaveSlot& slot) {
        try {
            std::ifstream file("saves/" + slot.filename);
            if (!file.is_open()) {
                Log::log_info("Save file not found: " + slot.filename, __FILE__, __LINE__);
                return false;
            }

            json save_data = json::parse(file);
            
            if (!save_data.contains("map_manager") || 
                !save_data.contains("version") || 
                !save_data.contains("timestamp")) {
                throw SerializationError("Invalid save file format");
            }
            
            // Version check
            if (save_data["version"] != slot.version) {
                Log::log_warning("Save version mismatch", __FILE__, __LINE__);
            }
            
            // Deserialize MapManager state
            deserialize_map_manager(save_data["map_manager"]);
            
            return true;
        } catch (const std::exception& e) {
            Log::log_warning("Load failed: " + std::string(e.what()), __FILE__, __LINE__);
            return false;
        }
    }

    // Static methods for direct access
    static bool save_game(Registry& registry, const std::string& save_name="untitled") {
        auto& instance = get_instance();
        SaveSlot slot = instance.create_new_slot(save_name);
        return instance.save_game_to_slot(slot);
    }

    static bool load_latest_game(Registry& registry) {
        const auto& slots = get_instance().list_save_slots();
        if (slots.empty()) {
            Log::log_info("No save slots found (SaveLoadSystem::load_latest_game)", __FILE__, __LINE__);
            return false;
        }

        // Find slot with latest timestamp
        const SaveSlot* latest_slot = &slots[0];
        for (const auto& slot : slots) {
            if (slot.timestamp > latest_slot->timestamp) {
                latest_slot = &slot;
            }
        }
        
        return load_game(registry, latest_slot->name);
    }

    static bool load_game(Registry& registry, const std::string& save_name="") {
        if (save_name.empty()) {
            return load_latest_game(registry);
        }

        // Find existing slot with matching name
        const auto& slots = get_instance().list_save_slots();
        for (const auto& slot : slots) {
            if (slot.name == save_name) {
                return get_instance().load_game_from_slot(slot);
            }
        }

        Log::log_info("No save slot found with name: " + save_name, __FILE__, __LINE__);
        return false;
    }

private:
    SaveLoadSystem() {
        load_index_file();
    }

    std::vector<SaveSlot> save_slots;
    int next_slot_id = 0;

    void save_index_file() {
        try {
        #ifdef _WIN32
            _mkdir("saves");
        #else
            mkdir("saves", 0777);
        #endif

        json index;
            index["next_slot_id"] = next_slot_id;
            
            std::vector<json> slots_data;
            for (const auto& slot : save_slots) {
                slots_data.push_back({
                    {"id", slot.id},
                    {"name", slot.name},
                    {"filename", slot.filename},
                    {"timestamp", slot.timestamp},
                    {"version", slot.version}
                });
            }
            index["slots"] = slots_data;
            
            std::ofstream file("saves/index.json");
            file << std::setw(4) << index << std::endl;
        } catch (const std::exception& e) {
            Log::log_warning("Failed to save index file (SaveLoadSystem::save_index_file): " + std::string(e.what()), __FILE__, __LINE__);
        }
    }

    void load_index_file() {
        try {
            std::ifstream file("saves/index.json");
            if (!file.is_open()) {
                next_slot_id = 0;
                save_slots.clear();
                return;
            }

            json index = json::parse(file);
            next_slot_id = index["next_slot_id"];
            save_slots.clear();
            
            for (const auto& slot_data : index["slots"]) {
                SaveSlot slot{
                    slot_data["id"],
                    slot_data["name"],
                    slot_data["filename"],
                    slot_data["timestamp"],
                    slot_data["version"]
                };
                save_slots.push_back(slot);
            }
        } catch (const std::exception& e) {
            Log::log_warning("Failed to load index file (SaveLoadSystem::load_index_file): " + std::string(e.what()), __FILE__, __LINE__);
            next_slot_id = 0;
            save_slots.clear();
        }
    }

    json serialize_map_manager() {
        auto& map_manager = MapManager::get_instance();
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

    void deserialize_map_manager(const json& map_data) {
        auto& map_manager = MapManager::get_instance();

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
};
