#pragma once
#include <game_math.h>  // single include header for all glm math stuffs
#include "iPhysicsComponent.h"

namespace nPhysics
{
    struct sClothDef
    {
	    glm::vec3 CornerA, CornerB, DownDirection;
        size_t NumNodesAcross, NumNodesDown;
        float NodeMass;
    };

    class iClothComponent : public iPhysicsComponent
    {
    public:
        virtual ~iClothComponent() = default;
        /*
        Things that the graphic part of the engine must now
        to render the soft body:
        - Amount of nodes
        - Position of nodes
        - Radius of nodes
        */
        virtual size_t NumNodes() = 0;
        virtual bool GetNodeRadius(size_t index, float& radiusOut) = 0;
        virtual bool GetNodePosition(size_t index, glm::vec3& posOut) = 0;
	protected:
        iClothComponent() : iPhysicsComponent(eComponentType::cloth){}
    private:
        iClothComponent(const iClothComponent& other) = delete;
        iClothComponent& operator=(const iClothComponent& other) = delete;
    };
}
