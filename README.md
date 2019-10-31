# Graphics Midterm

José Daniel Casado Aguilar - 0940167

Scene showing a pirate island during daytime or nightime.

The `Answer_1.ply` model answers the **Question 1** about creating the island mesh.

The `Answer_2.png` is a picture of the scene described in **Question 2.**
You can also rename the `GDP2019_20\assets\daylight_scene.json` to `scene1.json` to load the daylight scene.

## Build instructions

Open `j_casadoaguilar_Graphics_midterm.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

When the programs starts the scene will show the pirate island at night.
To change the scene, rename the `GDP2019_20\assets\daylight_scene.json` to `scene1.json`.
Same thing with the `night_scene.json` for coming back to the night scene.

In the daylight scene, you can rotate the camera with the A and D buttons.

Additionally in the night scene, you can change between the different cameras in **Question 4** for the buttons 1,2 or 3.

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