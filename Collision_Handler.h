//==============================================================================
// _CAPP_COLLISION_HANDLER_
//==============================================================================
#ifndef _CAPP_COLLISION_HANDLER_H_
	#define _CAPP_COLLISION_HANDLER_H_

#include "CApp_Entity_Map.h"
#include "CEntity.h"
#include <vector>
#include <functional>
#include <algorithm>

struct entityMinDistance { 
	Vector2D distance;
	CEntity* entity1;
	CEntity* entity2;
	bool operator ==(const entityMinDistance& other) {
		return (((other.entity1 == this->entity1) && (other.entity2 == this->entity2)) || ((other.entity1 == this->entity2) && (other.entity2 == this->entity1)));
	} 
	bool operator ==(const CEntity* other) {
		return (other == this->entity1 || other == this->entity2);
	} 
};

struct collision {
	std::vector <CEntity*> collidingEntities;
	std::vector <entityMinDistance> minDistances;	
};

//==============================================================================
class CollisionHandler {
	private:
		 std::vector<CEntity*> possibleCollisions;
		 std::vector<collision> currentCollisions;	
		 static bool compareCenterX(CEntity* a, CEntity* b);
		 static bool compareCenterY(CEntity* a, CEntity* b);
		 char* msgbuf; 
	public:
		 CollisionHandler();
		 bool hasCollisions();
		 void broadPhaseCheck(double xOffset, double yOffset);
		 void narrowPhaseCheck(double xOffset, double yOffset);
		 void handleCollisions();
		 void mergeCollisions();
};

//==============================================================================
#endif