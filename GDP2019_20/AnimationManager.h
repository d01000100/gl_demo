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
	std::map<std::string, sAnimationDef> animInfo;
	std::string idleAnimation;
	std::string currentAnimation;
	float timer, timeScale = 1.0f;

	AnimationManager(cSimpleAssimpSkinnedMesh* anim);
	~AnimationManager();
	bool isCurrentDone();
	void changeAnimation(std::string animation, float scale = 1.0f);
	void update(
		std::vector<glm::mat4>& FinalTransformation,
		std::vector<glm::mat4>& Globals,
		std::vector<glm::mat4>& Offsets);
};

