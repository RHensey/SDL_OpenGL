//==============================================================================
// CApp_Entity_Map.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
EntityMap::EntityMap(){	
}

//==============================================================================
void EntityMap::onInit() {
	//---------
	currentMapIndex = 0;
	//---------
	texWidth = 64;
	texHeight = 64;
	//---------
	rX = 0;
	rY = 0;
	//---------Set initial stateMovement to 0
	for (int a = 0; a < MAX_MOVEMENT_STATES; a++) {
		stateMovement[a] = 0;
	}
	//---------Set initial stateMovement to 0
	for (int a = 0; a < MAX_MOVEMENT_STATES; a++) {
		possibleScrollDir[a] = 0;
	}
	//---------Offset
	currentXOffset = 0.0f;
	currentYOffset = 0.0f;
	//---------Scrolling
	hSpeedMax = 10.0f;
	vSpeedMax = 10.0f;
	hSpeed = 0.0f;
	vSpeed = 0.0f;
	//---------Number of Frames *make sure it correlates otherwise all textures wont be loaded (used in CApp_OnInit)
	maxFrames = 3;
	//---------Tex File
	texFile[0] = "./Resource/EntityMapTest0.RAW";
	texFile[1] = "./Resource/EntityMapTest1.RAW";
	texFile[2] = "./Resource/EntityMapTest2.RAW";
}
//==============================================================================
void EntityMap::loadMapList() {
	//-File Name
	std::string fName = "./Resource/EntityMap_List.txt";
	//-Pointers
	FILE *file;
	//-Buffer String
	char buffer[128];	
	std::string lineString;
	BYTE i;
	map mapHolder;
	//-Open and Read
	const char * ccfName = fName.c_str();
	file = fopen(ccfName,"r");
	if(!(file == NULL)) {  //If it is null do something---- might cause problems later
		while(fgets(buffer,128,file)) {
			lineString = buffer;			
			if(lineString.find("MAP ID",0) != -1) i=0;
			if(lineString.find("WIDTH",0) != -1) i=1;
			if(lineString.find("HEIGHT",0) != -1) i=2;
			if(lineString.find("TILES",0) != -1) i=3;
			if(lineString.find("MAP END",0) != -1) i=4;
			switch (i) {
				case 0:					
					fgets(buffer,128,file);
					//IF ERROR CORRUPT
					mapHolder.mapIndex = atoi(buffer);
					break;
				case 1:
					fgets(buffer,128,file);
					//IF ERROR CORRUPT
					mapHolder.width = atoi(buffer);
					break;
				case 2:
					fgets(buffer,128,file);
					//IF ERROR CORRUPT
					mapHolder.height = atoi(buffer);
					break;
				case 3:
					for(int a = 0; a < mapHolder.width * mapHolder.height; a++) {
						fgets(buffer,128,file);
						//IF ERROR CORRUPT
						tile t;
						t.textureIndex = atoi(buffer);
						mapHolder.tileList.push_back(t);
					}
					break;
				case 4:
					mapList.push_back(mapHolder);
					break;
				default:
					break;
					//MAP END
			}
		}
		
		fclose(file);
	}
}

//==============================================================================
void EntityMap::onLoop(){		
}

//==============================================================================
void EntityMap::onRender(float interpolation) {
	//---------Reset
	glLoadIdentity();
	//---------Go to Location
	if(abs(hSpeed) > 0) {
		if(stateMovement[LEFT] == 1) {
			rX = currentXOffset - (abs(hSpeed) * interpolation);
			glTranslatef(rX,0.0f,0.0f);
		}
		if(stateMovement[RIGHT] == 1) {
			rX = currentXOffset + (abs(hSpeed) * interpolation);
			glTranslatef(rX,0.0f,0.0f);
		}
	}
	else {
		glTranslatef(currentXOffset,0,0);
	}
	if(abs(vSpeed) > 0) {
		if(stateMovement[UP] == 1) {
			rY = currentYOffset - (abs(vSpeed) * interpolation);
			glTranslatef(0.0f,rY,0.0f);
		}
		if(stateMovement[DOWN] == 1) {
			rY = currentYOffset + (abs(vSpeed) * interpolation);
			glTranslatef(0.0f,rY,0.0f);
		}
	}
	else {
		glTranslatef(0,currentYOffset,0);
	}		
	//---------Reset Texture Matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//---------Draw Model
	glMatrixMode(GL_MODELVIEW); 
	//---------Render grid
	//-Question = Will Map Index always be Vector Index?
	//-Think about synchronizing mapwidth and loaded mapwidth
	
	for(int a = 0; a < mapList.at(currentMapIndex).height; a++) {
		for(int b = 0; b < mapList.at(currentMapIndex).width; b++) {
			GLuint index = mapList.at(currentMapIndex).tileList[a+b].textureIndex;
			glBindTexture(GL_TEXTURE_2D, texID[index]);				
			glBegin(GL_QUADS);	
				glTexCoord2f(0.0f,1.0f);
				glVertex3f(b*texWidth,a*texHeight,0.0f);
				glTexCoord2f(0.0f,0.0f);
				glVertex3f(b*texWidth,a*texHeight+texHeight,0.0f);
				glTexCoord2f(1.0f,0.0f);
				glVertex3f(b*texWidth+texWidth,a*texHeight+texHeight,0.0f);
				glTexCoord2f(1.0f,1.0f);
				glVertex3f(b*texWidth+texWidth,a*texHeight,0.0f);	
			glEnd();
		}
	}	
	//---------
	/*
	glBindTexture(GL_TEXTURE_2D, texID[0]);	
	glBegin(GL_QUADS);		
	for(int a = 0; a < width; a = a + 64) {
		for(int b = 0; b < height; b = b + 64) {
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(a,b+64,0.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(a,b,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(a+64,b,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(a+64,b+64,0.0f);
		}
	}			
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texID[1]);	
	glBegin(GL_QUADS);		
	for(int a = 2560; a < width; a = a + 64) {
		for(int b = 1920; b < height; b = b + 64) {
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(a,b+64,0.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(a,b,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(a+64,b,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(a+64,b+64,0.0f);
		}
	}			
	glEnd();*/
	//---------
}

//==============================================================================
void EntityMap::onCleanup() {
	glDeleteTextures(numTexLoaded,texID);
}

//==============================================================================
void EntityMap::setDimensions(int width, int height) {
	//---------Set height and width
	this->width = width;
	this->height = height;
	//---------
	currentXOffset = 0;
	currentYOffset = 0;
	//---------
}

//==============================================================================
void EntityMap::scroll(int direction, float speed) {
	//---------idle
	if(direction == IDLE-1) {
		stateMovement[LEFT] = 0;
		stateMovement[RIGHT] = 0;
	    hSpeed = speed;
	}
	if(direction == IDLE) {
		stateMovement[UP] = 0;
		stateMovement[DOWN] = 0;
		vSpeed = speed;
	}
	//---------scroll left
	if(direction == LEFT) {
		stateMovement[LEFT] = 1;
		hSpeed = -speed;
		currentXOffset = currentXOffset + hSpeed;
	}
	//---------scroll right
	if(direction == RIGHT) {
		stateMovement[RIGHT] = 1;
		hSpeed = speed;
		currentXOffset = currentXOffset + hSpeed;
	}
	//---------scroll up
	if(direction == UP) {
		stateMovement[UP] = 1;
		vSpeed = -speed;
		currentYOffset = currentYOffset + vSpeed;
	}
	//---------scroll down
	if(direction == DOWN) {
		stateMovement[DOWN] = 1;
		vSpeed = speed;
		currentYOffset = currentYOffset + vSpeed;
	}
	

}

//==============================================================================
int EntityMap::canScroll(int direction) {
	if(direction == LEFT) {
		//---------Check LEFT
		if(currentXOffset <= -width/2 + hSpeedMax) {
			possibleScrollDir[LEFT] = 0;
			stateMovement[LEFT] = 0;
			if(hSpeed < 0) {
				hSpeed = 0;
				currentXOffset = -width/2;
			}
		}
		else {
			possibleScrollDir[LEFT] = 1;
		}
	}
	if(direction == RIGHT) {
		//---------Check RIGHT
		if(currentXOffset >= 0 - hSpeedMax){
			possibleScrollDir[RIGHT] = 0;
			stateMovement[RIGHT] = 0;
			if(hSpeed > 0) {
				hSpeed = 0;
				currentXOffset = 0;
			}
		}
		else {
			possibleScrollDir[RIGHT] = 1;
		}
	}
	if(direction == UP) {
		//---------Check UP
		if(currentYOffset <= -height/2 + vSpeedMax) {
			possibleScrollDir[UP] = 0;
			stateMovement[UP] = 0;
			if(vSpeed < 0) {
				vSpeed = 0;
				currentYOffset = -height/2;
			}
		}
		else {
			possibleScrollDir[UP] = 1;
		}
	}
	if(direction == DOWN) {
		//---------Check DOWN
		if(currentYOffset >= 0 - vSpeedMax) {
			possibleScrollDir[DOWN] = 0;
			stateMovement[DOWN] = 0;
			if(vSpeed > 0) {
				vSpeed = 0;
				currentYOffset = 0;
			}
		}
		else {
			possibleScrollDir[DOWN] = 1;
		}
	}

	return possibleScrollDir[direction];
}

//==============================================================================
float EntityMap::getXOffset() {
	return currentXOffset;
}

//==============================================================================
float EntityMap::getYOffset() {
	return currentYOffset;
}

//==============================================================================