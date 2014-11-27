#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <iostream>
#include <WrapperSDL.h>

class Hud{
	public:
		Hud(SDL_Renderer* renderer){
			renderer_ = renderer;
		}

		void draw(int lampLeft, int portableLampLeft, int cooldown, int ghostsKilled);
	private:
		SDL_Renderer* renderer_;
		ostringstream textHolder_;
};

#endif // HUD_H