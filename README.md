# Artificial Intelligence 

José Daniel Casado Aguilar - 0940167

Simple 2D shooter game with AI with different behaviours.

## Behaviours

**Seek and Flee**: This enemies *seek* the player when it faces away from them and *flee* from the player when it faces towards them. They turn *red* and *blue* respectively.
**Pursue and Evade**: The enemy will *pursue* the player and *evade* its bullets. When it's pursuing it turns *white*, when it's evading it turns *purple*.
**Wander and Idle**: The enemy will change between two modes: random *wandering* and not moving. When it's wandering it turns *grey* and when it's idle it turns *black*.
**Approach and Shoot**: The enemy will approach the enemy (with a slowdown when it's at a certain distance) and turn *green*. When it's close enough it will stop, fire a bullet toward the player and turn yellow.

There are videos showing each of the behaivours individually and the complete game.

## Build instructions

Open `j_casadoaguilar_AI_P1.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

The player will start at the center of the screen.
An enemy will spawn randomly every 6 seconds.
To steer the player, press the A and D keys. To move it, press the W or D keys.

## Scene file

The file `GDP2019_20/assets/scene1.json` defines the scene that the program will load in a JSON file.

The format of the file is the following:

```json
{
    "Sounds": 
    [
        // this will create a channel group per object
        {
            "name": "zombie sounds",
            // initial position of the sounds
            "position": [20.0, 0.0, 15.0],
            // the audios will loop automatically
            "audios": [
                // filepath of each of the sounds
                // all of them will be streamed
                "assets/sounds/zombies.mp4",
                "assets/sounds/gravel.mp4",
                "assets/sounds/cave.mp4"
            ]
        },
        {
            "name": "warriors",
            "position": [-20.0, 0.0, 15.0],
            "audios": [
                "assets/sounds/sword.mp4",
                "assets/sounds/magic.mp4",
                "assets/sounds/arrow.mp4"
            ]
        }
    ],
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
      // If the object should obey to physics simulation
      "Physics": {
        // if the object should fall due to gravity in the Y direction
        "gravity": true,
        // Initial velocity
        "velocity": [ 5.0, 5.0, 0.0 ], 
        // Initial accelerration
        "acceleration": [0.0, 1.0, 0.0],
        // Shape of the collider 
        "collider": "sphere", // or mesh
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