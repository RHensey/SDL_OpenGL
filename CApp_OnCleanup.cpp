//==============================================================================
// CApp_OnCleanup.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::onCleanup() {
	//---------Call Entities Cleanup
	for(unsigned int a = 0; a < CEntity::entityList.size(); a++) {
		if(CEntity::entityList.at(a) != NULL){
			(CEntity::entityList.at(a))->onCleanup();
		}
	}
	//---------Release main surface
    SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(windowDisplay);
	//---------Exit
    SDL_Quit();
}

//==============================================================================
