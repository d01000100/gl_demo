#include "AnimationManager.h"
#include "globalStuff.h"
#include "AABBGrid.h"
#include "AABBGrid.h"
#include "AABBGrid.h"
#include "AABBGrid.h"
#include "AABBGrid.h"
#include "AABBGrid.h"
#include "util.h"

AnimationManager::AnimationManager(cSimpleAssimpSkinnedMesh* anim) :
	assimpAnim(anim),
	idleAnimation("idle"),
	currentAnimation("idle"),
	timer(0.0f)
{}

AnimationManager::~AnimationManager()
{
	delete assimpAnim;
}

bool AnimationManager::isCurrentDone()
{
	if (mapContains(assimpAnim->mapAnimationFriendlyNameTo_pScene, currentAnimation))
	{
		return timer > assimpAnim->FindAnimationTotalTime(currentAnimation);
	}
	else
	{
		return false;
	}
}

void AnimationManager::changeAnimation(std::string animation, float scale)
{
	// Don't reset if we're actually not changing
	if (!animInfo[currentAnimation].isBlocking)
	{
		timeScale = scale;
		if (currentAnimation != animation)
		{
			currentAnimation = animation;
			timer = 0.0f;
		}
	}
}

void AnimationManager::update(
	std::vector<glm::mat4>& FinalTransformation,
	std::vector<glm::mat4>& Globals,
	std::vector<glm::mat4>& Offsets)
{
	timer += ::deltaTime * timeScale;
	if (timer < 0)
	{
		timer = assimpAnim->FindAnimationTotalTime(currentAnimation);
	}
	// If we're in the idle state or not
	if (currentAnimation == idleAnimation)
	{
		if (isCurrentDone())
			timer = 0.0f;
		assimpAnim->BoneTransform(timer,	
			idleAnimation,
			FinalTransformation,
			Globals,
			Offsets);
	}
	else
	{
		assimpAnim->BoneTransform(timer,
			currentAnimation,
			FinalTransformation,
			Globals,
			Offsets);
		if (isCurrentDone())
		{
			timer = 0.0f;
			// back to idle
			currentAnimation = idleAnimation;
		}
	}
}