-- This script tells the program what commands to build, what objects in the scene
-- will affect and how to group them and sort them together.

-- Each Build* command creates a new command and sets a name to it. If a name gets
-- repeated, the command gets overwritten.

-- The commands refer to game items (objects, lights) via the name specified in the
-- scene file (scene1.json). 

-- Rotate and move spaceship
BuildMoveCommand(
    "move_player_1", -- name of the new Move Command
    "player",  -- item to be moved
    0, 20, 0, -- starting position (x,y,z)
    0, 80, 0, -- end position (x,y,z)
    6.0, -- total duration of the movement
    1.0, -- duration to take easing into the movement
    1.0 -- duration to take easeing out of the movement
);
BuildRotateCommand(
    "rotate_player_1", -- name of the new Rotate Command
    "player", -- name of the item to be rotated
    0, 170, 0, -- rotation to apply to the object in Euler angles (degrees)
    2.0, -- total duration of the rotation
    1, -- duration to ease into the rotation
    0 -- duration to eae out of the rotation
)
BuildRotateCommand("rotate_player_2", "player", 0, 170, 0, 2.0, 0, 0)

-- Turn an item to look at a specific direction 
BuildLookToward(
    "rotate_player_3", -- name of the command
    "player", -- item to be turned
    0, 1, 0, -- direction to turn to (up)
    2.0, -- time to take to turn
    0, -- time to ease in
    1 -- time to ease out
)

-- "BuildSerialCommand" creates an empty serial command group
BuildSerialCommand("rotation");
-- "addToSerial" add commands to the serial group. The commands are run in
-- the order that they're added. All of the commands (serial and members)
-- must have been built earlier in the script.
addToSerial("rotation", "rotate_player_1");
addToSerial("rotation", "rotate_player_2");
addToSerial("rotation", "rotate_player_3");

BuildFollowCommand(
    "follow", -- name of the new follow command
    "ship1", -- follower item
    "player", -- target item 
    5, 0, 0, -- offset that the follower will follow the target
    10, -- total duration of the command
    30, -- distance from the target offset that the follower will start to slow down
    10 -- distance from the target offset that the follower will stop
)

-- The curve is built by a three points beizer curve:
-- start, end, and a middle point that controls the curvature.
BuildRideCurve(
    "curve", -- name of the new curve command
    "ship2", -- item that will follow the curve
    -20, 20, 0, -- start position xyz
    -20, 80, 0, -- end position xyz
    -70, 50, 20, -- middle point that controls the curvature
    6, -- total duration of the command
    2.0, -- duration to ease into the movement
    1 -- duration to eae out of the movement
);
    
-- Trigger for the curve airship
BuildRotateCommand("rotate_airship", 
    "ship2",
    -100, 0, 0,
    1.0, 0.5, 0.5)

-- The trigger command runs another command when an items enters an AABB
-- described by two points: minimum corner and maximum corner
BuildTriggerCommand(
    "trigger", -- Name of the new trigger command
    "ship2", -- Item to monitor
    "rotate_airship", -- Command to execute if triggered
    -320, 30, -100, -- minimum corner of the AABB
    200, 40, 100, -- maximum corner of the AABB
    10 -- lifetime of the trigger.
)
    
-- "theCamera" is a reserved name to affect the camera with a command. An item with
-- this name will not be affected by the command.
-- Camera movement and rotation
BuildMoveCommand("move_camera", "theCamera",
    0, 100, -150,
    150, 100, 0,
    6, 2, 1)

-- An item will turn towards another item
-- in this case "theCamera" towards "player"
BuildLookAt("rotate_camera", "theCamera", "player", 9)

-- combining the commands for the full script
-- Same as with serial command, a parallel command group is created empty
BuildParallelCommand("step_1");
-- and then other commands are added to it.
addToParallel("step_1", "move_player_1");
addToParallel("step_1", "rotation"); -- including other command groups
addToParallel("step_1", "follow");
addToParallel("step_1", "curve");
addToParallel("step_1", "trigger");
addToParallel("step_1", "move_camera");
addToParallel("step_1", "rotate_camera");

-- This command chooses which of the commands will be run by the engine.
-- Generally is a command group that contains all others commands.
chooseFinalScript("step_1")