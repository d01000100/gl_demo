# Physics Project 1

José Daniel Casado Aguilar - 0940167

Graphic scene and Physics simulator that shows a bunch of spheres bouncing around inside a bowl with a "blender" on the bottom to stir them up.

## Build instructions

Open `j_casadoaguiarl_Project1_openGL.sln` with Visual Studio 2019. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

When the programs starts, the simulation will show a lot of spheres bouncing around. The user can:

- Press the `pause` button and pause or play the scene.
- Press `F5` to reload the scene file and reset the scene.

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