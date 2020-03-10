#include "MazeDraw.h"
#include "globalStuff.h"
#include "Scene.h"

cGameObject* MazeDraw::cube = nullptr;

void MazeDraw::addToScene(cGameObject* cube)
{
	MazeDraw::cube = cube;
	Scene* theScene = Scene::getTheScene();
	for (int i = 0; i < mazeMaker.maze.size(); i++)
	{
		for (int j = 0; j < mazeMaker.maze[i].size(); j++)
		{
			if (mazeMaker.maze[i][j][0])
			{
				auto newCube = new cGameObject(cube);
				newCube->setPos(coordToWorldSpace(i, j));
				newCube->isVisible = true;
				theScene->addItem(newCube);
			}
		}
	}
}

glm::vec3 MazeDraw::coordToWorldSpace(int i, int j)
{
	return vec3(i * cube->scale, 0, j * cube->scale);
}
