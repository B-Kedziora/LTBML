#ifndef HERO_H
#define HERO_H
#include "Movable.h"
#include <SDL.h>
#include <SDL_image.h>
#include <WrapperSDL.h>
#include "Flashlight.h"
#include "PortableLamp.h"

/**
*	Player object. 
*/
class Hero : public Movable
{
    public:
        Hero(int startX, int startY, SDL_Renderer* renderer);
        virtual ~Hero();
        void go(bool** accessible);
        void draw(SDL_Renderer* renderer, long& globalFrames, field mousePosition);
        void putPortableLamp(SDL_Renderer* renderer);

        short moveFlag;

        //Objects below are used to kill enemies
        Flashlight* flashlight;	///Encapsulates hero ability to defend
        vector<PortableLamp*> portableLampsOnMap; ///Loc of portable lamps
    protected:
    private:
};

#endif // HERO_H
