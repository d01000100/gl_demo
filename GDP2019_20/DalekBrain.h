#pragma once
#include "cGameObject.h"

struct DalekBrain
{
	static std::vector<DalekBrain*> daleks;
	static const float speed;
	static vGameObjects getDaleksObjs();
	static glm::vec2 generateEmptyCoord();
	static glm::vec3 generateEmptyPoint();
	static void generateDaleks(int n);
	static glm::vec3 coordToMiddlePoint(glm::vec2 coord);
	/*
	 * Not only not a woll but inside the maze
	 */
	static bool isValidCoord(glm::vec2 coord);
	static void threadMoveDalek(DalekBrain* dalek);

	glm::vec2 target, origin;
	cGameObject* gameObj = nullptr;
	void moveDalek();
	glm::vec2 randomFreeDir();
	int freeDistanceInDir(glm::vec2 dir);
	bool isTargetReached();
};

typedef std::vector<DalekBrain*> vDaleks;