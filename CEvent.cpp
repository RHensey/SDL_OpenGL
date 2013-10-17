//==============================================================================
// CEvent.cpp
//==============================================================================
#include "CEvent.h"

//==============================================================================
CEvent::CEvent() {
}

//==============================================================================
CEvent::~CEvent() {
}

//==============================================================================
void CEvent::onEvent(SDL_Event *anEvent) {
    switch(anEvent->type) {

        case SDL_WINDOWEVENT: {

            switch(anEvent->window.event) {

                case SDL_WINDOWEVENT_ENTER:{
                    onMouseFocus();
                    break;
                }

                case SDL_WINDOWEVENT_LEAVE:{
                    onMouseBlur();
                    break;
                }

                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    onInputFocus();                   
                    break;
                }

                case SDL_WINDOWEVENT_FOCUS_LOST: {
                    onInputBlur();
                    break;
                }

                case SDL_WINDOWEVENT_RESTORED: {
                    onRestore();
                    break;
                }

                case SDL_WINDOWEVENT_MINIMIZED: {
                    onMinimize();
                    break;
                }

            }

            break;

        }

        case SDL_KEYDOWN: {
            onKeyPress(anEvent->key.keysym.sym, anEvent->key.keysym.mod);
            break;
        }

        case SDL_KEYUP: {
            onKeyRelease(anEvent->key.keysym.sym,anEvent->key.keysym.mod);
            break;
        }

        case SDL_MOUSEMOTION: {
            onMouseMove(anEvent->motion.x, anEvent->motion.y, anEvent->motion.xrel, anEvent->motion.yrel,(anEvent->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(anEvent->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(anEvent->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            switch(anEvent->button.button) {
                case SDL_BUTTON_LEFT: {
                    onLButtonDown(anEvent->button.x,anEvent->button.y);
                    break;
                 }

                case SDL_BUTTON_RIGHT: {
                    onRButtonDown(anEvent->button.x,anEvent->button.y);
                    break;
                 }

                case SDL_BUTTON_MIDDLE: {
                    onMButtonDown(anEvent->button.x,anEvent->button.y);
                    break;
                 }
            }
            break;
       }
     
        case SDL_MOUSEBUTTONUP: {
            switch(anEvent->button.button) {
                case SDL_BUTTON_LEFT: {
                    onLButtonUp(anEvent->button.x,anEvent->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    onRButtonUp(anEvent->button.x,anEvent->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    onMButtonUp(anEvent->button.x,anEvent->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_QUIT: {
            onExit();
            break;
        }

        case SDL_SYSWMEVENT: {
            //Ignore
            break;
        }

        case SDL_WINDOWEVENT_RESIZED: {
            onResize(anEvent->window.data1,anEvent->window.data2);
            break;
        }

        case SDL_WINDOWEVENT_EXPOSED: {
            onExpose();
            break;
        }

        default: {
            onUser(anEvent->user.type,anEvent->user.code,anEvent->user.data1,anEvent->user.data2);
            break;
        }
    }
}

//==============================================================================
void CEvent::onInputFocus() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onInputBlur() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onKeyPress(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onKeyRelease(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMouseFocus() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMouseBlur() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMouseWheel(bool Up, bool Down) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onLButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onLButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onRButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onRButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onMinimize() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onRestore() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onResize(int w,int h) {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onExpose() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onExit() {
    //Pure virtual, do nothing
}

//==============================================================================
void CEvent::onUser(Uint8 type, int code, void* data1, void* data2) {
    //Pure virtual, do nothing
}

//==============================================================================