//==============================================================================
// CApp_OnLoop.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::onLoop() {
	//---------Call Entities Loop
	for(unsigned int a = 0; a < CEntity::entityList.size(); a++) {
		if(CEntity::entityList.at(a) != NULL){
			(CEntity::entityList.at(a))->onLoop();
			(CEntity::entityList.at(a))->hadCollision = false;
		}
	}
	//Collision Detection
	colHandler.broadPhaseCheck(entityMap.getXOffset(),entityMap.getYOffset());	
	if(colHandler.hasCollisions()) {
		colHandler.handleCollisions();
	}		
}

//==============================================================================
