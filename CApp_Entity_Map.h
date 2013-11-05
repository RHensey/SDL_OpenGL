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

public:
		double scroll(int direction, double displace);
		bool canScroll(int direction, double distance);
		double getXOffset();
		double getYOffset();		
		void loadMapList();
		EntityMap();
		void onInit();
		void onLoop();
		void onRender(double interpolation);
		void onCleanup();
		void setDimensions(int width, int height);
};

//==============================================================================
#endif