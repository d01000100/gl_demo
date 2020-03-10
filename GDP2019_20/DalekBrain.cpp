#include "DalekBrain.h"
#include "globalStuff.h"
#include "util.h"
#include "MazeDraw.h"
#include "Scene.h"

vDaleks DalekBrain::daleks;
const float DalekBrain::speed = 0.002f;

vGameObjects DalekBrain::getDaleksObjs()
{
	vGameObjects res;
	for (auto dalek : daleks)
	{
		res.push_back(dalek->gameObj);
	}
	return res;
}

glm::vec2 DalekBrain::generateEmptyCoord()
{
	int i = 0, j = 0;
	do
	{
		i = randInRange(0, mazeMaker.maze.size() - 1);
		j = randInRange(0, mazeMaker.maze[0].size() - 1);
	} while (mazeMaker.maze[i][j][0]);
	return glm::vec2(i,j);
}

glm::vec3 DalekBrain::generateEmptyPoint()
{
	return coordToMiddlePoint(generateEmptyCoord());
}

void DalekBrain::generateDaleks(int n)
{
	Scene* theScene = Scene::getTheScene();
	for (int i = 0; i < n; ++i)
	{
		auto dalek = new DalekBrain();
		dalek->gameObj = new cGameObject((cGameObject*)theScene->findItem("dalek_template"));
		dalek->gameObj->isVisible = true;
		dalek->origin = generateEmptyCoord();
		dalek->target = dalek->origin;
		dalek->gameObj->setPos(coordToMiddlePoint(dalek->origin));
		theScene->addItem(dalek->gameObj);
		daleks.push_back(dalek);
	}
}

glm::vec3 DalekBrain::coordToMiddlePoint(glm::vec2 coord)
{
	auto point = MazeDraw::coordToWorldSpace(coord.s, coord.t);
	// So it's in the middle of the square, not at the beggining of it.
	point += glm::vec3(1, 0, 1) * MazeDraw::cube->scale * 0.5f;
	return point;
}

bool DalekBrain::isValidCoord(glm::vec2 coord)
{
	return coord.s >= 0 && coord.s < ::mazeMaker.maze.size() &&
		coord.t >= 0 && coord.t < ::mazeMaker.maze[0].size() &&
		!::mazeMaker.maze[coord.s][coord.t][0];
}

void DalekBrain::threadMoveDalek(DalekBrain* dalek)
{
	while (true)
	{
		dalek->moveDalek();
	}
}

void DalekBrain::moveDalek()
{
	glm::vec2 dir = glm::normalize(target - origin);
	// When you get (close) to the target
	if (isTargetReached())
	{
		// get right to the target
		gameObj->setPos(coordToMiddlePoint(target));
		origin = target;
		// Choose a random free direction.
		dir = randomFreeDir();
		// Choose a random distance in that direction. And choose a target
		int availableDistance = freeDistanceInDir(dir);
		int distance = randInRange(1, availableDistance);
		target = origin + dir * (float)distance;
	}

	// Smoothly move in that direction.
	glm::vec3 worldDir = glm::vec3(dir.s, 0, dir.t);
	auto objPos = gameObj->getPos();
	gameObj->setDirection(worldDir);
	objPos += worldDir * speed * (float)::avgDeltaTimeThingy.getAverage();
	gameObj->setPos(objPos);

	
	//// Debug thingys!!
	//// is direction correct???
	//::g_pDebugRenderer->addLine(
	//	objPos + glm::vec3(0,2,0),
	//	objPos + worldDir * 10.f,
	//	Colors::white,
	//	0.1f);
	//// is target (distance) correct??
	//glm::vec3 worldTarget = coordToMiddlePoint(target);
	//::g_pDebugRenderer->addLine(objPos, worldTarget, Colors::green, 0.1f);
}

glm::vec2 DalekBrain::randomFreeDir()
{
	glm::vec2 direction, newPos;
	do
	{
		int dir = randInRange(1, 5);
		switch (dir)
		{
		case 1:
			direction = glm::vec2(1, 0);
			break;
		case 2:
			direction = glm::vec2(0, 1);
			break;
		case 3:
			direction = glm::vec2(-1, 0);
			break;
		default:
			direction = glm::vec2(0, -1);
			break;
		}
		newPos = origin + direction;
	} while (!isValidCoord(newPos));
	return direction;
}

int DalekBrain::freeDistanceInDir(glm::vec2 dir)
{
	int distance = 0;
	glm::vec2 scanner = origin;
	while (isValidCoord(scanner))
	{
		scanner += dir;
		distance++;
	}
	return distance;
}

bool DalekBrain::isTargetReached()
{
	return glm::distance(gameObj->getPos(), coordToMiddlePoint(target)) < 1.f;
}
