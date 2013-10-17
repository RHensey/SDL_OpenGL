//==============================================================================
// CApp_OnRender.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::onRender(float interpolation) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//---------Call entities rendering
	for(unsigned int a = 0; a < CEntity::entityList.size(); a++) {
		(CEntity::entityList.at(a))->onRender(interpolation);
	}

    SDL_GL_SwapWindow(windowDisplay);
}

//==============================================================================
