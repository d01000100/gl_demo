#include "cGameAudio.h"
#include "globalStuff.h"

#include <algorithm>

cGameAudio::cGameAudio() {
	pitch = 1.0f;
}

void cGameAudio::applyDSPs() {
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_ECHO, &effects[0].dsp));
	effects[0].name = "ECHO";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &effects[1].dsp));
	effects[1].name = "TREMOLO";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_CHORUS, &effects[2].dsp));
	effects[2].name = "CHORUS";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_DELAY , &effects[3].dsp));
	effects[3].name = "DELAY";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &effects[4].dsp));
	effects[4].name = "PITCHSHIFT";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &effects[5].dsp));
	effects[5].name = "SFXRVERB";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_CONVOLUTIONREVERB, &effects[6].dsp));
	effects[6].name = "CONVOLUTION REVERB";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &effects[7].dsp));
	effects[7].name = "FLANGE";
	::error_check(::fmod_system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &effects[8].dsp));
	effects[8].name = "OSCILLATOR";

	for (int i = 0; i < 9; i++) {
		channel_group->addDSP(0, effects[i].dsp);
		effects[i].dsp->setBypass(true);
	}
}

glm::mat4x4 cGameAudio::calculateTransformationMatrix() { return glm::lowp_mat4x4(1.0f); }
void cGameAudio::draw(){ }
std::string cGameAudio::getName() { return name; }
glm::vec3 cGameAudio::getPos() { return position; }
std::string cGameAudio::getType() { return "audio"; }

void cGameAudio::recieveMessage(sMessage message){ 
	float translationStep = 1.0f, step;
	//printf("Sound recieving message %s with %d\n", message.name.c_str(), message.iValue);
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
		effects[message.iValue - 1].dsp->getBypass(&bypass);
		effects[message.iValue - 1].dsp->setBypass(!bypass);
	}
}

std::string cGameAudio::getInfo() {
	std::stringstream ss;
	ss.precision(2);
	FMOD_VECTOR the_actual_pos;
	audios[0]->channel->get3DAttributes(&the_actual_pos, &fmod_vel);
	ss << getType() << " - " << name
		<< " position: (" << the_actual_pos.x << "," << the_actual_pos.y << "," << the_actual_pos.z << ")";
	bool is_paused;
	channel_group->getPaused(&is_paused);
	ss << (is_paused ? ", is paused" : ", is playing");
	ss << ", volume: " << volume << ", pitch: " << pitch
		<< ", activeDSPs: ";
	for (int i = 0; i < 9; i++) {
		bool bypass;
		effects[i].dsp->getBypass(&bypass);
		if (!bypass) { ss << effects[i].name << ", "; }
	}
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