//==============================================================================
// CApp_Entity_Player.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
EntityPlayer::EntityPlayer() {

}

//==============================================================================
void EntityPlayer::onInit() {
	//---------Set initial stateMovement to 0
	for (int a = 0; a < MAX_MOVEMENT_STATES; a++) {
		stateMovement[a] = 0;
	}
	//---------
	attributes[MOVEABLE] = false;
	//---------Dimensions
	width = 128.0f;
	height = 128.0f;
	texWidth = 128.0f;
	texHeight = 128.0f;
	//---------Standard values for stateRotation
	lookToX = 0.0f;
	lookToY = 0.0f;
	directionTolerance = 5.0f;
	//---------Standard values for movement
	turnSpeed = 9.0f;
	moveSpeed = 5.0f;
	hAccelerationCurrent = 0.0f;
	vAccelerationCurrent = 0.0f;
	accelerationMax = 5.0f;
	accelerationIncrement = 0.625f;
	//---------Map Ineraction 
	currentMap = NULL;
	x1Bound = 0.0f;
	x2Bound = 0.0f;
	y1Bound = 0.0f;
	y2Bound = 0.0f;
	//---------Vertices
	numVertices = 45;
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
	//---------Check keys for moving right, positive acceleration
	if( !stateMovement[LEFT] == 1 && stateMovement[RIGHT] == 1) {
		//---------Check to see if got rid of left momentum
		if(hAccelerationCurrent >= 0) {
			if(hAccelerationCurrent == 0) {
				currentMap->scroll(IDLE-1,0);
			}
			//---------check in bounds
			if(x + (moveSpeed + abs(hAccelerationCurrent)) < currentMap->getWidth()) {
				x = x + (moveSpeed + abs(hAccelerationCurrent));				
			}
			else {				
				x = currentMap->getWidth();
			}
			if(x > abs(currentMap->getXOffset())+2560) {
				if(currentMap != NULL) {
					//---------scroll opposite (left)
					if(currentMap->canScroll(LEFT) == 1) {
						currentMap->scroll(LEFT,moveSpeed + abs(hAccelerationCurrent));
					}
				}
			}			
		}
		//---------get rid of left momentum
		else {
			//---------check in bounds
			if(x - abs(hAccelerationCurrent) > 0) {
				x = x - abs(hAccelerationCurrent);				
			}
			else {
				x = 0;
			}
			if(x < abs(currentMap->getXOffset())) {
				if(currentMap != NULL) {
					//---------scroll opposite (right)
					if(currentMap->canScroll(RIGHT) == 1) {
						currentMap->scroll(RIGHT,abs(hAccelerationCurrent));
					}
				}
			}		
		}
		//---------accelerate
		if(hAccelerationCurrent < accelerationMax) {
				hAccelerationCurrent = hAccelerationCurrent + accelerationIncrement;
		}			
	}

	//---------Check keys for moving left, negative acceleration 
	if(!stateMovement[RIGHT] == 1 && stateMovement[LEFT] == 1) {
		//Where to insert idle state
		//---------Check to see if got rid of right momentum
		if(hAccelerationCurrent <= 0) {
			if(hAccelerationCurrent == 0) {
				currentMap->scroll(IDLE-1,0);
			}
			//---------check in bounds
			if(x - (moveSpeed + abs(hAccelerationCurrent)) > 0) {
				x = x - (moveSpeed + abs(hAccelerationCurrent));				
			}
			else {
				x = 0;
			}
			if(x < abs(currentMap->getXOffset())) {				
				if(currentMap != NULL) {
					//---------scroll opposite (right)
					if(currentMap->canScroll(RIGHT) == 1) {
						currentMap->scroll(RIGHT,moveSpeed + abs(hAccelerationCurrent));
					}
				}
			}
		}
		//---------get rid of right momentum
		else {
			//---------check in bounds
			if(x + (abs(hAccelerationCurrent)) < currentMap->getWidth()){
				x = x + (abs(hAccelerationCurrent));				
			}
			else {
				x = currentMap->getWidth();
			}
			if(x > abs(currentMap->getXOffset())+2560) {				
				if(currentMap != NULL) {
					//---------scroll opposite (left)
					if(currentMap->canScroll(LEFT) == 1) {
						currentMap->scroll(LEFT,abs(hAccelerationCurrent));
					}
				}
			}			
		}
		//---------accelerate
		if(hAccelerationCurrent > -accelerationMax) {
				hAccelerationCurrent = hAccelerationCurrent - accelerationIncrement;
		}			
	}

	//---------Check keys for moving up, positive acceleration
	if(!stateMovement[DOWN] == 1  && stateMovement[UP] == 1) {
		//---------check to see if got rid of down momentum
		if(vAccelerationCurrent >= 0) {
			if(vAccelerationCurrent == 0) {
				currentMap->scroll(IDLE,0);
			}
			//---------check in bounds
			if(y - (moveSpeed + abs(vAccelerationCurrent)) > 0) {
				y = y - (moveSpeed + abs(vAccelerationCurrent));				
			}
			else {
				y = 0;
			}
			if(y < abs(currentMap->getYOffset())) {				
				if(currentMap != NULL) {
					//---------scroll opposite (down)
					if(currentMap->canScroll(DOWN) == 1) {
						currentMap->scroll(DOWN,moveSpeed + abs(vAccelerationCurrent));
					}
				}
			}			
		}
		//---------get rid of down momentum
		else {
			//---------check in bounds
			if(y + abs(vAccelerationCurrent) < currentMap->getHeight()) {
				y = y + abs(vAccelerationCurrent);
			}
			else {
				y = currentMap->getHeight();
			}
			if(y > abs(currentMap->getYOffset())+1920) {
				if(currentMap != NULL) { 
					//---------scroll opposite (up)
					if(currentMap->canScroll(UP) == 1) {
						currentMap->scroll(UP,abs(vAccelerationCurrent));
					}
				}
			}				
		}
		//---------accelerate
		if(vAccelerationCurrent < accelerationMax) {
			vAccelerationCurrent = vAccelerationCurrent + accelerationIncrement;
		}
	}

	//---------Check keys for moving down, negative acceleration
	if(!stateMovement[UP] == 1 && stateMovement[DOWN] == 1) {
		//---------check to see if got rid of up momentum
		if(vAccelerationCurrent <= 0) {
			if(vAccelerationCurrent == 0) {
				currentMap->scroll(IDLE,0);
			}
			//---------check in bounds
			if(y + (moveSpeed + abs(vAccelerationCurrent)) < currentMap->getHeight()) {
				y = y + (moveSpeed + abs(vAccelerationCurrent));
			}
			else {
				y = currentMap->getHeight();
			}
			if(y > abs(currentMap->getYOffset())+1920) {				
				if(currentMap != NULL) {
					//---------scroll opposite (up)
					if(currentMap->canScroll(UP) == 1) {
						currentMap->scroll(UP,moveSpeed + abs(vAccelerationCurrent));
					}
				}
			}					
		}
		//---------get rid of up momentum
		else {
			//---------check in bounds
			if(y - abs(vAccelerationCurrent) > 0) {
				y = y - abs(vAccelerationCurrent);
			}
			else {
				y = 0;
			}
			if(y < abs(currentMap->getYOffset())) {				
				if(currentMap != NULL) {
					//---------scroll opposite (down)
					if(currentMap->canScroll(DOWN) == 1) {
					currentMap->scroll(DOWN, abs(vAccelerationCurrent));
					}
				}
			}		
		}
		//---------accelerate
		if(vAccelerationCurrent > -accelerationMax) {
			vAccelerationCurrent = vAccelerationCurrent - accelerationIncrement;
		}
	}

	//---------Check no up/down keys, or both keys, keep momentum in direction, bring acceleration to 0
	if((!stateMovement[UP] == 1 && !stateMovement[DOWN] == 1) || (stateMovement[UP] == 1 && stateMovement[DOWN] == 1)) {
		//---------upwards momentum
		if(vAccelerationCurrent > 0) {
			//---------scroll down
			if(y - abs(vAccelerationCurrent) > 0) {
				y = y - abs(vAccelerationCurrent);
			}
			else {
				y = 0;
			}
			if(y < abs(currentMap->getYOffset())) {
				if(currentMap != NULL) {
					if(currentMap->canScroll(DOWN) == 1) {
						currentMap->scroll(DOWN,vAccelerationCurrent);
					}
				}
			}
			vAccelerationCurrent = vAccelerationCurrent - accelerationIncrement;			
		}
		//---------downwards momentum
		if(vAccelerationCurrent < 0) {
			//---------scroll up
			if(y + abs(vAccelerationCurrent) < currentMap->getHeight()) {
				y = y + abs(vAccelerationCurrent);
			}
			else {
				y = currentMap->getHeight();
			}
			if(y > abs(currentMap->getYOffset())+1920) {
				if(currentMap != NULL) {
					if(currentMap->canScroll(UP) == 1) {
						currentMap->scroll(UP,abs(vAccelerationCurrent));
					}
				}
			}
			vAccelerationCurrent = vAccelerationCurrent + accelerationIncrement;
		}	
		//---------no momentum 
		if(vAccelerationCurrent == 0) {
			currentMap->scroll(IDLE,0);
		}
	}

	//---------Check no left/right keys, or both keys, keep momentum in direction, bring acceleration to 0
	if((!stateMovement[LEFT] == 1 && !stateMovement[RIGHT] == 1) || (stateMovement[LEFT] == 1 && stateMovement[RIGHT] == 1)) {
		//---------right momentum
		if(hAccelerationCurrent > 0) {
			//---------scroll left
			if(x + abs(hAccelerationCurrent) < currentMap->getWidth()) {
				x = x + abs(hAccelerationCurrent);
			}
			else {
				x = currentMap->getWidth();
			}
			if(x > abs(currentMap->getXOffset())+2560) {
				if(currentMap != NULL) {
					if(currentMap->canScroll(LEFT) == 1) {
						currentMap->scroll(LEFT,hAccelerationCurrent);
					}
				}
			}
			hAccelerationCurrent = hAccelerationCurrent - accelerationIncrement;
		}
		//---------left momentum
		if(hAccelerationCurrent < 0) {
			//---------scroll left
			if(x - abs(hAccelerationCurrent) > 0) {
				x = x - abs(hAccelerationCurrent);
			}
			else {
				x = 0;
			}
			if(x < abs(currentMap->getXOffset())) {
				if(currentMap != NULL) {
					if(currentMap->canScroll(RIGHT) == 1) {
						currentMap->scroll(RIGHT,abs(hAccelerationCurrent));
					}
				}
			}
			hAccelerationCurrent = hAccelerationCurrent + accelerationIncrement;
		}
		//---------no momentum 
		if(hAccelerationCurrent == 0) {
			currentMap->scroll(IDLE-1,0);
		}
	}	
	//---------rotate to mouse
	float angle = atan2f((lookToX - (x+currentMap->getXOffset())), (lookToY - (y+currentMap->getYOffset())));
	//---------convert angle towards mouseX,mouseY from rad to degrees
	angle = ((angle/PI)*180)+270;
	//---------finish converting by taking down > 360 (added 270 to match other angle mode)
	if(angle >= 360) angle = angle - 360;
	//---------check stateRotation tolerance
	if(abs(angle - direction) > directionTolerance) {
		//---------see which direction to turn(+/-)
		float crossProduct = (cos(direction/180*PI) * sin(angle/180*PI)) - (cos(angle/180*PI)*sin(direction/180*PI));
		if(crossProduct >= 0) {
			if(abs(angle - direction) > 3*directionTolerance) {			
				stateRotation[LEFT] = 1;
				stateRotation[RIGHT] = 0;
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
				stateRotation[RIGHT] = 1;
				stateRotation[LEFT] = 0;
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
		stateRotation[LEFT] = 0;
		stateRotation[RIGHT] = 0;
	}
	//---------
}

//==============================================================================
void EntityPlayer::onRender(float interpolation) {	
	//---------Reset
	//glLoadIdentity();
	//---------Move World Coordinates to Player Location
	rX = x;
	rY = y;
	rDir = direction;
	//---------Movement Interpolation
	if(hadCollision == false) {
		if((x > 0) && (x < currentMap->getWidth())) {		
			if(hAccelerationCurrent < 0) {
				if(stateMovement[LEFT]) {
					rX = x - ((moveSpeed + abs(hAccelerationCurrent)) * interpolation);
				}
				else {
					rX = x - ((abs(hAccelerationCurrent)) * interpolation);
				}
			}
			if(hAccelerationCurrent > 0) {
				if(stateMovement[RIGHT]) {
					rX = x + ((moveSpeed + abs(hAccelerationCurrent)) * interpolation);
				}
				else {
					rX = x + ((abs(hAccelerationCurrent)) * interpolation);
				}
			}
		}
		if((y > 0) && (y < currentMap->getHeight())) {
			if(vAccelerationCurrent < 0) {
				if(stateMovement[DOWN]) {
					rY = y + ((moveSpeed + abs(vAccelerationCurrent)) * interpolation);
				}
				else {
					rY = y + ((abs(vAccelerationCurrent)) * interpolation);
				}
			}
			if(vAccelerationCurrent > 0) {
				if(stateMovement[UP]) {
					rY = y - ((moveSpeed + abs(vAccelerationCurrent)) * interpolation); 
				}
				else{
					rY = y - ((abs(vAccelerationCurrent)) * interpolation); 
				}
			}
		}
		if(rX < 0) {rX = 0;}
		if(rX > currentMap->getWidth()) {rX = currentMap->getWidth();}
		if(rY < 0) {rY = 0;}
		if(rY > currentMap->getHeight()){rY = currentMap->getHeight();}

		//---------stateRotation Interpolation
		if(stateRotation[LEFT]){
			rDir = direction + turnSpeed*interpolation;
		}
		if(stateRotation[RIGHT]){
			rDir = direction - turnSpeed*interpolation;
		}
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
	if(sym == SDLK_a) stateMovement[LEFT] = 1;
	if(sym == SDLK_d) stateMovement[RIGHT] = 1;
	if(sym == SDLK_w) stateMovement[UP] = 1;
	if(sym == SDLK_s) stateMovement[DOWN] = 1;
}

//==============================================================================
void EntityPlayer::onKeyRelease(SDL_Keycode sym) {
	//---------Set stateMovement for key releases
	if(sym == SDLK_a) stateMovement[LEFT] = 0;
	if(sym == SDLK_d) stateMovement[RIGHT] = 0;
	if(sym == SDLK_w) stateMovement[UP] = 0;
	if(sym == SDLK_s) stateMovement[DOWN] = 0;
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

//==============================================================================
void EntityPlayer::setBounds(float x1Bound, float x2Bound, float y1Bound, float y2Bound) {
	this->x1Bound = x1Bound; 
	this->x2Bound = x2Bound; 
	this->y1Bound = y1Bound; 
	this->y2Bound = y2Bound; 
}

//==============================================================================