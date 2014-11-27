#ifndef SPAWN_H
#define SPAWN_H
#include "WrapperSDL.h"
#include "Ghost.h"
#include "Phantom.h"

/**
*	Place creating new enemies
*/
class Spawn
{
    public:
        virtual ~Spawn();

        /**
        *	Cooperate with level init. Returns all Spawns on map
        */
        static vector<Spawn*> createSpawns(vector<string> spawnInfo, SDL_Renderer* renderer) throw(string);

        /**
        *	Spawn enemies if enough time elapsed
        */
        void spawnAction(vector<Enemy*>& enemyVector, long frames, SDL_Renderer* renderer);
        void draw(SDL_Renderer* renderer);
    protected:
    private:
    	/**
    	*	Called by spawn action
    	*/
        Spawn(int x, int y, string ghostPath, int ghostSpawnRate, int phantomSpawnRate);
        int fieldX_;	///Location of spawner
        int fieldY_;
        string ghostPath_;	///Ghosts spawned by this spawner will follow this way
        int ghostSpawnRate_;
        int phantomSpawnRate_;
        static SDL_Texture* spawnTexture_;
};

#endif // SPAWN_H
