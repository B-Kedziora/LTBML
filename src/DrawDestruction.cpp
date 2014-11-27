#include "DestructionSprite.h"

SDL_Texture* DestructionSprite::ghostDestructionTexture_=nullptr;

DestructionSprite::DestructionSprite(field place, SDL_Renderer* renderer)
{
    if(ghostDestructionTexture_ == nullptr)
    {
        SDL_Surface* temp = WrapperSDL::loadSingleImage("GhostExiled");
        ghostDestructionTexture_ = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    place_ = place;
    leftTime_ = 30;
}

/**
*   Draws enemy death animation
*   @return returns true if effect is finished
*/
bool DestructionSprite::drawEffect( long frames, SDL_Renderer* renderer )
{
    if(leftTime_)
    {
        if(leftTime_>20)
        {
            WrapperSDL::putImage(place_.fieldX, place_.fieldY, 0,0, ghostDestructionTexture_, renderer);
        }
        else if(leftTime_>10)
        {
            WrapperSDL::putImage(place_.fieldX, place_.fieldY, 80,0, ghostDestructionTexture_, renderer);
        }
        else if (leftTime_>0)
        {
            WrapperSDL::putImage(place_.fieldX, place_.fieldY, 160, 0, ghostDestructionTexture_, renderer);
        }
        leftTime_--;
        return false;
    }
    return true;
}
