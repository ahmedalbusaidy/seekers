# Seekers

A third-person 3D action RPG blending Soulslike combat with procedurally generated dungeon crawling. Built from scratch in C++ with a custom OpenGL renderer and Entity Component System.

## Demo

[![Watch the Demo](https://img.youtube.com/vi/hXh101_gjjw/maxresdefault.jpg)](https://www.youtube.com/watch?v=hXh101_gjjw)

*Click to watch on YouTube*

## About

Set in the dark fantasy world of Eldoria, players take on the role of a Seeker uncovering ancient secrets across three themed dungeons (Jungle, Castle, Crystal), each with its own boss. Originally built as a team project at UBC; this fork preserves the full commit history.

**Tech stack:** C++, OpenGL, SDL2, GLM, custom ECS architecture, JSON-based serialization.

## Features

**Combat**
- Real-time skill-based combat with enemy lock-on, dodge-roll i-frames, and attack combos
- Estus flask healing with cooldown, modeled on Soulslike resource pressure
- Stat-driven damage scaling

**World and Progression**
- Open world hub plus three procedurally generated dungeons
- Level-up orb system with randomized stat improvements (Health, Energy, Defense, Power)
- Bonfire checkpoints with auto-save

**AI**
- Boss encounters with multi-phase attack patterns and distinct behaviors per theme
- Goal-based enemy decision making with coordinated positioning and pathfinding

**Systems**
- Full game state serialization to human-readable JSON; multiple save slots
- Context-sensitive audio: distance-attenuated SFX, theme-specific music transitions
- Dynamic lighting and environmental interaction

## My Contributions

I primarily owned two systems on this project:

**Collision Detection System** (`src/systems/CollisionSystem.hpp`, `src/components/PhysicsComponents.hpp`)

Built the full collision pipeline from broad phase to narrow phase resolution.

- Spatial grid for broad-phase culling, reducing per-frame collision checks from O(n²) to roughly O(n) for typical entity densities.
- Four collider primitives supported: Circle, AABB, Wall (rotated AABB with line-segment edges), and Mesh (arbitrary convex polygon).
- Implemented all narrow-phase pair resolutions: circle-circle, circle-wall (with per-edge normal/penetration), and circle-mesh (used for projectile geometry).
- The mesh collider pipeline was the trickiest piece: at projectile spawn, I transform a model's triangle mesh through its model matrix into 2D world space, deduplicate vertices, sort them counter-clockwise around the centroid to form a convex hull, and compute a bounding radius for broad-phase rejection. This lets arrows collide with enemies using their actual rendered shape instead of a loose circle, which mattered for hit feel.
- Collision response includes velocity projection along contact normals, tangent friction for walls, and equal-mass impulse exchange between locomotive entities.

**Save/Load System** (`src/systems/SaveLoadSystem.hpp`, `src/utils/RegistrySerializer.hpp`, `src/utils/ComponentSerializer.hpp`, `src/utils/MapManagerSerializer.hpp`)

Designed and implemented end-to-end persistence for the entire game state.

- Serializes 30+ ECS component types to human-readable JSON (motions, locomotion stats, weapons, AI state including boss Q-learning tables, attack buildups, level-up orbs, collision bounds, light sources, interactables, and more).
- Two-pass deserialization to correctly remap entity IDs across the save boundary. First pass creates new entities and deserializes components without entity references; second pass resolves cross-references (e.g., `Attacker.weapon`, `Interactable.entity`, inventory contents, locked targets) using an old-ID-to-new-Entity map. Without this, any component holding an `Entity` would silently point at the wrong object after load.
- Persists the full `MapManager` state, including all four registries (open world, dungeon, spire, last checkpoint) and a marker for which one is active, so loading restores the player to the exact map they saved in.
- Integrated auto-save at bonfire interactions and manual save/load via F5/F6.

## Controls

| Action | Key |
|--------|-----|
| Move | WASD |
| Camera | Mouse |
| Dodge roll | Space |
| Attack | Left Mouse |
| Lock-on | Right Mouse |
| Heal (Estus) | 1 |
| Interact | F |
| Toggle stats | H |
| Pause | ESC |
| Quick save / load | F5 / F6 |

## Project Structure

```
src/
├── app/             # Core application loop, World, MapManager, EntityFactory
├── components/      # ECS components (Combat, AI, Physics, Render, Gameplay)
├── ecs/             # Entity Component System core
├── renderer/        # OpenGL rendering pipeline
├── systems/         # Game systems (AI, Combat, Physics, Collision, SaveLoad, Audio)
├── shaders/         # GLSL shaders (Blinn-Phong, skybox, HUD)
├── utils/           # Serialization, transforms, pathfinding, file IO
├── globals/         # Constants and shared state
└── main.cpp         # Entry point
```

## Building

Built with CMake. Dependencies: OpenGL, GLFW, SDL2, SDL2_mixer, GLM, Assimp, FreeType, stb_image, nlohmann/json.

## Team

Built by Team 23 (Los Pollos Hermanos Gaming) for CPSC 427 at UBC.
