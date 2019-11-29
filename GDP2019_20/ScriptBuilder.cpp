#include "ScriptBuilder.h"
#include <glm/vec3.hpp>

#include "MoveCommand.h"
#include "SerialCommand.h"
#include "ParallelCommand.h"
#include "RotateCommand.h"
#include "LookToward.h"
#include "LookAt.h"
#include "FollowCommand.h"
#include "RideCurve.h"
#include "TriggerCommand.h"
#include "Scene.h"
#include "DollyCamera.h"

using namespace std;
using namespace glm;

std::map<std::string, iCommand*> ScriptBuilder::loadedCommands;
iCommand* ScriptBuilder::finalScript = NULL;

iCommand* ScriptBuilder::TestScript()
{
	Scene* theScene = Scene::getTheScene();
	aGameItem* ship1 = theScene->findItem("ship1");
	aGameItem* ship2 = theScene->findItem("ship2");
	aGameItem* player = theScene->findItem("player");
	aGameItem* camera = DollyCamera::getTheCamera();
	SerialCommand* script = new SerialCommand();
	ParallelCommand* allUp = new ParallelCommand();
	camera->setPos(glm::vec3(0, 100, -150));
	camera->setDirection(-glm::vec3(0, 100, -150));
	script->pushCommand(new MoveCommand(camera, glm::vec3(0, 100, -150), glm::vec3(0, 90, -140), 3.0f, 1, 1));
	//script->pushCommand(new RotateCommand(camera, glm::vec3(0,0,10), 1.0f));
	//script->pushCommand(new BuildMoveCommand(ship, glm::vec3(0, 34, -20), glm::vec3(20, 30, 20), 5.0f, 1.0f, 1.5f));
	//script->pushCommand(new BuildMoveCommand(player, glm::vec3(10, 10, -10), glm::vec3(0, 20, 0), 2.0f, 0.0f, 1.0f));
	allUp->addCommand(new RideCurve(ship1, glm::vec3(-50, 30, 0), glm::vec3(50, 30, 0), glm::vec3(0, 120, 0), 12, 4, 4));
	allUp->addCommand(new RideCurve(ship2, glm::vec3(-50, 30, 30), glm::vec3(50, 30, 30), glm::vec3(0, 120, 0), 12, 0, 0));
	SerialCommand* cameraPan = new SerialCommand();
	cameraPan->pushCommand(new RotateCommand(camera, glm::vec3(-20, 0, 0), 6, 4, 2));
	cameraPan->pushCommand(new RotateCommand(camera, glm::vec3(20, 0, 0), 6, 2, 4));
	allUp->addCommand(cameraPan);
	//allUp->addCommand(new FollowCommand(player, ship, glm::vec3(0,10,0), 20, 30, 60, 0));
	//allUp->addCommand(new TriggerCommand(ship1, glm::vec3(-10), glm::vec3(10), new RotateCommand(ship2, glm::vec3(0, 170, 0), 7.0), 2));
	//allUp->addCommand(new BuildMoveCommand(player, glm::vec3(0, 20, 0), glm::vec3(0, 40, 0), 2.0f, 0.0f, 1.0f));
	//allUp->addCommand(new BuildMoveCommand(ship, glm::vec3(20, 30, 20), glm::vec3(20, 50, 20), 2.0f, 0.5f, 1.0f));
	script->pushCommand(allUp);
	return script;
}

void ScriptBuilder::printLoadedCommands()
{
	printf("\nLoaded commands:\n");
	for (std::map<std::string, iCommand*>::iterator cIt = loadedCommands.begin();
		cIt != loadedCommands.end(); cIt++)
	{
		printf("%s: %s\n",
			cIt->first.c_str(),
			cIt->second ? "Loaded" : "Not loaded");
	}
	printf("\n");
}

/*
	1. Name of Command that will be the script to run in the main
*/
int ScriptBuilder::chooseFinalScript(lua_State* L)
{
	// get the name of the command in the map
	std::string name = lua_tostring(L, 1);
	printLoadedCommands();
	printf("Script: %s\n", name.c_str());
	finalScript = loadedCommands[name];
	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3, 4, 5. StartPos
	6, 7, 8. finalPos
	9. totalTime
	10. easeInDuration
	11. easeOutDuration;
*/
int ScriptBuilder::BuildMoveCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	
	iCommand* moveCommand = new MoveCommand(
		// item to be affected by command
		gameItem,
		// Start position
		vec3(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5)),
		// End position
		vec3(lua_tonumber(L, 6), lua_tonumber(L, 7), lua_tonumber(L, 8)),
		// total time
		lua_tonumber(L, 9),
		// easeInDuration
		lua_tonumber(L, 10),
		// easeOutDuration
		lua_tonumber(L, 11)
	);

	moveCommand->name = name;

	ScriptBuilder::loadedCommands[name] = moveCommand;

	printf("Created MoveCommand : %s\n",
		moveCommand->name.c_str());

	return 0;
}

aGameItem* ScriptBuilder::findItem(std::string name)
{
	if (name == "theCamera") {
		return DollyCamera::getTheCamera();
	}

	return Scene::getTheScene()->findItem(name);
}

iCommand* ScriptBuilder::getFinalScript()
{
	return ScriptBuilder::finalScript;
}

/*
	1. Name of the new serial command
*/
int ScriptBuilder::BuildSerialCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	iCommand* serial = new SerialCommand();
	serial->name = name;

	loadedCommands[name] = serial;
	return 0;
}

/*
	1. Name of the existing serial command.
	2. Name of the existing command to add at the end of the serial command.
*/
int ScriptBuilder::addToSerial(lua_State* L)
{
	SerialCommand* serial = (SerialCommand*)loadedCommands[lua_tostring(L, 1)];
	serial->pushCommand(loadedCommands[lua_tostring(L, 2)]);
	return 0;
}

/*
	1. Name of the new parallel command
*/
int ScriptBuilder::BuildParallelCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	iCommand* parallel = new ParallelCommand();
	parallel->name = name;

	loadedCommands[name] = parallel;
	return 0;
}

/*
	1. Name of the existing parallel command.
	2. Name of the existing command to add to the parallel command.
*/
int ScriptBuilder::addToParallel(lua_State* L)
{
	ParallelCommand* parallel = (ParallelCommand*)loadedCommands[lua_tostring(L, 1)];
	parallel->addCommand(loadedCommands[lua_tostring(L, 2)]);
	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3, 4, 5. EulerAngles
	6. totalTime
	7. easeInDuration
	8. easeOutDuration;
*/
int ScriptBuilder::BuildRotateCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	
	iCommand* rotateCommand = new RotateCommand(
		// item to be affected by command
		gameItem,
		// euler angles to rotate
		vec3(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5)),
		// total time
		lua_tonumber(L, 6),
		// easeInDuration
		lua_tonumber(L, 7),
		// easeOutDuration
		lua_tonumber(L, 8)
	);

	rotateCommand->name = name;

	ScriptBuilder::loadedCommands[name] = rotateCommand;

	printf("Created RotationCommand : %s\n",
		rotateCommand->name.c_str());

	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3, 4, 5. direction to look toward
	6. totalTime
*/
int ScriptBuilder::BuildLookToward(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	
	iCommand* lookToward = new LookToward(
		// item to be affected by command
		gameItem,
		// direction to look forward
		vec3(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5)),
		// total time
		lua_tonumber(L, 6)
	);

	lookToward->name = name;

	ScriptBuilder::loadedCommands[name] = lookToward;

	printf("Created LookTowardCommand : %s\n",
		lookToward->name.c_str());

	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3. Name of the object in scene to follow
	4, 5, 6. offset from the target to follow to
	7. duration
	8. maxVel
	9. distance from target to begin to slow down 
	10. distance from target to stop
*/
int ScriptBuilder::BuildFollowCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	aGameItem* targetItem = findItem(lua_tostring(L, 3));
	
	iCommand* followCommand = new FollowCommand(
		// item to be affected by command
		gameItem,
		// item to follow in scene
		targetItem, 
		// offset from target
		vec3(lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6)),
		lua_tonumber(L, 7),
		// max velocity
		lua_tonumber(L, 8),
		// distance from target to begin to slow down
		lua_tonumber(L, 9),
		// distance from target to stop
		lua_tonumber(L, 10)
	);

	followCommand->name = name;

	ScriptBuilder::loadedCommands[name] = followCommand;

	printf("Created FollowCommand : %s\n",
		followCommand->name.c_str());

	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3, 4, 5. StartPos
	6, 7, 8. finalPos
	9, 10, 11. beizer middle point
	12. totalTime
	13. easeInDuration
	14. easeOutDuration;
*/
int ScriptBuilder::BuildRideCurve(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	
	iCommand* curveCommand = new RideCurve(
		// item to be affected by command
		gameItem,
		// Start position
		vec3(lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5)),
		// End position
		vec3(lua_tonumber(L, 6), lua_tonumber(L, 7), lua_tonumber(L, 8)),
		// Beizer middle point
		vec3(lua_tonumber(L, 9), lua_tonumber(L, 10), lua_tonumber(L, 11)),
		// total time
		lua_tonumber(L, 12),
		// easeInDuration
		lua_tonumber(L, 13),
		// easeOutDuration
		lua_tonumber(L, 14)
	);

	curveCommand->name = name;

	ScriptBuilder::loadedCommands[name] = curveCommand;

	printf("Created CurveCommand : %s\n",
		curveCommand->name.c_str());

	return 0;
}

/*
	1. Name of the new Command
	2. Name of object to detect
	3. Name of existing command to execute with trigger
	4, 5, 6. minimun corner of AABB
	7, 8, 9. maximum corner of AABB
	10. life time of trigger
*/
int ScriptBuilder::BuildTriggerCommand(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	iCommand* toTrigger = ScriptBuilder::loadedCommands[lua_tostring(L, 3)];

	iCommand* triggerCommand = new TriggerCommand(
		// item to be detected to enter the AABB
		gameItem,
		// minimun corner of AABB
		vec3(lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6)),
		// Maximum corner of AABB
		vec3(lua_tonumber(L, 7), lua_tonumber(L, 8), lua_tonumber(L, 9)),
		// command to execute when triggered
		toTrigger,
		// life time of trigger
		lua_tonumber(L, 10)
	);

	triggerCommand->name = name;

	ScriptBuilder::loadedCommands[name] = triggerCommand;

	printf("Created TriggerCommand : %s\n",
		triggerCommand->name.c_str());

	return 0;
}

/*
	1. Name of the new Command
	2. Name of the object in scene to move
	3. Name of the object in scene to look at
	4. duration
*/
int ScriptBuilder::BuildLookAt(lua_State* L)
{
	string name = lua_tostring(L, 1);
	aGameItem* gameItem = findItem(lua_tostring(L, 2));
	aGameItem* targetItem = findItem(lua_tostring(L, 3));

	iCommand* lookAtCommand = new LookAtCommand(
		// item to be affected by command
		gameItem,
		// item to follow in scene
		targetItem,
		// duration
		lua_tonumber(L, 4)
	);

	lookAtCommand->name = name;

	ScriptBuilder::loadedCommands[name] = lookAtCommand;

	printf("Created lookAtCommand : %s\n",
		lookAtCommand->name.c_str());

	return 0;
}
