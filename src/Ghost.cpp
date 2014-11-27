#include "Ghost.h"

Ghost::Ghost(int startX, int startY, string moveList , SDL_Renderer* renderer) : Enemy(startX*80, startY*80, "Ghost", renderer)
{
    rushFlag_ = false;
    stepsToNextDirection_ = 20;
    currentPositionOnStepList_ = 0;
    for(int i=0; i<moveList.length(); i++)
    {
        char temp=moveList[i];
        int step = atoi(&temp);
        stepsDirectionList_.push_back(step);
    }
}

void Ghost::go(bool** accessible, field HeroPosition)
{
    //Hero has been spotted
    if(rushFlag_)
    {
        updatePosition(rush(HeroPosition, getPixelLocalization()), 1.5);
    }
   
    else if( WrapperSDL::checkIfInRadius(HeroPosition, getPixelLocalization(), 240) )
    {
        rushFlag_ = true;
    }
    else if (stepsToNextDirection_) ///Continue movement in the same direction
    {
        updatePosition(stepsDirectionList_.at(currentPositionOnStepList_), 1.0);
        stepsToNextDirection_--;
    }
    else    ///Change direction
    {
        if( currentPositionOnStepList_ < stepsDirectionList_.size()-1 ) //List of moves didn't end
        {
            currentPositionOnStepList_++;
            stepsToNextDirection_ = 20;
        }
        else    ///Start list again
        {
            currentPositionOnStepList_=0;
            stepsToNextDirection_ = 20;
        }
    }
}

Ghost::~Ghost()
{
    //dtor
}
