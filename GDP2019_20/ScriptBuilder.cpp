#include "ScriptBuilder.h"
#include "MoveCommand.h"
#include "SerialCommand.h"
#include "Scene.h"

iCommand* TestScript()
{
	Scene* theScene = Scene::getTheScene();
	iGameItem* ship = theScene->findItem("aaaskyship");
	iGameItem* player = theScene->findItem("player");
	SerialCommand* script = new SerialCommand();
	script->pushCommand(new MoveCommand(ship, glm::vec3(20, 30, 20), 3.0f, 1.0f, 0.5f));
	script->pushCommand(new MoveCommand(player, glm::vec3(0, 20, 0), 2.0f, 0.0f, 1.0f));
	script->pushCommand(new MoveCommand(ship, glm::vec3(20, 50, 20), 2.0f, 0.5f, 0.5f));
	return script;
}