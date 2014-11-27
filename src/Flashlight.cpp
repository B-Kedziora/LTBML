#include "Flashlight.h"

#define FLASH_RNG 120

Flashlight::Flashlight(SDL_Renderer* renderer)
{
    range_ = 270;
    cooldown_ = 880;
    ghostKilled_ = 0;
    cooldownTimer_ = 0;
    lastAttackMode_=0;
    upgraded_ = false;

    ///Textures 
    //Flash
    SDL_Surface* temp = WrapperSDL::loadSingleImage("flash");
    flashTexture_ = SDL_CreateTextureFromSurface(renderer,temp);
    SDL_SetTextureAlphaMod( flashTexture_ , 50 );
    SDL_FreeSurface(temp);
    //Range
    temp = WrapperSDL::loadSingleImage("range");
    rangeTexture_ = SDL_CreateTextureFromSurface(renderer,temp);
    SDL_SetTextureAlphaMod( rangeTexture_ , 50 );
    SDL_FreeSurface(temp);
}

void Flashlight::upgrade(SDL_Renderer* renderer)
{
    upgraded_ = true;
    range_ +=110;
    cooldown_ -= 360;
}

int Flashlight::cooldownTimeLeft()
{
        int time = (cooldownTimer_ + cooldown_) - SDL_GetTicks();
        if(time < 0)
        {
            return 0;
        }
        else
        {
            return time;
        }
}

bool Flashlight::shoot(vector<Enemy*>& enemyVector, SDL_Renderer* renderer, field heroField, field mousePosition)
{
    if((cooldownTimer_ + cooldown_) <SDL_GetTicks()) ///Check cooldown
    {
        cooldownTimer_ = SDL_GetTicks();
        lastAttackMode_ = 1;

        int i=0;
        for(Enemy* enemy : enemyVector)
        {
            ///Check if hit
            if(WrapperSDL::lineIntesectsSquare(heroField, mousePosition, enemy->getPixelLocalization(), 80, 80)
               && WrapperSDL::checkIfInRadius(heroField, enemy->getPixelLocalization(), range_+20))
            {

                enemy->killed(renderer);
                enemyVector.erase(enemyVector.begin()+i);
                i--;                                          
                ghostKilled_++;
                if(ghostKilled_ == 3)
                {
                    upgrade(renderer);
                }
            }
            i++;
        }
    }
    return 1;
}

int Flashlight::flash(vector<Enemy*>& enemyVector, field heroField, SDL_Renderer* renderer)
{
    heroField.fieldX+=20; //Exact location of hero's hand
    if(upgraded_ && (cooldownTimer_ + cooldown_) <SDL_GetTicks())
    {
        cooldownTimer_ = SDL_GetTicks();
        lastAttackMode_ = 2;

        int i = 0;
        for(Enemy* enemy: enemyVector)
        {
                if(WrapperSDL::checkIfInRadius(enemy->getPixelLocalization(), heroField, FLASH_RNG))
                {
                    enemy->killed(renderer);
                    enemyVector.erase(enemyVector.begin()+i);
                    i--;
                    ghostKilled_++;
                }
                i++;
        }
    }
    return 1;
}

///shot()
int Flashlight::drawLight(field heroLocation, field targetLocation, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255,255,0,0);
    int x1=heroLocation.fieldX+15;
    int x2=targetLocation.fieldX;
    int y1=heroLocation.fieldY+30; ///Fits hero's hand
    int y2=targetLocation.fieldY;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x1+1, y1+1, x2+1, y2+1);
    return 1;
}

///flash()
int Flashlight::drawLight(field heroField, SDL_Renderer* renderer)
{
        WrapperSDL::putImage(heroField.fieldX - 80, heroField.fieldY - 80, flashTexture_, renderer);
        return 1;
}

void Flashlight::draw(field heroLocation, field targetLocation, SDL_Renderer* renderer)
{
    ///Atack
    if( (SDL_GetTicks() - cooldownTimer_) < 150)
    {
        if(lastAttackMode_==1)
        {
            drawLight(heroLocation, targetLocation, renderer);
        }
        else if(lastAttackMode_==2)
        {
            drawLight(heroLocation, renderer);
        }
    }

    ///Range
    if(!upgraded_)
    {
        SDL_Rect source={0,110,534,540};
        SDL_Rect destination={heroLocation.fieldX-270, heroLocation.fieldY-270, 540,540};
        SDL_RenderCopy(renderer, rangeTexture_, &source, &destination );
    }
    else
    {
        SDL_Rect source={535,0,765,760};
        SDL_Rect destination={heroLocation.fieldX-380, heroLocation.fieldY-380,760,765};
        SDL_RenderCopy(renderer, rangeTexture_, &source, &destination );
    }
}

Flashlight::~Flashlight()
{
    SDL_DestroyTexture(flashTexture_);
    SDL_DestroyTexture(rangeTexture_);
}
