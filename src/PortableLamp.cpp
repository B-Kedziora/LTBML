#include "PortableLamp.h"

signed short PortableLamp::lampsLeft_ = 5;
signed long PortableLamp::lastPut_ = 0;
SDL_Texture* PortableLamp::portableLampTexture_ = nullptr;

PortableLamp* PortableLamp::putLamp(field loc, SDL_Renderer* renderer)
{
    if(portableLampTexture_ == nullptr)
    {
        SDL_Surface* temp = WrapperSDL::loadSingleImage("portableLamp");
        portableLampTexture_ = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    if( (lastPut_ + COOLDOWN < SDL_GetTicks()) && lampsLeft_ > 0)
    {
        lastPut_= SDL_GetTicks();
        lampsLeft_--;
        return new PortableLamp(loc);
    }
    else
    {
        return nullptr;
    }
}

void PortableLamp::draw(SDL_Renderer* renderer)
{
    WrapperSDL::putImage(localiztion_.fieldX*80, localiztion_.fieldY*80, portableLampTexture_, renderer);
}


PortableLamp::PortableLamp(field loc)
{
    localiztion_=loc;
}

PortableLamp::~PortableLamp()
{
    //dtor
}
