//==============================================================================
// _CENTITY_H_
//==============================================================================
#ifndef _CENTITY_H_
	#define _CENTITY_H_

#include <vector>
#include <glew.h>
#include <string>
#include <algorithm>
#include <cmath>

const int MAX_VECTOR = 4;
const int VECTOR_RIGHT = 0;
const int VECTOR_UP = 1;
const int VECTOR_LEFT= 2;
const int VECTOR_DOWN = 3;

const int LEFT_RIGHT = 0;
const int UP_DOWN = 1;

const int MAX_ROTATION_STATES = 3;
const int ROTATE_LEFT = 0;
const int ROTATE_RIGHT = 1;

const int MAX_ATTRIBUTES = 2;
const int MOVEABLE = 0;
const int INTERPOLATE = 1;

typedef unsigned char BYTE;

struct Vertex {
	double x; 
	double y;
};

struct Vector2D {
	double xComponent;
	double yComponent;
	Vector2D operator*= (double& other) {
		this->xComponent *= other;
		this->yComponent *= other;
		return *(this);
	}
	double magnitude() {
		return std::sqrt(std::pow(xComponent,2) + std::pow(yComponent,2));
	}
};

//==============================================================================
class CEntity {
	private:
		static std::vector<CEntity*> entityList; 
	protected:		
		int stateRotation[MAX_ROTATION_STATES];
		int stateMovement[MAX_VECTOR];
		//-Movement Related Variables
		double bounds[MAX_VECTOR];
		double displacements[MAX_VECTOR/2];
		double excessDisplacements[MAX_VECTOR/2];
		double velocities[MAX_VECTOR];
		double accelerations[MAX_VECTOR];
		double maxAcceleration;
		double accelerationIncrement;
		double maxSpeed;
		double turnSpeed;			
		//-Rotation Related Variables
		double lookToX;
		double lookToY;
		double directionTolerance;
		double x;
		double y;
		double width;
		double height;
	    double direction;
		double rX, rY, rDir;		
    
	public:		

		//-Rendering / Collision
		Vertex vertices[45];
		int numVertices;
		//-------0 = Immovable?
		//-------1 = Temporarily immovable?
		bool attributes[MAX_ATTRIBUTES];
		//-------Animation
		unsigned int currentFrame;
		unsigned int maxFrames;
		//-------Textures
		double texWidth;
		double texHeight;
		GLuint texID[255];//Array of Textures
		int numTexLoaded;
		std::string texFile[255];
		int twoLoops;

	//-------
	public:
		CEntity();
		virtual ~CEntity();

	//-------
	public:
		void loadTexture();
		double getHeight();
		double getRadius();
		double getWidth();
		double getX();
		double getY();
		double getRX(double interpolation);
		double getRY(double interpolation);
		void setBounds(double right, double up, double left, double down);
		void setHeight(double height);
		void setWidth(double width);
		void setX(double x);
		void setY(double y);
		void move();
		static CEntity* getEntityAt(int a);
		static int getNumEntities();
		virtual void onInit();
		virtual void onLoop();
		virtual void onRender(double interpolation);
		virtual void onCleanup();
		

	//-------
};

//==============================================================================

#endif
