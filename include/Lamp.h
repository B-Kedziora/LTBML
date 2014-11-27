#ifndef LAMP_H
#define LAMP_H
#include <SDL.h>
#include <SDL_image.h>
#include <WrapperSDL.h>
#include <vector>

class Lamp
{
    public:
        ///lampID for easier checking collision
        Lamp(int x, int y, SDL_Renderer* renderer, int lampID);

        Lamp(int x, int y, SDL_Renderer* renderer);

        ///Make lamp on
        void iluminate();

        ///Returns lampID if hero is on its position
        static int testIfLampField(field HeroPosition);

        ///After loading map. Parse string into lamp vector
        static vector<Lamp*> lampCreator (vector<string> lampPositions, SDL_Renderer* renderer);


        void draw(SDL_Renderer* renderer);

        ///Prints position of all lamps. Debug purposes.
        static void printLampMap();

        static int getLampNumber()
        {
            return lampNumber;
        }

        static bool checkIfDoorOpened()
        {
            return doorOpened;
        }

        static void closeDoor()
        {
            doorOpened = false;
        }

        virtual ~Lamp();
    protected:
    private:
        static int** lampMap;    //Map containing position of lamps
        static int lampNumber;   //How many lamps remain off
        static SDL_Texture* lightOffTexture;
        static SDL_Texture* lightOnTexture;
        static SDL_Texture* doorOpenedTexture;
        int lampID;              
        SDL_Rect drawInfo;       
        bool lit;                //If lamp is on 
        static bool doorOpened;  //Set if all lamps are on
};


#endif // LAMP_H
