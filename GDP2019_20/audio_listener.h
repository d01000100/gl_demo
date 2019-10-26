#pragma once

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class AudioListener
{
private:
	float stepsize;
	FMOD_RESULT _result;	
	FMOD::System* _system;
	void error_check();

public:
	FMOD_VECTOR last_position, position, up, forward, velocity;
	AudioListener(FMOD::System* system);
	virtual ~AudioListener();

	void setPosition(FMOD_VECTOR);
	std::string get_position_info();
};

