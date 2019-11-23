#ifndef QUATERNION_UTILS_H
#define QUATERNION_UTILS_H

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

using namespace glm;

/* Taken from https://github.com/opengl-tutorials/ogl/blob/master/common/quaternion_utils.cpp */

quat RotationBetweenVectors(vec3 start, vec3 dest);

quat LookAt(vec3 direction, vec3 desiredUp);

quat RotateTowards(quat q1, quat q2, float maxAngle);

#endif