#ifndef _CANIMATION_H_
	#define _CANIMATION_H_
	#include <sdl/SDL.h>
	#include <vector>
	
	class CAnimation {
		private:
			int surfSprite;
			int currentFrame;
			int frameInc;

		private:
			int frameRate;
			int oldTime;

		public:
			int maxFrames;
			bool oscillate;

		public:
			CAnimation();
			void onAnimate();

		public:
			void setSprite(int surfSprite);
			int getSprite();
			void setFrameRate(int rate);
			void setCurrentFrame(int frame);
			int getCurrentFrame();
	};


#endif