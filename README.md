# Media Project 2

José Daniel Casado Aguilar - 0940167

FMOD 3D audio showcase displaying a fight between zombies and knights protecting a forest.

## Build instructions

Open `j_casadoaguilar_Media_P2.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

When the programs starts the scene will load some knights fighting zombies with a forest to the right.

Three different sound groups will be played:
- The knights figthing with epic battle music.
- The zombies walking with cave sounds.
- Forest sounds.

The user can walk to the right and left with the A and D keys. They can also walk forward and backwards with the W and S keys.

Using the up and down arrows, the user can select different sounds.
The selected sound info will be displayed in the title of the window. Also three perpendicular lines will show on the location of the sound.

To control the sound, the user can hold the **SHIFT** key and press:
- W,A,S,D,Q,E to move the sound across the scene.
- Up and down to change the volume.
- Left and right to change the pitch.
- Space to toggle the pause.

9 different DSPs can the toggled for the selected sound by pressing the keys from 1 to 9.

## Scene file

The file `GDP2019_20/assets/scene1.json` defines the scene that the program will load in a JSON file.

The format of the file is the following:

```json
{
    "Sounds": 
    [
        // this will create a channel group per JSON object
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
}
```