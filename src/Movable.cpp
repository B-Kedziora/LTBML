#include "Movable.h"

Movable::Movable(int startX, int startY, string Name, SDL_Renderer* renderer)
{
    drawInfo.x=0;
    drawInfo.y=0;
    drawInfo.w=80;
    drawInfo.h=80;
    drawDestination.x=0;
    drawDestination.y=0;
    drawDestination.w=80;
    drawDestination.h=80;
    localeX = startX;
    localeY = startY;

    //Loading graphics
    SDL_Surface* temp = WrapperSDL::loadSingleImage(Name);
    imageTexture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);

    //Determine number of frames in animation
    int w;
    SDL_QueryTexture(imageTexture, NULL, NULL, &w, NULL);
    animationFrames = w/80;
    currentFrame = 0;
}

Movable::~Movable()
{
    SDL_DestroyTexture(imageTexture);
}

field Movable::getField()
{
    field localization;
    localization.fieldX = (localeX+40)/80;
    localization.fieldY = (localeY+40)/80;
    return localization;
}

field Movable::getPixelLocalization()
{
    field localization;
    localization.fieldX = localeX;
    localization.fieldY = localeY;
    return localization;
}

void Movable::animate()
{
    if(currentFrame<animationFrames-1)
    {
        currentFrame++;
    }
    else
    {
        currentFrame=0;
    }
    drawInfo.x = currentFrame*80;
}

void Movable::draw(SDL_Renderer* renderer, long& GlobalFrame)
{
    if(GlobalFrame%10 == 0)
    {
        animate();
    }
    drawDestination.x = localeX;
    drawDestination.y = localeY;
    SDL_RenderCopy(renderer, imageTexture, &drawInfo, &drawDestination);
}

void Movable::go(bool** accessible, field HeroPosition){};

void Movable::updatePosition(int direction, float speedMultiplier)
{
    switch(direction)
        {
            case 0: break;
            case 2: localeY-=4*speedMultiplier; break;
            case 7: localeY+=4*speedMultiplier; break;
            case 4: localeX-=4*speedMultiplier; break;
            case 5: localeX+=4*speedMultiplier; break;
            case 1: localeY-=3*speedMultiplier; localeX-=3*speedMultiplier; break;
            case 3: localeY-=3*speedMultiplier; localeX+=3*speedMultiplier; break;
            case 6: localeY+=3*speedMultiplier; localeX-=3*speedMultiplier; break;
            case 8: localeY+=3*speedMultiplier; localeX+=3*speedMultiplier; break;
        }
}

bool Movable::checkCanMove(short& direction, bool** accessible)
{
    //Outside of screen
    if(localeY<0 || localeX<0 || localeY>700 || localeX>1200)
    {
        if (
            ((direction == 1 || direction == 2 || direction == 3) && localeY<0)
            ||
            ((direction == 1 || direction == 4 || direction == 6) && localeX<0)
            ||
            ((direction == 6 || direction == 7 || direction == 8) && localeY>700)
            ||
            ((direction == 3 || direction == 5 || direction == 8) && localeX>1200)
            ) return false;
    }


    /* Direction convention
    *   1 2 3
    *   4 0 5
    *   6 7 8
    */
    int temp[]={0,0};
    switch(direction)   //Check collision with wall
    {
        case 0:
             return true; break;
        case 2:
             temp[0] = (localeX+40)/80;
             temp[1] = (localeY+36)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 7:
             temp[0] = (localeX+40)/80;
             temp[1] = (localeY+70)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 4:
             temp[0] = (localeX+10)/80;
             temp[1] = (localeY+40)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 5:
             temp[0] = (localeX+70)/80;
             temp[1] = (localeY+40)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 1:
             temp[0] = (localeX+10)/80;
             temp[1] = (localeY+10)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 3:
             temp[0] = (localeX+70)/80;
             temp[1] = (localeY+10)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 6:
             temp[0] = (localeX+10)/80;
             temp[1] = (localeY+70)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
        case 8:
             temp[0] = (localeX+70)/80;
             temp[1] = (localeY+70)/80;
             if(accessible[temp[0]][temp[1]]) return true;
             else return false;
             break;
    }
    return false;
}
