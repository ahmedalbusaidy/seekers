# Seekers

![Seekers Splash Screen](doc/Splashscreen.jpg)

## Team 23: Los Pollos Hermanos Gaming

Seekers is a third person 3D action RPG that melds dungeon crawling with open-world exploration. Set in the dark fantasy world of Eldoria, players take on the role of a Seeker, striving to uncover ancient secrets and restore balance to a land shrouded in darkness.

### Key Features

- **Dynamic Combat System**: 
  - Skill-based real-time combat with enemy lock-on
  - Attack buildup and combo system
  - Dodge-roll mechanics with i-frames
  - Estus flask healing system with cooldown
  - Damage scaling based on stats

- **Progression System**:
  - Level-up orbs with randomized stat improvements
  - Multiple character stats (Health, Energy, Defense, Power, etc.)
  - Upgradeable healing capacity
  - Persistent character progression

- **World System**:
  - Vast open world hub area
  - Three unique dungeon themes (Jungle, Castle, Crystal)
  - Procedurally generated dungeons
  - Dynamic lighting system
  - Environmental variety with interactive objects

- **Boss System**:
  - Unique themed bosses for each dungeon type
  - Complex AI behavior patterns
  - Multiple attack patterns and phases
  - Distinctive visual designs

- **Technical Features**:
  - Robust save/load system with auto-save
  - Comprehensive menu system
  - Cross-platform compatibility
  - Memory-efficient design
  - Stable frame rate performance

### Game Controls

#### Basic Movement
- **WASD**: Movement
- **Mouse**: Camera control
- **Space**: Dodge roll
- **H**: Toggle stats display

#### Combat
- **Left Mouse**: Attack
- **Right Mouse**: Lock-on target
- **1**: Use healing flask (Estus)

#### Interaction
- **F**: Interact with objects/entrances (Bonfires, Doors, etc.)
- **ESC**: Pause menu

#### System
- **F5**: Quick save
- **F6**: Quick load

### Project Structure

```
src/
├── app/                 # Core application logic
├── components/         # Game components (Combat, AI, Physics)
├── ecs/               # Entity Component System
├── globals/           # Global variables and constants
├── renderer/          # Graphics and rendering
├── systems/           # Game systems (AI, Combat, Physics)
├── utils/            # Utility functions
└── main.cpp          # Entry point
```

## Milestone 4 Implementation Details

### Mandatory Requirements (70%)

#### Playability (15%) ✓
- 10+ minutes of non-repetitive gameplay
- Three unique dungeon themes
- Multiple boss encounters
- Progressive difficulty scaling

#### Stability (15%) ✓
- Cross-platform compatibility
- Consistent performance
- Memory optimization
- Robust error handling

#### User Experience (10%) ✓
- Self-explanatory tutorial system
- Intuitive UI/HUD elements
- Responsive controls
- Clear visual feedback

#### Robustness (15%) ✓
- Efficient memory management
- Stable frame rate
- Graceful error handling
- Robust input system

#### Reporting (15%) ✓
- Comprehensive bug tracking
- Detailed test plan
- Video demonstration
- User feedback implementation

### Creative Components (30%)

#### Reloadability (10%) ✓
- Complete game state serialization
- JSON-based save file system
- Multiple save slots support
- Automatic saving at rest points
- Full entity/component preservation
- Test path: `src/systems/SaveLoadSystem.hpp`

#### Advanced Decision Making (10%) ✓
- Complex boss AI with multiple attack patterns
- Goal-based enemy behavior
- Dynamic difficulty scaling
- Advanced pathfinding for enemies
- Coordinated enemy positioning
- Test path: `src/systems/AISystem.hpp`

#### Audio System (10%) ✓
- Context-sensitive combat sounds
- Environmental audio feedback
- Distance-based audio scaling
- Theme-specific background music
- Interactive sound effects
- Test path: `src/systems/AudioSystem.hpp`

### Implementation Details

1. **Save/Load System**
   - Human-readable JSON save files
   - Complete entity serialization
   - Component state preservation
   - Registry management
   - Checkpoint system at bonfires

2. **AI Decision Making**
   - Boss combat patterns
   - Enemy coordination
   - Advanced pathfinding
   - Dynamic difficulty adjustment
   - Contextual behavior selection

3. **Audio Implementation**
   - Combat feedback system
   - Environmental ambience
   - Musical theme transitions
   - Distance-based audio
   - Interactive sound effects

### Additional Features

#### Combat Enhancements
- Attack buildup system
- Damage scaling
- Dodge cancellation
- Estus cooldown system

#### World Design
- Themed dungeons
- Open world hub area
- Environmental objects
- Dynamic lighting

#### Technical Improvements
- Enhanced save system
- Menu system
- Performance optimization
- Memory management

## Acknowledgments

- OpenGL and GLFW communities
- SDL2 developers
- GLM developers
- Our dedicated team members and supportive instructors

---

<p align="center">
    <strong>Embark on an Epic Journey Through the Twisted Lands of Eldoria!</strong>
</p>