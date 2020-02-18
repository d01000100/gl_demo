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
	currentAnimation("idle")
{}

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

void AnimationManager::changeAnimation(std::string animation)
{
	// Don't reset if we're actually not changing
	if (currentAnimation != animation)
	{
		currentAnimation = animation;
		timer = 0.0f;
	}
}

void AnimationManager::update(
	std::vector<glm::mat4>& FinalTransformation,
	std::vector<glm::mat4>& Globals,
	std::vector<glm::mat4>& Offsets)
{
	timer += ::deltaTime;
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