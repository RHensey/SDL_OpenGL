//==============================================================================
// CApp_Entity_Player.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
EntityPlayer::EntityPlayer() {

}

//==============================================================================
void EntityPlayer::onInit() {
	x = 1280.0f;
	y = 960.0f;
	//---------
	attributes[MOVEABLE] = false;
	//---------Dimensions
	width = 128.0f;
	height = 128.0f;
	texWidth = 128.0f;
	texHeight = 128.0f;
	//---------Standard values for rotation
	lookToX = 0.0f;
	lookToY = 0.0f;
	directionTolerance = 5.0f;
	turnSpeed = 9.0f;
	setBounds(1760,640,800,1280);
	//---------Standard values for movement
	maxSpeed = 8.0;
	maxAcceleration = 1.0;	
	accelerationIncrement = 0.25;
	//---------Map Ineraction 
	currentMap = NULL;
	//---------Vertices
	numVertices = 45;
	//---------Attributes
	attributes[0] = true;
	//Later add file loading
	for (int a = 0; a < 45; a++) {
		vertices[a].x = 64.0f*cos(a*8*PI/180.0f);
		vertices[a].y = 64.0f*sin(a*8*PI/180.0f);
	}
	//---------Tex File
	maxFrames = 4;
	texFile[0] = "./Resource/EntityPlayer_F.RAW";
	texFile[1] = "./Resource/EntityPlayer_L.RAW";
	texFile[2] = "./Resource/EntityPlayer_R.RAW";
	texFile[3] = "./Resource/EntityPlayer_B.RAW";
}

//==============================================================================
void EntityPlayer::onLoop() {    
	move();
	//SOMETHING IS WRONG WITH DISPLACEMENTS
	if(excessDisplacements[LEFT_RIGHT] > 0) {
		if(currentMap->canScroll(VECTOR_LEFT,abs(excessDisplacements[LEFT_RIGHT]))) {
			double dx = currentMap->scroll(VECTOR_LEFT,abs(excessDisplacements[LEFT_RIGHT]));
			//displacements[LEFT_RIGHT] += dx;
			x += dx;
		}
	} else if(excessDisplacements[LEFT_RIGHT] < 0) {
		if(currentMap->canScroll(VECTOR_RIGHT,abs(excessDisplacements[LEFT_RIGHT]))) {
			double dx = currentMap->scroll(VECTOR_RIGHT,abs(excessDisplacements[LEFT_RIGHT]));
			//displacements[LEFT_RIGHT] += dx;
			x += dx;
		}
	}
	if(excessDisplacements[UP_DOWN] > 0) {
		if(currentMap->canScroll(VECTOR_UP,abs(excessDisplacements[UP_DOWN]))) {
			double dy = currentMap->scroll(VECTOR_UP,abs(excessDisplacements[UP_DOWN]));
			//displacements[UP_DOWN] += dy;
			y += dy;
		}
	} else if(excessDisplacements[UP_DOWN] < 0) {
		if(currentMap->canScroll(VECTOR_DOWN,abs(excessDisplacements[UP_DOWN]))) {
			double dy = currentMap->scroll(VECTOR_DOWN,abs(excessDisplacements[UP_DOWN]));
			//displacements[UP_DOWN] += dy;
			y += dy;
		}
	}
	setBounds(1760-currentMap->getX(),640-currentMap->getY(),800-currentMap->getX(),1280-currentMap->getY());
	//---------rotate to mouse
	double angle = atan2f((lookToX - (x+currentMap->getX())), (lookToY - (y+currentMap->getY())));
	//---------convert angle towards mouseX,mouseY from rad to degrees
	angle = ((angle/PI)*180)+270;
	//---------finish converting by taking down > 360 (added 270 to match other angle mode)
	if(angle >= 360) angle = angle - 360;
	//---------check stateRotation tolerance
	if(abs(angle - direction) > directionTolerance) {
		//---------see which direction to turn(+/-)
		double crossProduct = (cos(direction/180*PI) * sin(angle/180*PI)) - (cos(angle/180*PI)*sin(direction/180*PI));
		if(crossProduct >= 0) {
			if(abs(angle - direction) > 3*directionTolerance) {			
				stateRotation[ROTATE_LEFT] = 1;
				stateRotation[ROTATE_RIGHT] = 0;
			}
			if(direction < 360) {
				//---------turn left
				direction = direction + turnSpeed;
			}
			else {
				direction = 0;
			}
		}
		else {
			if(abs(angle - direction) > 3*directionTolerance) {			
				stateRotation[ROTATE_RIGHT] = 1;
				stateRotation[ROTATE_LEFT] = 0;
			}
			if(direction > 0) {
				//---------turn right
				direction = direction - turnSpeed;
			}
			else {
				direction = 360;
			}
		}
	}
	else {
		stateRotation[ROTATE_LEFT] = 0;
		stateRotation[ROTATE_RIGHT] = 0;
	}
	//---------
}

//==============================================================================
void EntityPlayer::onRender(double interpolation) {	
	//---------Reset
	//glLoadIdentity();
	//---------Move World Coordinates to Player Location	
	rX = getRX(interpolation);
	rY = getRY(interpolation);
	rDir = direction;
	//---------stateRotation Interpolation
	if(stateRotation[ROTATE_LEFT]){
		rDir = direction + turnSpeed*interpolation;
	}
	if(stateRotation[ROTATE_RIGHT]){
		rDir = direction - turnSpeed*interpolation;
	}	
	//---------Translate to interpolated location
	glTranslatef(rX,rY,0.0f);
	//---------Texture
	//Facing Right
	if((rDir < 45) || (rDir > 315)) {currentFrame = 2;}
	//Facing Back
	if((rDir < 135) && (rDir > 45)) {currentFrame = 3;}
	//Facing Left
	if((rDir < 225) && (rDir > 135)) {currentFrame = 1;}
	//Facing Front
	if((rDir < 315) && (rDir > 225)) {currentFrame = 0;}
	
	//Bind Texture
	glBindTexture(GL_TEXTURE_2D, texID[currentFrame]);	
	//---------Reset Texture Matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//---------Rotate texture according to rDir
	//glTranslatef(0.5,0.5,0.0);
	//glRotatef(rDir-90,0.0,0.0,1.0);
	//glTranslatef(-0.5,-0.5,0.0);
	//---------Draw Model
	glMatrixMode(GL_MODELVIEW); 
	//---------
	glBegin(GL_POLYGON);
		for(int a = 0; a < numVertices; a++) {		
			glTexCoord2f(0.5f+(vertices[a].x/width),0.5f+(vertices[a].y/height));
			glVertex3f(vertices[a].x,vertices[a].y,0.0f);
		}	
	glEnd();
	//--------
	glTranslatef(-rX,-rY,0.0f);
}
//==============================================================================
void EntityPlayer::onKeyPress(SDL_Keycode sym) {
	//---------Set stateMovement for key presses
	if(sym == SDLK_a) stateMovement[VECTOR_LEFT] = true;
	if(sym == SDLK_d) stateMovement[VECTOR_RIGHT] = true;
	if(sym == SDLK_w) stateMovement[VECTOR_UP] = true;
	if(sym == SDLK_s) stateMovement[VECTOR_DOWN] = true;
}

//==============================================================================
void EntityPlayer::onKeyRelease(SDL_Keycode sym) {
	//---------Set stateMovement for key releases
	if(sym == SDLK_a) stateMovement[VECTOR_LEFT] = false;
	if(sym == SDLK_d) stateMovement[VECTOR_RIGHT] = false;
	if(sym == SDLK_w) stateMovement[VECTOR_UP] = false;
	if(sym == SDLK_s) stateMovement[VECTOR_DOWN] = false;
}

//==============================================================================
void EntityPlayer::onMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){
	//---------store mouse coords
	lookToX = mX;
	lookToY = mY;
}

//==============================================================================
void EntityPlayer::onCleanup() {
	glDeleteTextures(numTexLoaded,texID);
}

//==============================================================================
void EntityPlayer::setCurrentMap(EntityMap *newMap) {
	currentMap = newMap;

}

