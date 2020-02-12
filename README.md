# Physics 2 Project 2

José Daniel Casado Aguilar - 0940167

Scene simulating rigid bodies (spheres and planes) moving, accelerating, and colliding to each other.

The solution is divided in several projects. Each one either groups functionality or defines an interface for other projects to use:

**MathIncludes**

Offers a handy header file with all the includes of math libraries (glm) that the other projects use.

**PhysicsInterfaces**

Defines interfaces for physics related entities: physics components, like ball or plane, physic world and a factory that creates these entitites. A client only need to know about the interfaces classes and functions in order to use physics.

**MyPhysicsLibrary**

Implements a simulation of *rigid bodies* and shapes that accelerate, move and collide in 3D space.  

- The **phys/cIntegrator** class implements a fourth order Runge-Kutta integration to simulate moving objects in 3D space.
- The **phys/nCollide** class implements the 3D shape collision detection algorithms from the Ericson's book.
- The **phys/cWorld** class has a collection of rigid bodies in which it applies the integration in *cIntegration*. It also checks for collisions with the detectors in *nCollide* and reacts accordingly if it finds one.

**MyPhysicsWrapper**

Implements *PhysicsInterfaces* delegating the real work to *MyPhysicsLibrary*. That way clients can use *MyPhysicsLibrary* only knowing about *PhysicsInterfaces*

**GDP2019_20**

Game engine that draws 3D models in 3D space, simulates physics on them, through *MyPhysicsWrapper*, and listens to keyboard presses to interact with the scene.

## Important files in GDP2019_20

All of the files in the projects *MathIncludes*, *PhysicsInterfaces*, *MyPhysicsLibrary* and *MyPhysicsWrapper* are involved in the requirements of the project. All of ther classes and functions are documented explaining what they do and how to use them.

That does not happen in *GDP2019_20*. As it's a project that was built during a lot of courses, trying to meet all requirements and still a work in progress, it's a little messier than the rest. I will highlight the important files and classes that are relevant for the requirement of the project:

**PhysicsConfigs**

Necessary includes and declaration of global variables related to the physics library.

It has an "easy to change" macro that chooses what concrete physics library to use. Right now there's only *MyPhysicsWrapper*.

It initializes an *iPhysicsFactory* and an *iPhysicsWorld* in global scope for the rest of the engine to use.

**JSON_IO**

JSON parser that reads the configuration file and, using `nlohmann/json`, builds data structures in a format that the engine understands.

In particular, the function *readItems*, when it encounters and object with physics, it adds it to the global *iPhysicsWorld*.

**GameItemFactory\iGameItemFactory**

Abstract Factory that generates the items in the 3D scene: lights, objects, cameras, etc. All of its methods recieve a `nlohmann/json` object to get the necessary information to create the respective item.

In particular, the method *createGameItem* when it's asked to build an object with physics, it calls the builder method *buildPhysicsObject* to create the proper *iPhysicsComponent* (through the global *iPhysicsFactory*) with the data in the json object and add it to the game object.

**cGameObject**

This class describes a 3D visible object in the scene. If its property *mPhysicsComponent* is initialized, it also represents a physics body in the scene.

If it has one, it uses the position described by *mPhysicsComponent* to draw itself in the scene.

**FollowCamera**

A 3D camera that follows and looks at a game object with physics in the scene.

It can yaw and pitch around the object and zooms closer and farther from it.

When it is initialized, it filters all the objects of the Scene (via a static Singleton) and fills a vector of the physic spheres. This way, it can iterate through them and change its target to a different sphere.

**CameraPusher**

Class that, using the relative position of the *FollowCamera* with its target, offers methods to apply forces to the target in orthogonal directions to the camera direction.

**UserInput**

Class that listens for the keyboard inputs and calls other functions in order to alter the scene.

In particular the *camera_orbit* function tells the *FollowCamera* (through a static Singleton) to pitch, yaw or zoom around its target. It also tells the camera to switch to the next sphere.

The *physicsControls* calls the *CameraPusher* to apply forces to the "selected" sphere in the scene.

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
