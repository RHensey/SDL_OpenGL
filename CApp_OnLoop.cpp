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
			CEntity::getEntityAt(a)->attributes[INTERPOLATE] = false;
			if(CEntity::getEntityAt(a)->twoLoops == 0) {
				CEntity::getEntityAt(a)->twoLoops++;
			} else {
				CEntity::getEntityAt(a)->attributes[1] = false;
				CEntity::getEntityAt(a)->twoLoops = 0;
			}
		}
	}
	//Collision Detection
	colHandler.broadPhaseCheck(entityMap.getX(),entityMap.getY());	
	if(colHandler.hasCollisions()) {
		colHandler.handleCollisions();
	}		
}

//==============================================================================
