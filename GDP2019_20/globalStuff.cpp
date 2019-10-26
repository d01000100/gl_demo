#include "globalStuff.h"
#include <fmod/fmod_errors.h>

FMOD_VECTOR glm_2_fmod_vec(glm::vec3 v) {
	return { v.x, v.y, v.z };
}

glm::vec3 fmod_vec_2_glm(FMOD_VECTOR v) {
	return glm::vec3(v.x, v.y, v.z);
}

void error_check(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		fprintf(stderr, "FMOD error: %s\n", FMOD_ErrorString(result));
	}
}