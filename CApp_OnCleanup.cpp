//==============================================================================
// CApp_OnCleanup.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::onCleanup() {
	//---------Call Entities Cleanup
	for(unsigned int a = 0; a < CEntity::getNumEntities(); a++) {
		if(CEntity::getEntityAt(a) != NULL){
			(CEntity::getEntityAt(a))->onCleanup();
		}
	}
	//---------Release main surface
    SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(windowDisplay);
	//---------Exit
    SDL_Quit();
}

//==============================================================================
