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
	glTranslatef(x,y,0.0);
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
}

//==============================================================================
double EntityMap::scroll(int direction, double distance) {	
	if(direction == VECTOR_RIGHT) {
		if(x+distance < 0) {
			displacements[LEFT_RIGHT] = distance;
			x+=distance;
		} else {
			displacements[LEFT_RIGHT] = (0 - x);
			x=0;				
		}
		return -displacements[LEFT_RIGHT];
	} else if (direction == VECTOR_UP) {
		if(y-distance > -height/2) {
			displacements[UP_DOWN] = -distance;
			y-=distance;
		} else {
			displacements[UP_DOWN] = (-height/2 - y);
			y=-height/2;				
		}
		return -displacements[UP_DOWN];
	} else if (direction == VECTOR_LEFT) {
		if(x-distance > -width/2) {
			displacements[LEFT_RIGHT] = -distance;
			x-=distance;
		} else {
			displacements[LEFT_RIGHT] = (-width/2 - x);
			x=-width/2;				
		}
		return -displacements[LEFT_RIGHT];
	} else {
		if(y+distance < 0) {
			displacements[UP_DOWN] = distance;
			y+=distance;
		} else {
			displacements[UP_DOWN] = (0 - y);
			y=0;				
		}
		return -displacements[UP_DOWN];
	}
}

//==============================================================================
bool EntityMap::canScroll(int direction, double distance) {
	if(direction == VECTOR_RIGHT) {
		return x < 0;
	} else if (direction == VECTOR_UP) {
		return y > -height/2;
	} else if (direction == VECTOR_LEFT) {		
		return x > -width/2;
	} else {
		return y < 0;		
	}
}
