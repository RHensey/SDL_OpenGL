//==============================================================================
// CApp_OnEvent.cpp
//==============================================================================
#include "CApp.h"
//==============================================================================
void CApp::onEvent(SDL_Event* Event) {
    //---------Divert event to appropriate handle
    CEvent::onEvent(Event);
}

//==============================================================================
void CApp::onExit() {
    //---------Break Condition
    running = false;
}

//==============================================================================
void CApp::onKeyPress(SDL_Keycode sym, Uint16 mod) {
    if(sym == SDLK_ESCAPE) {
        running = false;
    }
    //---------Relay event to entity
    entityPlayer.onKeyPress(sym);

}

//==============================================================================
void CApp::onKeyRelease(SDL_Keycode sym, Uint16 mod){
    //---------Relay event to entity
    entityPlayer.onKeyRelease(sym);

}

//==============================================================================
void CApp::onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){
    entityPlayer.onMouseMove(2*(mX-mXOffset),2*(mY-mYOffset),relX,relY,Left,Right,Middle);
    /*glLoadIdentity();
    glTranslatef(mX,mY,0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-5.0f,0.0f,0.0f);
        glVertex3f(5.0f,0.0f,0.0f);
        glVertex3f(0.0f,5.0f,0.0f);
    glEnd();
    SDL_GL_SwapBuffers();*/
}

//==============================================================================