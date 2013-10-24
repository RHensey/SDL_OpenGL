//==============================================================================
// CEntity.cpp
//==============================================================================
#include "CEntity.h"

//==============================================================================
//---------How to access all entities
std::vector<CEntity*> CEntity::entityList;

CEntity::CEntity() {
	x = 0.0;
	y = 0.0;
	texWidth = texHeight = width = height = 0;
	direction = 90;
	//---------
	for (int a = 0; a < MAX_ATTRIBUTES; a++) {
		attributes[a] = false;
	}
	std::fill_n(stateMovement,MAX_VECTOR,0);
	std::fill_n(bounds,MAX_VECTOR,0);
	std::fill_n(velocities,MAX_VECTOR,0);
	std::fill_n(accelerations,MAX_VECTOR,0);
	//---------
	accelerationIncrement = 1.0;
	maxAcceleration = 1.0;
	maxSpeed = 1.0;
	turnSpeed = 1.0;
	//---------
	rX = 0;
	rY = 0;
	rDir = 0;
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
	twoLoops = 0;
	//---------
	entityList.push_back(this);
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
double CEntity::getRadius() {
	double radius = sqrt((double)(width*width + height*height));	
	return radius;
}

//==============================================================================
double CEntity::getWidth() {
	return width;
}

//==============================================================================
double CEntity::getHeight() {
	return height;
}

//==============================================================================
double CEntity::getX() {
	return x;
}

//==============================================================================
double CEntity::getY() {
	return y;
}

//==============================================================================
void CEntity::setX(double x) {
	this->x = x;
}

//==============================================================================
void CEntity::setY(double y) {
	this->y = y;
}

//==============================================================================
void CEntity::setWidth(double width) {
	this->width = width;
}

//==============================================================================
void CEntity::setHeight(double height) {
	this->height = height;
}

//==============================================================================
void CEntity::move() {
	double ratio = 1;
	//Handle Accelerations
	if(stateMovement[VECTOR_RIGHT]) {
		if(accelerations[VECTOR_RIGHT] < maxAcceleration) {
			accelerations[VECTOR_RIGHT] += (accelerationIncrement*ratio);
			if(accelerations[VECTOR_RIGHT] > maxAcceleration) {
				accelerations[VECTOR_RIGHT] = maxAcceleration;
			}
		}
	} else {
		if(accelerations[VECTOR_RIGHT] > 0) {
			accelerations[VECTOR_RIGHT] -= (accelerationIncrement*ratio/4);
			if(accelerations[VECTOR_RIGHT] < 0) {
				accelerations[VECTOR_RIGHT] = 0;
			}
		}		
	}
	if(stateMovement[VECTOR_UP]) {
		if(accelerations[VECTOR_UP] < maxAcceleration) {
			accelerations[VECTOR_UP] += (accelerationIncrement*ratio);
			if(accelerations[VECTOR_UP] > maxAcceleration) {
				accelerations[VECTOR_UP] = maxAcceleration;
			}
		} 
	} else {
		if(accelerations[VECTOR_UP] > 0) {
			accelerations[VECTOR_UP] -= (accelerationIncrement*ratio/4);
			if(accelerations[VECTOR_UP] < 0) {
				accelerations[VECTOR_UP] = 0;
			}
		}
	}
	
	if(stateMovement[VECTOR_LEFT]) {
		if(accelerations[VECTOR_LEFT] < maxAcceleration) {
			accelerations[VECTOR_LEFT] += (accelerationIncrement*ratio);
			if(accelerations[VECTOR_LEFT] > maxAcceleration) {
				accelerations[VECTOR_LEFT] = maxAcceleration;
			}
		} 
	} else {
		if(accelerations[VECTOR_LEFT] > 0) {
			accelerations[VECTOR_LEFT] -= (accelerationIncrement*ratio/4);
			if(accelerations[VECTOR_LEFT] < 0) {
				accelerations[VECTOR_LEFT] = 0;
			}
		}
	}
	
	if(stateMovement[VECTOR_DOWN]) {
		if(accelerations[VECTOR_DOWN] < maxAcceleration) {
			accelerations[VECTOR_DOWN] += (accelerationIncrement*ratio);
			if(accelerations[VECTOR_DOWN] > maxAcceleration) {
				accelerations[VECTOR_DOWN] = maxAcceleration;
			}
		} 
	} else {
		if(accelerations[VECTOR_DOWN] > 0) {
			accelerations[VECTOR_DOWN] -= (accelerationIncrement*ratio/4);
			if(accelerations[VECTOR_DOWN] < 0) {
				accelerations[VECTOR_DOWN] = 0;
			}
		}
	}
	
	//Change velocities
	if(velocities[VECTOR_RIGHT] <= maxSpeed) {
			velocities[VECTOR_RIGHT] = maxSpeed*pow(accelerations[VECTOR_RIGHT],3);
			if(velocities[VECTOR_RIGHT] > maxSpeed) {
				velocities[VECTOR_RIGHT] = maxSpeed;
			}
	}
	if(velocities[VECTOR_UP] <= maxSpeed) {
			velocities[VECTOR_UP] = maxSpeed*pow(accelerations[VECTOR_UP],3);
			if(velocities[VECTOR_UP] > maxSpeed) {
				velocities[VECTOR_UP] = maxSpeed;
			}
	}
	if(velocities[VECTOR_LEFT] <= maxSpeed) {
			velocities[VECTOR_LEFT] = maxSpeed*pow(accelerations[VECTOR_LEFT],3);
			if(velocities[VECTOR_LEFT] > maxSpeed) {
				velocities[VECTOR_LEFT] = maxSpeed;
			}
	}
	if(velocities[VECTOR_DOWN] <= maxSpeed) {
			velocities[VECTOR_DOWN] = maxSpeed*pow(accelerations[VECTOR_DOWN],3);
			if(velocities[VECTOR_DOWN] > maxSpeed) {
				velocities[VECTOR_DOWN] = maxSpeed;
			}
	}
	//Bounds Checking
	x += velocities[VECTOR_RIGHT] - velocities[VECTOR_LEFT];
	y += velocities[VECTOR_DOWN] - velocities[VECTOR_UP];	
}

//==============================================================================
CEntity* CEntity::getEntityAt(int a) {
	return CEntity::entityList.at(a);
}

//==============================================================================
int CEntity::getNumEntities() {
	return CEntity::entityList.size();
}

//==============================================================================
void CEntity::onRender(double interpolation) {
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