#pragma once
#include "utils/RegistrySerializer.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include "app/MapManager.hpp"
#include "utils/MapManagerSerializer.hpp"

#ifdef _WIN32
    #include <direct.h>
#endif

using json = nlohmann::json;

class SaveLoadSystem {
public:
    static SaveLoadSystem& get_instance() {
        static SaveLoadSystem instance;
        return instance;
    }

    bool save_game(Registry& registry) {
        try {
            json save_data;
            save_data["version"] = "1.0.0";
            save_data["timestamp"] = std::time(nullptr);
            
            // Save MapManager state using the serializer
            save_data["map_manager"] = MapManagerSerializer::serialize_map_manager(MapManager::get_instance());
            
            #ifdef _WIN32
                _mkdir("saves");
            #else
                mkdir("saves", 0777);
            #endif
            
            std::ofstream file("saves/save.json");
            file << std::setw(4) << save_data << std::endl;
            return true;
        } catch (const std::exception& e) {
            Log::log_warning("Save failed: " + std::string(e.what()), __FILE__, __LINE__);
            return false;
        }
    }

    bool load_game(Registry& registry) {
        try {
            std::ifstream file("saves/save.json");
            if (!file.is_open()) {
                Log::log_info("No save file found", __FILE__, __LINE__);
                return false;
            }

            json save_data = json::parse(file);
            
            if (!save_data.contains("map_manager") || 
                !save_data.contains("version") || 
                !save_data.contains("timestamp")) {
                throw SerializationError("Invalid save file format");
            }
            
            // Version check
            if (save_data["version"] != "1.0.0") {
                Log::log_warning("Save version mismatch", __FILE__, __LINE__);
            }
            
            // Deserialize MapManager state using the serializer
            MapManagerSerializer::deserialize_map_manager(MapManager::get_instance(), save_data["map_manager"]);
            
            return true;
        } catch (const std::exception& e) {
            Log::log_warning("Load failed: " + std::string(e.what()), __FILE__, __LINE__);
            return false;
        }
    }

    bool has_save() {
        std::ifstream file("saves/save.json");
        return file.good();
    }

private:
    SaveLoadSystem() = default;
    SaveLoadSystem(const SaveLoadSystem&) = delete;
    void operator=(const SaveLoadSystem&) = delete;
};
