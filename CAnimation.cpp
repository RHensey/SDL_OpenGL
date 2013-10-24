#include "CAnimation.h"

CAnimation::CAnimation() {
	currentFrame = 0;
	maxFrames = 0;
	frameInc = 1;
	frameRate = 100;
	oldTime = 0;
	oscillate = false;
}

void CAnimation::setSprite(int surfSprite) {
	this->surfSprite = surfSprite;
}
int CAnimation::getSprite() {
	return surfSprite;
}

void CAnimation::onAnimate() {
	if(oldTime + frameRate > SDL_GetTicks()) {
		return;
	}
	oldTime = SDL_GetTicks();
	currentFrame += frameInc;

	if(oscillate) {
		if(frameInc > 0) {
			if(currentFrame >= maxFrames-1) {
				frameInc = -frameInc;
			}
		}
		else {
			if(currentFrame <= 0) {
				frameInc = -frameInc;
			}
		}
	}
	else {
		if(currentFrame >= maxFrames - 1) {
			currentFrame = 0;
		}
	}
}

void CAnimation::setFrameRate(int rate) {
	frameRate = rate;
}

void CAnimation::setCurrentFrame(int frame) {
	if(frame < 0 || frame >= maxFrames) return;
	currentFrame = frame;
}

int CAnimation::getCurrentFrame() {
	return currentFrame;
}


