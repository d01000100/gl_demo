# Artificial Intelligence Project 2

José Daniel Casado Aguilar - 0940167

Showcase of different grouped steering behaviours.

## Build instructions

Open `j_casadoaguilar_AI_P2.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

At the start of the application, 12 planes will spawn on the screen and form themselves in a circle.

By pressing the buttons from 1-5, they can change formation: 1 - circle, 2 - V, 3 - Square, 4 - Line, 5 - two lines.

While following a formation, you can control the leader by pressing the `w` and `s` keys to go forward or backwards and `a` and `d` to steer the plane. The formation will follow and orient itself accordingly.

By pressing the `6` key, all of the vehicles will start flocking. While flocking you can adjust the weight of the three forces involved in flocking:

- Separation: `u` to increase, `j` to decrease.
- Cohesion: `i` to increase, `k` to decrease.
- Alignment: `o` to increase, `l` to decrease.

The values are displayed on the title of the application. They will remain normalized, increasing one will decrease the others and vice versa.

Pressing the `7` key will return the ships to their previous formation.

Pressing the `8` key will make the leader start following the path. The formation will follow him and orient themselves to the leader direction. The current followed node is highlighted blue.

The `9` key will inverse the path and the formation direction.

The `0` key will stop the path following and return to the previous formation.

All of the behaviours look way better with collisions off, so I made collisions toggable. By pressing the `c` key you can toggle the collisions on and off. The collisions start turned on.

## Solution outline

There's only one project called `GDP2019_20`. I'll explain the relevant classes:

There are videos showing each of the behaivours individually and the complete applications.

Most of the behaviours look better without collisions, so most of the videos have the collisions disabled.

There is one that showcases everything with collisions activated and the application runs by default with collisions activated.

### Formation

The file `Coordinator.h` and `Coordinator.cpp` manages a collection of game objects to seek toward a collection of offsets relative to a leader.

The 5 formations (circle, V, 1 lines, 2 lines, square) are defined explicitly as static collections of 3D vectors.

Every frame, for each member of the formation, the calculate the seek force toward the respective offset (by index in the vector) and apply the velocity to the member.

### Flocking

The `Flocking` header and cpp manages a collection of vehicles and applies forces to them to simulate the `cohesion`, `separation` and `alignment` behaviours and combines them with normalized weights (they all add up to 1).

Each frame, for each vehicle it calculates the neighbors around it, calculates each of the forces, combines them and applies them to the vehicle.

### Path Following

The `PathFollowing` header and cpp file has collection of points in the scene in which to cycle through.

It makes a game object seek toward the point until it's close enough and then goes to the next.

Its capable to cycle through the collection in both directions.

By affecting the leader of a formation, the whole formation follows the path.

### Manager

The `FlockingManager` class joins all of the behaviours together, listens to the user input to change them, and combines them when necessary.

## Scene file

The file `GDP2019_20/assets/scene1.json` defines the scene that the program will load in a JSON file.

The format of the file is the following:

```json
{
    // The meshes that the scene will need
  "Meshes": [
    // filepath relative to GDP2019_20 folder
    {"pretty_mesh": "filepath"},
    { "name2": "filepath2" }
  ],
  // Objects in the scene
  "Objects": [
    {
      // optional identifier to explicitly use in the program
      "name": "cool_object",
      // Name of the mesh the object uses. Must match exactly
      "mesh": "pretty_mesh",
      // Initial position
      "position": [ 1.0, 4.0, -5.0 ],
      // Initial Euler rotation in the three axis
      "rotation": [ 0.0, 90.0, 0.0 ],
      "scale": 1.0,
      // RGB
      "diffuseColor": [ 1.0, 0.0, 1.0 ],
      // RGB and intensity
      "specularColor": [ 1.0, 1.0, 0.0, 50.0 ],
      "tags": ["player", "path_point"]
      // If the object should obey to physics simulation
      "Physics": {
        // if the object should fall due to gravity in the Y direction
        "gravity": true,
        // Initial velocity
        "velocity": [ 5.0, 5.0, 0.0 ], 
        // Initial accelerration
        "acceleration": [0.0, 1.0, 0.0],
        // Shape of the collider 
        "shape": "sphere", // or mesh
        // radius of the sphere. Unrelated to scale (so far)
        "radius": 4.0
      }
    }
  ],
  // Lights in the scene
  "Lights": [
    {
      // unique name
      "name": "shiny_light",
      "position": [ 10.0, 5.0, 10.0 ],
      "type": "point", // spotlight, directional
      "linearAtten": 0.003,
      "quadAtten": 0.5,
      // for spotlights
      "direction": [ 1.0, 1.0, 1.0 ], // will be normalized
      "innerRad": 2.0,
      "outerRad": 3.0
    }
  ]
  // Diferent cameras positions to see the scene from
  "Cameras": [
      {
          "name": "all scene",
          "position": [
              -173.37274169921875,
              371.9513854980469,
              381.727783203125
          ],
          "target": [
              14.0,
              0.0,
              14.0
          ]
      }
  ]
}
```