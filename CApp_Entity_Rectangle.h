//==============================================================================
// _CAPP_ENTITY_RECTANGLE_H_
//==============================================================================
#ifndef _CAPP_ENTITY_RECTANGLE_H_
	#define _CAPP_ENTITY_RECTANGLE_H_

#include "CEntity.h"
//==============================================================================	
class EntityRectangle : public CEntity {
	private:
		char * msgbuf;
	public:
		int moveDir;
		EntityRectangle();

		void onInit();
		void onLoop();
		void onRender(double interpolation);
		void onCleanup();
};

//==============================================================================

#endif