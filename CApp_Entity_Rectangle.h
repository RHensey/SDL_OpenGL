//==============================================================================
// _CAPP_ENTITY_RECTANGLE_H_
//==============================================================================
#ifndef _CAPP_ENTITY_RECTANGLE_H_
	#define _CAPP_ENTITY_RECTANGLE_H_

#include "CEntity.h"
//==============================================================================	
class EntityRectangle : public CEntity {
	private:

	public:
		EntityRectangle();

		void onInit();
		void onLoop();
		void onRender(float interpolation);
		void onCleanup();
};

//==============================================================================

#endif