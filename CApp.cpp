//==============================================================================
// CApp.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
CApp::CApp() {
	//---------Initialize main surface
    windowDisplay = NULL;
	sdlRenderer = NULL;
	//---------Set Condition
    running = true;
	//---------
}

//==============================================================================
int CApp::onExecute() {
	//---------Make sure initialized
    if(onInit() == false) {
        return -1;
    }
	//---------Event Handling
    SDL_Event event;
	//---------FPS/Game Time
	unsigned int next_game_tick = SDL_GetTicks();
	int loops;
	float interpolation;
	//---------Game Loop
    while(running) {
		//Update Counter
		loops = 0;
		while( (SDL_GetTicks() > next_game_tick) && (loops < MAX_FRAMESKIP)) {
			onLoop();
			while(SDL_PollEvent(&event)) {
				onEvent(&event);
			}

			next_game_tick += SKIP_TICKS;
			loops++;
		}        
        interpolation = float( SDL_GetTicks() + SKIP_TICKS - next_game_tick ) / float(SKIP_TICKS);
        onRender(interpolation);
		//END FPS COUNTER
    }
	//---------Clean Up
    onCleanup();

    return 0x13013;
	//---------
}

//==============================================================================
int main(int argc, char* argv[]) {
    CApp theApp;

    return theApp.onExecute();
}

//==============================================================================
