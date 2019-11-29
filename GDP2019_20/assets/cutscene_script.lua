-- Rotate and move spaceship
BuildMoveCommand("move_player_1", "player", 
    0, 20, 0, 
    0, 80, 0, 
    6.0, 1.0, 1.0)
BuildRotateCommand("rotate_player_1", "player", 0, 170, 0, 2.0, 1, 0)
BuildRotateCommand("rotate_player_2", "player", 0, 170, 0, 2.0, 0, 0)
BuildLookToward("rotate_player_3", "player", 0, 1, 0, 2.0, 0, 1)
BuildSerialCommand("rotation");
addToSerial("rotation", "rotate_player_1");
addToSerial("rotation", "rotate_player_2");
addToSerial("rotation", "rotate_player_3");

BuildFollowCommand("follow", "ship1", "player", 5, 0, 0, 10, 30, 10)

BuildRideCurve(
    "curve", 
    "ship2", 
    -20, 20, 0, 
    -20, 80, 0, 
    -70, 50, 20, 
    6, 
    2.0, 
    1.0);

-- Trigger for the curve airship
BuildRotateCommand("rotate_airship", 
    "ship2",
    -100, 0, 0,
    1.0, 0.5, 0.5)
BuildTriggerCommand("trigger", 
    "ship2", "rotate_airship",
    -320, 30, -100,
    200, 40, 100,
    10)

-- Camera movement and rotation
BuildMoveCommand("move_camera", "theCamera",
    0, 100, -150,
    0, 100, 0,
    6, 2, 1)
BuildRotateCommand("rotate_camera", "theCamera",
    0, -90, -0,
    6, 2, 1)

-- combining the commands for the full script
BuildParallelCommand("step_1");
addToParallel("step_1", "move_player_1");
addToParallel("step_1", "rotation");
addToParallel("step_1", "follow");
addToParallel("step_1", "curve");
addToParallel("step_1", "trigger");
addToParallel("step_1", "move_camera");
addToParallel("step_1", "rotate_camera");

chooseFinalScript("step_1")