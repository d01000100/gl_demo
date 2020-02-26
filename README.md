# Physics 2 Project 3

José Daniel Casado Aguilar - 0940167

Scene simulating rigid bodies (spheres and planes) moving, accelerating, and colliding to each other with Bullet Physics.

The Bullet Physics library and wrapper is defined in the *BulletPhysicsWrapper* Project.

In it, there are two main sections:

**physics/bullet**

The Bullet library itself plus a *nConvert* header with useful function to transform between Bullet math objects and glm objects.

**wrapper/bullet**

Bullet wrapper that implements *PhysicsInterfaces* and delegates the work to the Bullet Physics.

## Changes to engine (GDP2019_20)

Theres only a handful of lines of code added to the game engine project in order to use Bullet.

In the *GDP2019_20/PhysicsConfig.h* header, I added a conditional macro to include and use the Bullet Library and I changed the `define` in order to actually use Bullet.

In order to change back to *MyPhysicsLibrary* it's only necessary to define `MY_PHYSICS` instead of `BULLET_PHYSICS` (line 3,4).

## Build instructions

Open `j_casadoaguilar_engine.sln` with Visual Studio. Select `Release` and `x64` and then `Build Solution` (Ctrl + Shift + B)

## Run instructions

Within Visual Studio, click on `Local Windows Debugger` (F5)

### Use instructions

When the programs starts the scene will show 10 spheres of different sizes falling to a horizontal square.

Press the `W` or `S` keys to pitch the camera. The `A` or `D` to yaw. `Q` and `E` to zoom in or out to the selected sphere.

`Space` changes the camera and the control to the next sphere. It rollsback when it gets to the last sphere.

The `I`, `K`, `J`, and `L` keys apply forces to the selected sphere forward, backwards, to the left or to the right respectively.

`Esc` closes the program.

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
      // Name of the mesh the object uses. 
      // Must match exactly to the ones defined in "Meshes".
      "mesh": "pretty_mesh",
      // Scale in which the mesh will be rendered on screen
      "scale": 1.0,
      // RGB
      "diffuseColor": [ 1.0, 0.0, 1.0 ],
      // RGB and intensity
      "specularColor": [ 1.0, 1.0, 0.0, 50.0 ],
      // If the object should have an "iPhysicsComponent"
      "Physics": {
        // Physics shape. One of "sphere" or "plane:
        // Every physics configuration must have one
        "shape": "sphere", // or mesh
        // Initial position of the object in xyz coordinates
        "position": [10, 100, -20],
        // Mass to consider when applying forces to the body.
        // It it's 0, it doesn't move.
        "mass": 1.0,
        // (For spheres) radius of the sphere
        // Unrelated to the graphic scale
        "radius": 4.0,
        // (For planes) direction in 3D space that the plane is facing.
        "normal": [0,1,0]
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
