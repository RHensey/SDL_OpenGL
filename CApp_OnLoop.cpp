//==============================================================================
// CApp_OnLoop.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::onLoop() {
	//---------Call Entities Loop
	for(unsigned int a = 0; a < CEntity::getNumEntities(); a++) {
		if(CEntity::getEntityAt(a) != NULL){
			CEntity::getEntityAt(a)->onLoop();
		}
	}
	//Collision Detection
	colHandler.broadPhaseCheck(entityMap.getX(),entityMap.getY());	
	if(colHandler.hasCollisions()) {
		colHandler.handleCollisions();
	}		
}

//==============================================================================
