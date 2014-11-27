#include "Spawn.h"

SDL_Texture* Spawn::spawnTexture_ = nullptr;

Spawn::Spawn(int x, int y, string ghostPath, int ghostSpawnRate, int phantomSpawnRate)
{
    fieldX_ = x;
    fieldY_ = y;
    ghostPath_ = ghostPath;
    ghostSpawnRate_ = ghostSpawnRate;
    phantomSpawnRate_ = phantomSpawnRate;
}

void Spawn::draw(SDL_Renderer* renderer)
{
    WrapperSDL::putImage(fieldX_*80, fieldY_*80, spawnTexture_, renderer);
}

vector<Spawn*> Spawn::createSpawns(vector<string> spawnInfo, SDL_Renderer* renderer) throw(string)
{
    if(!spawnTexture_)
    {
        SDL_Surface* temp = WrapperSDL::loadSingleImage("Spawn");
        spawnTexture_ = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_SetTextureAlphaMod( spawnTexture_ , 230 );
        SDL_FreeSurface(temp);
    }

    vector<Spawn*> readySpawns;
    string tempGhostPath;
    string tempPhantomTime;
    string tempGhostTime;
    string tempPosition;

    int* XY = new int[2];

    if(spawnInfo.size() % 4 != 0)
    {
        cout<<"Spawn creating error. Check map file."<<endl;
        throw("SPAWN");
    }

    cout<<"Spawn size: "<<spawnInfo.size()<<endl;

    while( !spawnInfo.empty() )
    {
        tempGhostPath=spawnInfo.back();
        spawnInfo.pop_back();
        tempPhantomTime=spawnInfo.back();
        spawnInfo.pop_back();
        tempGhostTime=spawnInfo.back();
        spawnInfo.pop_back();
        tempPosition=spawnInfo.back();
        spawnInfo.pop_back();
        int x,y;
        XY = WrapperSDL::positionTokenizer(tempPosition);
        x=XY[0];
        y=XY[1];
        readySpawns.push_back(new Spawn(x,y, tempGhostPath, atoi(tempGhostTime.c_str()), atoi(tempPhantomTime.c_str())));
    }

    delete(XY);
    return readySpawns;
}

void Spawn::spawnAction(vector<Enemy*>& enemyVector, long frames, SDL_Renderer* renderer)
{
    frames++;
    if(frames % ghostSpawnRate_ == 0)
    {
        enemyVector.push_back(new Ghost(fieldX_, fieldY_, ghostPath_, renderer));
    }
    if(frames % phantomSpawnRate_ == 0)
    {
        enemyVector.push_back(new Phantom(fieldX_, fieldY_, renderer));
    }
}

Spawn::~Spawn()
{
    //dtor
}
