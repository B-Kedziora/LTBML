#ifndef PHANTOM_H
#define PHANTOM_H

#include <Enemy.h>

/**
*	Enemy moving semi-random
*/
class Phantom : public Enemy
{
    public:
        Phantom(int startX, int startY, SDL_Renderer* renderer);
        virtual ~Phantom();

        /**
        *	Move towards next field then select new direction. Rush towards
        *	player if rush_ flag is set
        */
        void go(bool** accessible, field HeroPosition);

        /**
        *   For given string vector creates vector of phantoms. 
        */
        static void createInitialPhantoms(vector<string>, vector<Enemy*>&, SDL_Renderer* renderer);

    protected:
    private:
        field previousField_;   //To check if new field is reached
        short directionOfMove_;
        int minimalStepsLeft_;  //Minimal steps in one direction
        bool changedField_;     //whether enemy can change direction
};

#endif // PHANTOM_H
