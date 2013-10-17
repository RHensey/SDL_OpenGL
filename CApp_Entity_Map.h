//==============================================================================
// _CAPP_ENTITY_MAP_H_
//==============================================================================
#ifndef _CAPP_ENTITY_MAP_H_
	#define _CAPP_ENTITY_MAP_H_

#include "CEntity.h"
#include "Windows.h"//temporary

struct tile {
	BYTE textureIndex;
	BYTE passable;
};
struct map {
	BYTE mapIndex;
	unsigned short int width;
	unsigned short int height;
	std::vector<tile> tileList;
};

//==============================================================================
class EntityMap : public CEntity {
	private:
		//-Map Info
		std::vector<map> mapList;
		BYTE currentMapIndex;
	    //-States
		int stateMovement[MAX_MOVEMENT_STATES];
		int possibleScrollDir[MAX_MOVEMENT_STATES];
		//-Position
		float hSpeed, vSpeed, hSpeedMax, vSpeedMax;
		float currentXOffset;
		float currentYOffset;
		float rX, rY;

	public:
		void scroll(int direction, float speed);
		int canScroll(int direction);
		float getXOffset();
		float getYOffset();		
		void loadMapList();
		EntityMap();
		void onInit();
		void onLoop();
		void onRender(float interpolation);
		void onCleanup();
		void setDimensions(int width, int height);
};

//==============================================================================
#endif