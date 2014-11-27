#include "Button.h"

Button::Button(int x, int y, int width, int height, SDL_Renderer* renderer){
	x_ = x;
	y_ = y;
	endX_ = width + x;
	endY_ = height + y;
	renderer_ = renderer
} 

bool Button::checkMouseHover(int mouseX, int mouseY){
	if( (mouseX > x_) && (mouseX < endX_) &&
        (mouseY > y_) && (mouseY < endY_) ){
			highlightbutton();
			return true;
	}
	return false;
}

void Button::highlightbutton(){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 200 );
    SDL_Rect highlight;
    highlight.w = endX_ - x_;
    highlight.h = endY_ - y_;
    highlight.x = x_;
    highlight.y = y_;
    SDL_RenderDrawRect( renderer, &highlight );
}
