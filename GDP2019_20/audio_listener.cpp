#include "audio_listener.h"
#include "globalStuff.h"

AudioListener::AudioListener(FMOD::System* system)
{
	last_position = { 0.0f, 0.0f, 0.0f };
	position = { 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f }; 
	forward = { 1.0f, 0.0f, 0.0f };
	velocity = {0.0f, 0.0f, 0.0f };
	stepsize = 1.0f;
	_result = ::fmod_system->set3DListenerAttributes(0,
		&position,
		&velocity,
		&forward,
		&up);
	error_check();
}

AudioListener::~AudioListener()
{

}

void AudioListener::setPosition(FMOD_VECTOR pos) {
	position = pos;
	_result = ::fmod_system->set3DListenerAttributes(0,
		&position,
		&velocity,
		&forward,
		&up);
	error_check();
}

std::string AudioListener::get_position_info() {
	std::ostringstream ss;
	ss.precision(2);

	ss << "X: " << position.x << " Y: " << position.y << " Z: " << position.z;

	return ss.str();
}

void AudioListener::error_check() {
	if (this->_result != FMOD_OK) {
		fprintf(stderr, "FMOD error: %s", FMOD_ErrorString(this->_result));
		exit(1);
	}
}