#include "Hud.h"

void Hud::draw(int lampLeft, int portableLampLeft, int cooldown, int ghostsKilled){
	//Timer
    textHolder_ << (SDL_GetTicks() - gameTimer)/1000<<"s";
    WrapperSDL::applyText(1175, 6, textHolder_.str(), renderer, 0);
    textHolder_.str("");

    //Lamps
    textHolder_ << lampLeft;
    if(textHolder_.str()=="0")
    {
        WrapperSDL::applyText(1175, 50, "Drzwi", renderer, 1);
        WrapperSDL::applyText(1175, 70, "Otwarte", renderer, 1);
    }
    else
    {
        WrapperSDL::applyText(1175, 54, textHolder_.str(), renderer, 0);
    }
    textHolder_.str("");

    //Portable lamps
    textHolder_<<portableLampLeft;
    WrapperSDL::applyText(1242, 113, textHolder_.str(), renderer, 0);
    textHolder_.str("");

    //Flashlight cooldown
    textHolder_<<cooldown;
    WrapperSDL::applyText(1152, 105, textHolder_.str(), renderer, 1);
    WrapperSDL::applyText(1152, 125, "ms", renderer, 1);
    textHolder_.str("");

    //Number of killed ghosts
    textHolder_<<ghostsKilled;
    WrapperSDL::applyText(1242, 169, textHolder_.str(), renderer, 0);
}