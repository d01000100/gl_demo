#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include "cMesh.h"
#include "GLCommon.h"
#include "GameItemFactory/iGameItem.h"

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

struct sTextureSpec {
	std::string textureName;
	float weight = 1.0f;
};

class cGameObject : public iGameItem
{
public:
	cGameObject();
	// Used to draw this mesh
	std::string meshName;
	cMesh* mesh;
	std::string friendlyName;
	std::vector<sTextureSpec> textures;
	std::vector<glm::vec3> collision_points;

	glm::vec3  position;
	float scale, alpha, lifeTime;

	glm::mat4 matWorld;

	glm::vec4 diffuseColor;		// Colour of the object
	glm::vec4 specularColor;		// Colour of the HIGHLIGHT + Shininess
	                                // RGB - colour
	                                // 4th value (a or w) is the "power"
	                                // 1.0 to 10,000.0f 
	bool isWireframe;
	bool isVisible, isLit;

	sPhysicsObject* physics;

	unsigned int getUniqueID(void);

	void setDebugRenderer(iDebugRenderer* pDebugRenderer);

	glm::mat4 calculateTransformationMatrix();
	std::vector<glm::vec3> getTransformedCollisionPoints();

	void draw();
	std::string getName();
	glm::vec3 getPos();
	void setPos(glm::vec3);

	void recieveMessage(sMessage message);
	std::string getType();
	std::string getInfo();
	json toJSON();

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

	iDebugRenderer* m_pDebugRenderer;

	void IntegrationStep(float deltaTime);
};

#endif


