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
	x = 500.0f;
	y = 500.0f;
	//---------Dimensions
	width = 256.0f;
	height = 192.0f;
	texWidth = 256.0f;
	texHeight = 192.0f;
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
}

//==============================================================================
void EntityRectangle::onLoop() {
	//---------Move for Fun
	x = x + 1.0f;
	y = y + 1.0f;
}

//==============================================================================
void EntityRectangle::onRender(float interpolation) {
	//---------Texture
	glBindTexture(GL_TEXTURE_2D, texID[currentFrame]);
	//---------Translate To
	rX = x;
	rY = y;
	if(hadCollision == false) {
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
				glTexCoord2f(0.5f+(vertices[a].x/width),0.5f+(vertices[a].y/height));
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