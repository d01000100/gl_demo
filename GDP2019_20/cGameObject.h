#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "cMesh.h"

// NOTE: We are including the INTERFACE, not the actual renderer
#include "DebugRenderer/iDebugRenderer.h"

enum eShapeTypes
{
	AABB,					// min and max corners 
	SPHERE,					// Radius
	CAPSULE,			
	PLANE,
	MESH,		// Triangle test
	UNKNOWN
};

struct sPhysicsObject {
	float speed;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	bool gravity;

	// We could contain another class or struct with the phsyics stuff
	// or we could use this type to determine what we need to check.
	eShapeTypes shape;
	float radius;
	cMesh* mesh;
};

class cGameObject
{
public:
	cGameObject();
	// Used to draw this mesh
	std::string meshName;
	cMesh* mesh;
	std::string friendlyName;

	glm::vec3  position;
	glm::vec3  rotationXYZ;
	glm::vec3 front;
	float scale;

	glm::mat4 matWorld;

	//glm::vec4  objectColourRGBA;	
	glm::vec4 diffuseColor;		// Colour of the object
	glm::vec4 specularColor;		// Colour of the HIGHLIGHT + Shininess
	                                // RGB - colour
	                                // 4th value (a or w) is the "power"
	                                // 1.0 to 10,000.0f 
	bool isWireframe;
	bool isVisible;

	sPhysicsObject* physics;

	unsigned int getUniqueID(void);

	void setDebugRenderer(iDebugRenderer* pDebugRenderer);

	glm::mat4 calculateTransformationMatrix();

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

	iDebugRenderer* m_pDebugRenderer;
};

#endif


