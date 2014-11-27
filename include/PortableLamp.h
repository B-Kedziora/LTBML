#ifndef PORTABLELAMP_H
#define PORTABLELAMP_H

#define COOLDOWN 500
#include "WrapperSDL.h"

/**
*	Static trap for ghosts. 
*/
class PortableLamp
{
    public:

        /**
        *   Check whether conditions to put lamp are met.
        *	@returns reference to newly created lamp or nullptr if lamp cannot be created
        */
        static PortableLamp* putLamp(field loc, SDL_Renderer* renderer);


        void draw(SDL_Renderer* renderer);

        static int getLampsLeft()
        {
            return lampsLeft_;
        }

        field getField()
        {
            return localiztion_;
        }

        static void resetLampsLeft()
        {
            lampsLeft_=5;
        }

        virtual ~PortableLamp();
    protected:
    private:
        PortableLamp(field loc);
        static signed short lampsLeft_;                     
        static SDL_Texture* portableLampTexture_;
        field localiztion_;
        static signed long lastPut_;

};

#endif // PORTABLELAMP_H
