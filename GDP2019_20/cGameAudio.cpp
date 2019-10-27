#include "cGameAudio.h"
#include "globalStuff.h"

#include <algorithm>

cGameAudio::cGameAudio() {
	pitch = 1.0f;
	dsp_echo = 0;
	dsp_distortion = 0;
	dsp_chorus = 0;
	dsp_tremolo = 0;

}

void cGameAudio::applyDSPs() {
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp_echo));
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dsp_distortion));
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dsp_chorus));
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp_tremolo));

	channel_group->addDSP(0, dsp_echo);
	channel_group->addDSP(0, dsp_distortion);
	channel_group->addDSP(0, dsp_chorus);
	channel_group->addDSP(0, dsp_tremolo);

	dsp_echo->setBypass(true);
	dsp_distortion->setBypass(true);
	dsp_chorus->setBypass(true);
	dsp_tremolo->setBypass(true);
}

glm::mat4x4 cGameAudio::calculateTransformationMatrix() { return glm::lowp_mat4x4(1.0f); }
void cGameAudio::draw(){ }
std::string cGameAudio::getName() { return name; }
glm::vec3 cGameAudio::getPos() { return position; }
std::string cGameAudio::getType() { return "audio"; }

void cGameAudio::recieveMessage(sMessage message){ 
	float translationStep = 1.0f, step;
	//printf("Sound recieving message %s with %f\n", message.name.c_str(), message.fValue);
	if (message.name == "translate") {
		setPos(position + glm::normalize(message.v3Value) * translationStep);
	}
	if (message.name == "volume") {
		step = 0.2f;
		volume += message.fValue * step;
		volume = std::max(volume, 0.0f);
		volume = std::min(volume, 10.0f);
		channel_group->setVolume(volume);
	}
	if (message.name == "pitch") {
		step = 0.01f;
		pitch += message.fValue * step;
		pitch = std::max(pitch, 0.5f);
		pitch = std::min(pitch, 2.0f);
		channel_group->setPitch(pitch);
	}
	if (message.name == "speed") {

	}
	if (message.name == "toggle") {
		bool is_paused;
		channel_group->getPaused(&is_paused);
		channel_group->setPaused(!is_paused);
	}
	if (message.name == "dsp") {
		bool bypass;
		switch (message.iValue) {
		case 1:
			dsp_echo->getBypass(&bypass);
			dsp_echo->setBypass(!bypass);
			break;
		case 2:
			dsp_tremolo->getBypass(&bypass);
			dsp_tremolo->setBypass(!bypass);
			break;
		case 3:
			break;
		case 4:
			break;
		}
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

json cGameAudio::toJSON() {
	json jAudio;

	jAudio["name"] = name;
	jAudio["volume"] = volume;

	jAudio["position"][0] = position.x;
	jAudio["position"][1] = position.y;
	jAudio["position"][2] = position.z;

	for (int i = 0; i < audios.size(); i++)
	{
		jAudio["audios"][i] = audios[i]->path;
	}

	return jAudio;
}