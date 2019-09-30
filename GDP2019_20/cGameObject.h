#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

enum eShapeTypes
{
	AABB, 
	SPHERE, 
	CAPSULE,
	PLANE,
	MESH,		// Triangle test
	UNKNOWN
};

class cGameObject
{
public:
	cGameObject();
	// Used to draw this mesh
	std::string meshName;			//"Pirate"

	// Values that we can assign and then look for them
	unsigned int friendlyIDNumber;
	std::string friendlyName;

	glm::vec3  positionXYZ;
	glm::vec3  rotationXYZ;
	glm::vec4  objectColourRGBA;	

	// Likely want this now:
	glm::vec4 diffuseColour;		// Colour of the object
	glm::vec4 specularColour;		// Colour of the HIGHLIGHT + Shininess
	                                // RGB - colour
	                                // 4th value (a or w) is the "power"
	                                // 1.0 to 10,000.0f 

	float scale;;    

	// Add some physics things
	glm::vec3 velocity;
	glm::vec3 accel;

	// If the object has an inverse mass of 0.0
	//	then it's not updated by the physics code
	float inverseMass;	// 0.0f = Doesn't move

	//bool bIsDynamic;

	eShapeTypes physicsShapeType;

	// Won't be lit, and is wireframe
	bool isWireframe;
	glm::vec4 debugColour;

	bool isVisible;

	unsigned int getUniqueID(void);

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;
};

#endif


