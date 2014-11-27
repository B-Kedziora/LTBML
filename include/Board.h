#ifndef BOARD_H
#define BOARD_H
#include <SDL.h>
#include <Level.h>
#include <Hero.h>
#include <WrapperSDL.h>
#include <SDL_image.h>
#include <vector>
#include <Lamp.h>
#include <Phantom.h>
#include <Ghost.h>
#include <Spawn.h>
#include <iostream>
#include <algorithm>
#include <PortableLamp.h>
#include <Hud.h>
#include <Button.h>

class Board
{
    public:
        Board();
        virtual ~Board();
    protected:
    private:
    SDL_Window* gWindow = nullptr;      //Game window
    SDL_Renderer* renderer = nullptr;   //Main game canvas
    Level* currentLevel_ = nullptr;     //Loader of current level. Exists only while init
    int currentLevelNumber_;            
    Hero* hero = nullptr;               //Player class. 
    long gameTimer;                     //Stores time of a game
    SDL_Texture* mapImage_;             //Rendered background
    const Uint8* keyboard;              //Stores keyboard state since last pumpEvents() or PollEvents()
    bool** fieldAccessible;             //Determines which map parts can be accessed
    vector<Lamp*> lampVector_;
    vector<Enemy*> enemyVector_;        //Both ghosts and phanotoms
    vector<Spawn*> spawnVector_;        //Points creating enemies
    bool levelFinished_;                //Stores whether game loop was exited due to finishing level
    Hud* hud = nullptr;                 //Info panel on the right-top of the screen

    /*Used by menu handler. Returns chosen option*/
    int menu();

    /**
    * Main loops. Returns reason of break: finishing level, return key or game over
    */
    int gameLoop();

    /**
	* Loads level from file and initilize all objects
    */
    void initLevel(int levelNumber);


    /*Renders game state on screen after updating*/
    void rendering(long frames, field mousePosition);

    /*Manages game interaction. Can break main loop*/
    bool inputHandler(SDL_Event& e);

    /*Manages interaction between game objects like player, enemies, lamps...*/
    bool mechanicsHandler(long frames);

    /*Shows menu*/
    void menuHandler();

    /*Dealocate memory and destroys window*/
    void endGame();

    /*Dealocate memory and shows main menu*/
    void restart();

    /*Shows game over screen*/
    void showGameOverInfo();

    /*Cleans memory used by level*/
    int cleanMemory();

    /*Allows player to type his name*/
    void hiScore(int playerTime);

    /*Shows high score screen*/
    bool highScoreMenu();

    /*Shows game info screen*/
    bool infoScreen();

    /*Starts game and controlls its exit*/
    void startGame();
};

#endif // BOARD_H
