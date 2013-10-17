//==============================================================================
// CApp_OnInit.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
bool CApp::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    //SDL_ShowCursor(SDL_DISABLE);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,            32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

    if((windowDisplay = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          0, 0,
                          SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL)) == NULL) {
        return false;
    }
    SDL_GLContext glcontext = SDL_GL_CreateContext(windowDisplay);   
    //SDL_Renderer *sdlRenderer = SDL_CreateRenderer(windowDisplay, -1, 0);


    glClearColor(0, 0, 0, 0);

    SDL_GetWindowSize(windowDisplay,&mXOffset,&mYOffset);
    if((mXOffset > 1280) && (mYOffset > 960)) {
        mXOffset = (mXOffset-1280.0f)/2.0f;
        mYOffset = (mYOffset-960.0f)/2.0f;
    }
    else {
        mXOffset = (mXOffset-1280.0f)/2.0f;
        mYOffset = (mYOffset-960.0f)/2.0f;
        //do math to find closest 4:3 resolution?
        //use the base height (probably the constraining)
    }

    //how to handle smaller resolutions?
    glViewport(mXOffset, mYOffset, 1280, 960);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //-Mouse Coords will be half what they should be
    glOrtho(0, 2560, 1920, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);

    glLoadIdentity();
    //---------Create Entities
    for(unsigned int a = 0; a < CEntity::entityList.size(); a++) {
        if(CEntity::entityList.at(a) != NULL){
            (CEntity::entityList.at(a))->onInit();
        }
    }
    //---------Load Textures
    for(unsigned int a = 0; a < CEntity::entityList.size(); a++) {
        if(CEntity::entityList.at(a) != NULL){
            for(int b = 0; b < (CEntity::entityList.at(a))->maxFrames; b++) {
                (CEntity::entityList.at(a))->loadTexture();
            }
        }
    }
    //---------Initialize Map Stuff (later  going in EntityWorld's init)
    entityMap.loadMapList();
    entityMap.setDimensions(5120,3840);
    entityPlayer.setCurrentMap(&(entityMap));
    entityPlayer.setBounds(800,1760,640,1280);//************************make fancier
	//meh
	entityRectangle1.x+=10;
	entityRectangle2.x-=10;


    return true;
}

//==============================================================================