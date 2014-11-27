#include "Phantom.h"

Phantom::Phantom(int startX, int startY, SDL_Renderer* renderer) : Enemy (startX*80, startY*80, "Phantom", renderer)
{
    previousField_ = {startX, startY};
    directionOfMove_ = 0;
    rushFlag_ = false;
    minimalStepsLeft_ =0;
    changedField_ = true;
}

Phantom::~Phantom()
{
    //dtor
}

void Phantom::go(bool** accessible, field HeroPosition)
{
    ///Rush if hero have been spotted
    if(rushFlag_)
    {
        updatePosition(rush(HeroPosition, getPixelLocalization()), 1.5);
    }
    ///Check if hero is spotted
    else if( WrapperSDL::checkIfInRadius(HeroPosition, getPixelLocalization(), 240) )
    {
        rushFlag_ = true;
    }
    ///Select new direction
    else if( (changedField_ || !directionOfMove_) && !minimalStepsLeft_ )
    {
        field temp = getField();
        vector<int> availbleMovements;
        bool notCrossUp, notCrossDown, notCrossLeft, notCrossRight;
        if(temp.fieldY-1>=0) notCrossUp=true; else notCrossUp=false;
        if(temp.fieldY+1<=8) notCrossDown=true; else notCrossDown=false;
        if(temp.fieldX-1>=0) notCrossLeft=true; else notCrossLeft=false;
        if(temp.fieldX+1<=15) notCrossRight=true; else notCrossRight=false;

        if( notCrossUp && notCrossLeft )
            if( accessible[temp.fieldX-1][temp.fieldY-1] )
            {
                availbleMovements.push_back(1);
            }

        if( notCrossUp )
            if( accessible[temp.fieldX][temp.fieldY-1] )
            {
                availbleMovements.push_back(2);
            }

         if( notCrossUp && notCrossRight )
            if( accessible[temp.fieldX+1][temp.fieldY-1] )
            {
                availbleMovements.push_back(3);
            }

         if( notCrossLeft )
            if( accessible[temp.fieldX-1][temp.fieldY] )
            {
                availbleMovements.push_back(4);
            }

         if( notCrossRight )
            if( accessible[temp.fieldX+1][temp.fieldY] )
            {
                availbleMovements.push_back(5);
            }

         if( notCrossDown && notCrossLeft )
            if( accessible[temp.fieldX-1][temp.fieldY+1] )
            {
                availbleMovements.push_back(6);
            }

         if( notCrossDown )
            if( accessible[temp.fieldX][temp.fieldY+1] )
            {
                availbleMovements.push_back(7);
            }

         if( notCrossDown && notCrossRight )
            if( accessible[temp.fieldX+1][temp.fieldY+1] )
            {
                availbleMovements.push_back(8);
            }
        if(availbleMovements.size() == 0) {cout<<"brak dostepnych ruchow"<<endl;}
        directionOfMove_ = availbleMovements.at(rand()%availbleMovements.size());
        minimalStepsLeft_=15;
    }
    else //continue movement
    {
        minimalStepsLeft_--;
            if(!changedField_)
            {
                previousField_ = getField();
                updatePosition(directionOfMove_, 1.0);                                    ///Ruch
                changedField_=!WrapperSDL::fieldsEquality( previousField_, getField() );
            }
            else
                updatePosition(directionOfMove_, 1.0);                                    ///Ruch
    }
}

void Phantom::createInitialPhantoms(vector<string> enemyPositions, vector<Enemy*>& enemyVector, SDL_Renderer* renderer)
{
    string temp;                     //Stores line from enemyPositions
    int* locXY = new int[2];         //Coords

    while( !enemyPositions.empty() )
    {
        ///Parse string into coords
        temp = enemyPositions.back();
        locXY = WrapperSDL::positionTokenizer( temp );
        int x = locXY[0];
        int y = locXY[1];

        enemyVector.push_back(new Phantom(x, y, renderer));

        enemyPositions.pop_back();
    }

    delete(locXY);

}
