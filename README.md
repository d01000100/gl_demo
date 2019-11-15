# Graphics Project 2

José Daniel Casado Aguilar - 0940167

Scene showing a space battle

In the `pictures` folder there are some snaphsots showcasing the different grading points:

1. The different transparencies are shown in the `transparency-1` and `multiple-transparencies` picture.
2. The trivial texture combinations are in `fire-and-smoke` (with contrast on the `just-fire`) and in `colors-and-metal`
3. The `holes` picture shows the involved combination of textures: using one to make "holes" in the model via discard transparency.
4. The `fragmentShader` in  `GDP2019_20\assets\shaders` describes 6 different shaders.

## Build instructions

Open `j_casadoaguilar_Graphics_P2.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

- The WASD keys rotate the camera around the selected object.
- The EQ keys zoom in and zoom out.
- With the up and down arrows you can change the selected object: it's name and more info is shown in the title of the window.
- Pressing the 0 key toggles debug mode: the "wireframe" of all of the objects is shown alongside their normal color and textures.

## Scene file

The file `GDP2019_20/assets/scene1.json` defines the scene that the program will load in a JSON file.

The format of the file is the following:

```json
{
    // The meshes that the scene will need
 "Meshes": [
    // filepath relative to GDP2019_20 folder
    {
      "filename": "assets/models/low-poly-ship.ply",
      "name": "low-poly-ship"
    },
    {
      "filename": "assets/models/sphere.ply",
      "name": "sphere"
    }
  ],
  // Objects in the scene
  "Objects": [
    {
      // optional identifier to explicitly use in the program
      "name": "cool_object",
      // Name of the mesh the object uses. Must match exactly
      "mesh": "pretty_mesh",
      "isVisible": true,
      "isLit": true,
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
      },
      "textures": [
          {
              "textureName": "submarine.bmp",
              "weight": 1.0
          }
      ]
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