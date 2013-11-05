//==============================================================================
// _CAPP_ENTITY_PLAYER_H_
//==============================================================================
#ifndef _CAPP_ENTITY_PLAYER_H_
	#define _CAPP_ENTITY_PLAYER_H_

#include "CEntity.h"
#include "CApp_Entity_Map.h"
#include "Windows.h"//temporary

//==============================================================================	
class EntityPlayer : public CEntity {
	private:		

	public:
		//-Map Stuff
		EntityMap *currentMap;
		void setCurrentMap(EntityMap *newMap);

		EntityPlayer();
		void onInit();
		void onLoop();
		void onRender(double interpolation);
		void onCleanup();
		void onKeyPress(SDL_Keycode sym);
		void onKeyRelease(SDL_Keycode sym);
		void onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
};

//==============================================================================

#endif