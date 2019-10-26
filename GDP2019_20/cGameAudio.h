#pragma once
#include <glm/vec3.hpp>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include <vector>

#include "GameItemFactory/iGameItem.h"
#include "audio_item.h"

FMOD_VECTOR glm_2_fmod_vec(glm::vec3);

class cGameAudio : public iGameItem
{
private:
public:
	FMOD::ChannelGroup* channel_group;
	std::vector<AudioItem*> audios;
	glm::vec3 position;
	FMOD_VECTOR fmod_pos, fmod_vel;
	std::string name;
	float volume;
	// from iGameItem
	glm::mat4x4 calculateTransformationMatrix();
	void draw();
	std::string getName();
	glm::vec3 getPos();
	void setPos(glm::vec3);
	void recieveMessage(sMessage message);
	std::string getType();
	std::string getInfo();
};

