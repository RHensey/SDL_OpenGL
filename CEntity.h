//==============================================================================
// _CENTITY_H
//==============================================================================
#ifndef _CENTITY_H_
	#define _CENTITY_H_

#include <vector>
#include <glew.h>
#include <string>

const int IDLE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int UP = 3;
const int DOWN = 4;

const int MAX_ATTRIBUTES = 2;
const int MOVEABLE = 0;

const int MAX_MOVEMENT_STATES = 5;
const int MAX_ROTATION_STATES = 3;	

typedef unsigned char BYTE;

struct Vertex {
	float x; 
	float y;
};

//==============================================================================
class CEntity {
	//-------
	public:
		static std::vector<CEntity*> entityList;

    //-------
	public:
		bool hadCollision;
		float x;
		float y;
		float direction;
		float rX, rY, rDir;
		int width;
		int height;
		//-Rendering / Collision
		Vertex vertices[45];
		int numVertices;
		bool recentCollisions[5];
		//-------0 = Static? 
		//-------1 = Special Function?
		bool attributes[MAX_ATTRIBUTES];
		//-------Animation
		unsigned int currentFrame;
		unsigned int maxFrames;
		//-------Textures
		int texWidth;
		int texHeight;
		GLuint texID[255];//Array of Textures
		int numTexLoaded;
		std::string texFile[255];

	//-------
	public:
		CEntity();
		virtual ~CEntity();

	//-------
	public:
		void loadTexture();
		float getRadius();
		float getWidth();
		float getHeight();
		virtual void onInit();
		virtual void onLoop();
		virtual void onRender(float interpolation);
		virtual void onCleanup();
		

	//-------
};

//==============================================================================

#endif
