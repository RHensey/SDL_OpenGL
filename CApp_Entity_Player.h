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
		//-States
		int stateMovement[MAX_MOVEMENT_STATES];
		int stateRotation[MAX_ROTATION_STATES];
		//-Movement
		float turnSpeed;
		float moveSpeed;
		float vAccelerationCurrent;
		float hAccelerationCurrent;
		float accelerationMax;
		float accelerationIncrement;
		//-stateRotation
		float lookToX;
		float lookToY;
		float directionTolerance;
		//-Map Stuff
		float x1Bound, x2Bound, y1Bound, y2Bound;

	public:
		//-Map Stuff
		EntityMap *currentMap;
		void setBounds(float lBound, float rBound, float tBound, float bBound);
		void setCurrentMap(EntityMap *newMap);

		EntityPlayer();
		void onInit();
		void onLoop();
		void onRender(float interpolation);
		void onCleanup();
		void onKeyPress(SDL_Keycode sym);
		void onKeyRelease(SDL_Keycode sym);
		void onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
};

//==============================================================================

#endif