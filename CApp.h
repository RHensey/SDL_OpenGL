//==============================================================================
// _CAPP_H
//==============================================================================
#ifndef _CAPP_H_
    #define _CAPP_H_

#include <glew.h>
#include <math.h>
#include "CEvent.h"
#include "CEntity.h"
#include "Collision_Handler.h"
#include "CApp_Entity_Map.h"
#include "CApp_Entity_Player.h"
#include "CApp_Entity_Rectangle.h"
#include <Windows.h>


const float PI = 3.141519265358979323846f;
//-FPS/Game Logic Counters
const int UPDATES_PER_SECOND = 50;
const int SKIP_TICKS = 20;
const int MAX_FRAMESKIP = 5;

//==============================================================================
class CApp : public CEvent {
    private:
        bool running;
        SDL_Window* windowDisplay;
        SDL_GLContext glcontext;
        SDL_Renderer* sdlRenderer;

        //-
        int mXOffset;
        int mYOffset;
        //float sdlToScreenRatio = 2;
        //Drawn First = Behind Other - later can add a depth
        EntityMap entityMap;//Change to Entity World which in turn calls loadMaps which in turn creates maps respectively       
        EntityRectangle entityRectangle1;
		EntityRectangle entityRectangle2;
		EntityRectangle entityRectangle3;
		EntityRectangle entityRectangle4;
		EntityRectangle entityRectangle5;
		EntityRectangle entityRectangle6;
		EntityRectangle entityRectangle7;
		EntityRectangle entityRectangle8;
		EntityRectangle entityRectangle9;
        EntityPlayer entityPlayer;
        //not added to entitylist
        CollisionHandler colHandler;

    public:
        CApp();
        int onExecute();

    public:
        bool onInit();
        void onEvent(SDL_Event* anEvent);
        void onLoop();
        void onRender(double interpolation);
        void onCleanup();

        void onExit();
        void CApp::onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
        void onKeyPress(SDL_Keycode sym, Uint16 mod);
        void onKeyRelease(SDL_Keycode sym, Uint16 mod);

};

//==============================================================================

#endif