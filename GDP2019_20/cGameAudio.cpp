#include "cGameAudio.h"
#include "globalStuff.h"

glm::mat4x4 cGameAudio::calculateTransformationMatrix() { return glm::lowp_mat4x4(1.0f); }
void cGameAudio::draw(){ }
std::string cGameAudio::getName() { return name; }
glm::vec3 cGameAudio::getPos() { return position; }
std::string cGameAudio::getType() { return "audio"; }

void cGameAudio::recieveMessage(sMessage message){ 
	float translationStep = 1.0f;

	if (message.name == "translate") {
		setPos(position + glm::normalize(message.v3Value) * translationStep);
	}
	if (message.name == "volume") {

	}
	if (message.name == "pitch") {

	}
	if (message.name == "speed") {

	}
	if (message.name == "pause") {

	}
	if (message.name == "dsp") {

	}
}

std::string cGameAudio::getInfo() {
	std::stringstream ss;
	FMOD_VECTOR the_actual_pos;
	audios[0]->channel->get3DAttributes(&the_actual_pos, &fmod_vel);
	ss << getType() << " - " << name
		<< " position: (" << the_actual_pos.x  << "," << the_actual_pos.y << "," << the_actual_pos.z << ")";
	return ss.str();
}

void cGameAudio::setPos(glm::vec3 pos) {
	position = pos;
	fmod_pos = glm_2_fmod_vec(pos);
	//::error_check(channel_group->set3DAttributes(&fmod_pos, &fmod_vel));

	for (int i = 0; i < audios.size(); i++)
	{
		AudioItem* ai = audios[i];

		::error_check(ai->channel->set3DAttributes(&fmod_pos, &fmod_vel));
	}

	::fmod_system->update();
}