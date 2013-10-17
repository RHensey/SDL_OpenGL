//==============================================================================
// CEntity.cpp
//==============================================================================
#include "CEntity.h"

//==============================================================================
//---------How to access all entities
std::vector<CEntity*> CEntity::entityList;

CEntity::CEntity() {
	x = 1280.0f;
	y = 960.0f;
	texWidth = texHeight = width = height = 0;
	direction = 90;
	//---------
	hadCollision = false;
	for (int a = 0; a <  5; a++) {
		recentCollisions[a] = false;
	}
	//---------
	for (int a = 0; a < MAX_ATTRIBUTES; a++) {
		attributes[a] = false;
	}
	//---------
	rX = 0;
	rY = 0;
	rDir = 0;
	//---------
	entityList.push_back(this);
	//---------Initialize Arrays
	for (int a = 0; a < 255; a++) {
		texID[a] = 0;
		texFile[a] = "";
	}	
	numVertices = 0;
	numTexLoaded = 0;
	//---------Animation
	currentFrame = 0;
	maxFrames = 1;
}

//==============================================================================
CEntity::~CEntity() {
	//---------Virtual
}

//==============================================================================
void CEntity::onInit() {
	//---------Virtual
}

//==============================================================================
void CEntity::loadTexture() {
	//pass an array of filenames
	if(!(texFile[numTexLoaded] == "")) {
		//-Pointers
		BYTE *data;
		FILE *file;
		//-Allocate Buffer (RAW file)
		data = (BYTE*) malloc(texWidth*texHeight*3);	
		//-Open and Read
		const char * ccfName = texFile[numTexLoaded].c_str();
		file = fopen(ccfName,"rb");
		if(!(file == NULL)) {  //If it is null do something---- might cause problems later
			fread(data, texWidth * texHeight * 3, 1, file);
			fclose(file);
			//---------
			glGenTextures(1, &texID[numTexLoaded]);
			//---------Texture
			glBindTexture(GL_TEXTURE_2D, texID[numTexLoaded]);
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
			//---------Set to MIP map
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//---------Wrap Texture
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			//---------Create MIP map
			gluBuild2DMipmaps(GL_TEXTURE_2D,3,texWidth,texHeight,GL_RGB,GL_UNSIGNED_BYTE,data);
			free(data);
		}
		numTexLoaded++;
	}
}
//==============================================================================
float CEntity::getRadius() {
	float radius = sqrt((float)(width*width + height*height));	
	return radius;
}

//==============================================================================
float CEntity::getWidth() {
	return width;
}

//==============================================================================
float CEntity::getHeight() {
	return height;
}

//==============================================================================
void CEntity::onRender(float interpolation) {
	//---------Virtual
}

//==============================================================================
void CEntity::onLoop() {
	//---------Virtual
}

//==============================================================================
void CEntity::onCleanup() {
	//---------Virtual
}

//==============================================================================