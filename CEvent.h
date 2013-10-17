//==============================================================================
// _CEVENT_H_
//==============================================================================
#ifndef _CEVENT_H_
    #define _CEVENT_H_   
   
#include<SDL.h>

//==============================================================================
class CEvent {
    public:   
        CEvent();
        virtual ~CEvent();

    public:
        virtual void onEvent(SDL_Event *anEvent);
        virtual void onInputFocus();
        virtual void onInputBlur();
        virtual void onKeyPress(SDL_Keycode sym, Uint16 mod);
        virtual void onKeyRelease(SDL_Keycode sym, Uint16 mod);
        virtual void onMouseFocus();
        virtual void onMouseBlur();
        virtual void onMouseMove(int mX, int mY, int relX, int relY, bool left, bool right, bool middle);
        virtual void onMouseWheel(bool up, bool down);
        virtual void onLButtonDown(int mX, int mY);
        virtual void onLButtonUp(int mX, int mY);
        virtual void onRButtonDown(int mX, int mY);
        virtual void onRButtonUp(int mX, int mY);
        virtual void onMButtonDown(int mX, int mY);
        virtual void onMButtonUp(int mX, int mY);
        virtual void onMinimize();
        virtual void onRestore();
        virtual void onResize(int w, int h);
        virtual void onExpose();
        virtual void onExit();
        virtual void onUser(Uint8 type, int code, void *data1, void *data2);

};

//==============================================================================
#endif