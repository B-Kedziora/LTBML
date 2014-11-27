#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

class Button{
	public:
		Button(int x, int y, int width, int height, SDL_Renderer* renderer);

		bool checkMouseHover(int mouseX, int mouseY);

		int getX(){
			return x_;
		}

		int getY(){
			return y_;
		}

	private:
		/**
		*	Called by checkMouseHover
		*/
		void highlightButton();

		int x_;
		int y_;
		int endX_;
		int endY_;
		SDL_Renderer* renderer_;
};


#endif // DESTRUCTIONSPRITE_H