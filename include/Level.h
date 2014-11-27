#ifndef LEVEL_H
#define LEVEL_H
#include <SDL.h>
#include <SDL_image.h>
#include <WrapperSDL.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <memory>
#include <time.h>

using namespace std;

class Level
{
    public:

        ///Init level
        Level(int levelNumber);

        ///Returns hero position
        field getHeroStart();

        ///Init board info
        void getFieldAccessibility(bool** table);

        ///Read in Board
        SDL_Surface* getMapImage()
        {
            return mapImage;
        }

        vector<string> getLampPositions()
        {
            return lampPositions;
        }

        vector<string> getEnemyPositions()
        {
            return enemyPositions;
        }

        vector<string> getSpawnPositions()
        {
            return spawnPositions;
        }

        static int getNumberOfLevels()
        {
           return numberOfLevels;
        }

        ///After parsing file object is no longer needed
        virtual ~Level();

    protected:
    private:
        bool** fieldAccesible; //Fields on which Hero can move
        string levelName;
        ifstream mapFile;     
        SDL_Surface* mapImage; //Ready background image
        field heroStart;
        vector<string>lampPositions;
        vector<string>spawnPositions;
        vector<string>enemyPositions;
        static int numberOfLevels;

        static void checkNumberOfLevels();

        ///Loading procedure
        void loadMap();

        ///Loads tiles to create mapImage
        void loadGraphics(vector<SDL_Surface*>& graphics);
};

#endif // LEVEL_H
