#include "Hero.h"

Hero::Hero(int startX, int startY, SDL_Renderer* renderer) : Movable (startX, startY, "HeroAlpha", renderer)
{
    flashlight = new Flashlight(renderer);
}

Hero::~Hero()
{
    delete(flashlight);
    portableLampsOnMap.clear();
}

void Hero::go(bool** accessible)
{
    if(checkCanMove(moveFlag, accessible))
    {
        updatePosition(moveFlag, 1.00);
    }
}

void Hero::draw(SDL_Renderer* renderer, long& globalFrames, field mousePosition)
{
    Movable::draw(renderer, globalFrames);
    flashlight->draw(getPixelLocalization(), mousePosition, renderer);
}

void Hero::putPortableLamp(SDL_Renderer* renderer)
{
    PortableLamp* temp = PortableLamp::putLamp(getField(), renderer);
    if(temp)
        portableLampsOnMap.push_back(temp);
}
