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
	/*for (int a = 0; a < MAX_MOVEMENT_STATES; a++) {
		stateMovement[a] = 0;
	}
	//---------Set initial stateMovement to 0
	for (int a = 0; a < MAX_MOVEMENT_STATES; a++) {
		possibleScrollDir[a] = 0;
	}*/
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
void EntityMap::onRender(double interpolation) {
	//---------Reset
	glLoadIdentity();
	//---------Go to Location
	/*
	if(abs(hSpeed) > 0) {
		if(stateMovement[MOVE_LEFT] == 1) {
			rX = currentXOffset - (abs(hSpeed) * interpolation);
			glTranslatef(rX,0.0f,0.0f);
		}
		if(stateMovement[MOVE_RIGHT] == 1) {
			rX = currentXOffset + (abs(hSpeed) * interpolation);
			glTranslatef(rX,0.0f,0.0f);
		}
	}
	else {
		glTranslatef(currentXOffset,0,0);
	}
	if(abs(vSpeed) > 0) {
		if(stateMovement[MOVE_UP] == 1) {
			rY = currentYOffset - (abs(vSpeed) * interpolation);
			glTranslatef(0.0f,rY,0.0f);
		}
		if(stateMovement[MOVE_DOWN] == 1) {
			rY = currentYOffset + (abs(vSpeed) * interpolation);
			glTranslatef(0.0f,rY,0.0f);
		}
	}
	else {
		glTranslatef(0,currentYOffset,0);
	}		*/
	glTranslatef(currentXOffset,currentYOffset,0.0);
	//---------Reset Texture Matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//---------Draw Model
	glMatrixMode(GL_MODELVIEW); 
	//---------Render grid
	//-Question = Will Map Index always be Vector Index?
	//-Think about synchronizing mapwidth and loaded mapwidth
	
	for(double a = 0; a < mapList.at(currentMapIndex).height; a++) {
		for(double b = 0; b < mapList.at(currentMapIndex).width; b++) {
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
void EntityMap::scroll(int direction, double speed) {/*
	//---------idle
	if(direction == MOVE_IDLE-1) {
		stateMovement[MOVE_LEFT] = 0;
		stateMovement[MOVE_RIGHT] = 0;
	    hSpeed = speed;
	}
	if(direction == MOVE_IDLE) {
		stateMovement[MOVE_UP] = 0;
		stateMovement[MOVE_DOWN] = 0;
		vSpeed = speed;
	}
	//---------scroll left
	if(direction == MOVE_LEFT) {
		stateMovement[MOVE_LEFT] = 1;
		hSpeed = -speed;
		currentXOffset = currentXOffset + hSpeed;
	}
	//---------scroll right
	if(direction == MOVE_RIGHT) {
		stateMovement[MOVE_RIGHT] = 1;
		hSpeed = speed;
		currentXOffset = currentXOffset + hSpeed;
	}
	//---------scroll up
	if(direction == MOVE_UP) {
		stateMovement[MOVE_UP] = 1;
		vSpeed = -speed;
		currentYOffset = currentYOffset + vSpeed;
	}
	//---------scroll down
	if(direction == MOVE_DOWN) {
		stateMovement[MOVE_DOWN] = 1;
		vSpeed = speed;
		currentYOffset = currentYOffset + vSpeed;
	}
	*/

}

//==============================================================================
int EntityMap::canScroll(int direction) {/*
	if(direction == MOVE_LEFT) {
		//---------Check MOVE_LEFT
		if(currentXOffset <= -width/2 + hSpeedMax) {
			possibleScrollDir[MOVE_LEFT] = 0;
			stateMovement[MOVE_LEFT] = 0;
			if(hSpeed < 0) {
				hSpeed = 0;
				currentXOffset = -width/2;
			}
		}
		else {
			possibleScrollDir[MOVE_LEFT] = 1;
		}
	}
	if(direction == MOVE_RIGHT) {
		//---------Check MOVE_RIGHT
		if(currentXOffset >= 0 - hSpeedMax){
			possibleScrollDir[MOVE_RIGHT] = 0;
			stateMovement[MOVE_RIGHT] = 0;
			if(hSpeed > 0) {
				hSpeed = 0;
				currentXOffset = 0;
			}
		}
		else {
			possibleScrollDir[MOVE_RIGHT] = 1;
		}
	}
	if(direction == MOVE_UP) {
		//---------Check MOVE_UP
		if(currentYOffset <= -height/2 + vSpeedMax) {
			possibleScrollDir[MOVE_UP] = 0;
			stateMovement[MOVE_UP] = 0;
			if(vSpeed < 0) {
				vSpeed = 0;
				currentYOffset = -height/2;
			}
		}
		else {
			possibleScrollDir[MOVE_UP] = 1;
		}
	}
	if(direction == MOVE_DOWN) {
		//---------Check MOVE_DOWN
		if(currentYOffset >= 0 - vSpeedMax) {
			possibleScrollDir[MOVE_DOWN] = 0;
			stateMovement[MOVE_DOWN] = 0;
			if(vSpeed > 0) {
				vSpeed = 0;
				currentYOffset = 0;
			}
		}
		else {
			possibleScrollDir[MOVE_DOWN] = 1;
		}
	}
	
	return possibleScrollDir[direction];*/
	return 1;
}

//==============================================================================
double EntityMap::getXOffset() {
	return currentXOffset;
}

//==============================================================================
double EntityMap::getYOffset() {
	return currentYOffset;
}

//==============================================================================