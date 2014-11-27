#include "Enemy.h"

vector<DestructionSprite*> Enemy::effectVector_;

Enemy::Enemy(int startX, int startY, string name, SDL_Renderer* renderer) : Movable (startX, startY, name, renderer)
{
    //ctor
}

Enemy::~Enemy()
{
    //dtor
}

/**
*   After spotting hero moves towards him and don't concern with filed accessibility. Why should he? It's a ghost.
*/
short Enemy::rush(field HeroPosition, field ownPosition)
{
    if( HeroPosition.fieldX == ownPosition.fieldX )
    {
        if(HeroPosition.fieldY>ownPosition.fieldY)
        {
            return 7;
        }
        else
            return 2;
    }
    else if (HeroPosition.fieldY == ownPosition.fieldY)
    {
        if(HeroPosition.fieldX>ownPosition.fieldX)
        {
            return 5;
        }
        else
            return 4;
    }
    else if(HeroPosition.fieldX > ownPosition.fieldX)
    {
        if(HeroPosition.fieldY>ownPosition.fieldY)
        {
            return 8;
        }
        else
            return 3;
    }
    else
    {
        if(HeroPosition.fieldY>ownPosition.fieldY)
        {
            return 6;
        }
        else
            return 1;
    }
}

void Enemy::clearDestructionVector()
{
    for(DestructionSprite* a: effectVector_)
    {
        delete(a);
    }
    effectVector_.clear();
}

void Enemy::displayDestruction(long frames, SDL_Renderer* renderer )
{
    short ended=0;
    for(DestructionSprite* a: effectVector_)
    {
        if(a->drawEffect(frames, renderer))
        ended++;
    }
    if(ended == effectVector_.size())
        clearDestructionVector();
}
