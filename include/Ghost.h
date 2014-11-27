#ifndef GHOST_H
#define GHOST_H

#include "WrapperSDL.h"
#include "Enemy.h"

/**
*	Ghost moving on given path
*/
class Ghost : public Enemy
{
    public:
        Ghost(int startX, int startY, string moveList, SDL_Renderer* renderer);
        virtual ~Ghost();
        void go(bool** accessible, field HeroPosition);
    protected:
    private:
        vector<short> stepsDirectionList_;
        short stepsToNextDirection_;
        short currentPositionOnStepList_;
};

#endif // GHOST_H
