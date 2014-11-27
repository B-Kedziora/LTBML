#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "WrapperSDL.h"
#include <Enemy.h>

/**
*   Encapsulates hero ability to defend against enemies
*/
class Flashlight
{
    public:
        Flashlight(SDL_Renderer* renderer);
        virtual ~Flashlight();

        /**
        *  	Area of effect ability of player. Do nothing if upgraded_ not set.
        *	@return - number of killed enemy
        */
        int flash(vector<Enemy*>& enemyVector, field heroField, SDL_Renderer* renderer);

        /**
        *   Tests whether flashlight ray hits enemy and checks if enemy is in range.
        *	@return - is enemy killed
        */
        bool shoot(vector<Enemy*>& enemyVector, SDL_Renderer* renderer, field heroField, field mousePosition);

        /**
        *   @return time until next shoot can be performed
        */
        int cooldownTimeLeft();

        /**
        *   Draws light effect.
        */
        void draw(field heroLocation, field targetLocation, SDL_Renderer* renderer);

        short getGhostKilled()
        {
            return ghostKilled_;
        }

        bool checkIfUpgraded()
        {
            return upgraded_;
        }

    protected:
    private:

        /**
        *   Called after killing 3 ghosts. Extend range, reduce cooldown and give area of effect atack
        */
        void upgrade(SDL_Renderer* renderer);

        int drawLight(field heroLocation, field targetLocation, SDL_Renderer* renderer); //shot()
        int drawLight(field heroField, SDL_Renderer* renderer); //flash()
        int range_;
        int cooldown_;
        int cooldownTimer_;
        int ghostKilled_;
        int lastAttackMode_;
        bool upgraded_;
        SDL_Texture* flashTexture_ = nullptr;
        SDL_Texture* rangeTexture_ = nullptr;
};

#endif // FLASHLIGHT_H
