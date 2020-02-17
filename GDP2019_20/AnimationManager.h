#pragma once
#include <string>
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

/*
 * TODO: Animaciones interrumpibles y no interrumpibles
 */
struct sAnimationDef
{
	std::string name;
	bool isBlocking = false;
};

struct AnimationManager
{
	cSimpleAssimpSkinnedMesh* assimpAnim;
	std::string idleAnimation;
	std::string currentAnimation;
	float timer = 0.0f;

	AnimationManager(cSimpleAssimpSkinnedMesh* anim);
	bool isCurrentDone();
	void changeAnimation(std::string animation);
	void update(
		std::vector<glm::mat4>& FinalTransformation,
		std::vector<glm::mat4>& Globals,
		std::vector<glm::mat4>& Offsets);
};

