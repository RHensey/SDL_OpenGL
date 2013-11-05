//==============================================================================
// CApp_Entity_Rectangle.cpp
//==============================================================================
#include "CApp.h"

//==============================================================================
EntityRectangle::EntityRectangle() {

}

//==============================================================================
void EntityRectangle::onInit() {
	//--Start Coords
	x = 500.0;
	y = 500.0;
	moveDir = 1;
	//---------Dimensions
	width = 256.0;
	height = 192.0;
	texWidth = 256.0;
	texHeight = 192.0;
	//---------Vertices
	numVertices = 4;
    vertices[0].x = -width/2;
	vertices[0].y = height/2;
    vertices[1].x = -width/2;
	vertices[1].y = -height/2;
	vertices[2].x = width/2;
	vertices[2].y = -height/2;
    vertices[3].x = width/2;
	vertices[3].y = height/2;
	//---------Tex File
	texFile[0] = "./Resource/EntityRectangle.RAW";
	//---------
	setBounds(5120.0,0.0,0.0,3840.0);
}

//==============================================================================
void EntityRectangle::onLoop() {
	stateMovement[VECTOR_RIGHT] = true;
	move();
}

//==============================================================================
void EntityRectangle::onRender(double interpolation) {
	//---------Texture
	glBindTexture(GL_TEXTURE_2D, texID[currentFrame]);
	//---------Translate To
	rX = x;
	rY = y;
	if(attributes[INTERPOLATE] == false) {
		rX = x + interpolation;
		rY = y + interpolation;
	}
	glTranslatef(rX,rY,0.0f);
	//---------Reset Texture Identity
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//---------Go to Model 
	glMatrixMode(GL_MODELVIEW); 
	glBegin(GL_POLYGON);/*
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(-width/2,-height/2,0.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(-width/2,height/2,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(width/2,height/2,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(width/2,-height/2,0.0f);
			*/
			for(int a = 0; a < numVertices; a++) {		
				glTexCoord2f(0.5+(vertices[a].x/width),0.5+(vertices[a].y/height));
				glVertex3f(vertices[a].x,vertices[a].y,0.0f);
			}	
	glEnd();
	//

	//---------Translate Back
	glTranslatef(-rX,-rY,0);
}

//==============================================================================
void EntityRectangle::onCleanup() {
	glDeleteTextures(numTexLoaded,texID);
}

//==============================================================================