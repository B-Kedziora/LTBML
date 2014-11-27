#include "Board.h"


const int WIDTH = 1280;
const int HEIGHT = 720;
const int FRAME_TIME = 1000/60;

const int MAP_IMAGE = 0;

Board::Board()
{
    ///Init window
    gWindow = SDL_CreateWindow( "Let There Be More Light", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer( gWindow, -1 , 0 );
    WrapperSDL::SDL_Wrapper_init(renderer);
    keyboard = SDL_GetKeyboardState(NULL);                                  //Maps keyboard
    gameTimer = 0;
    menuHandler();
}

void Board::menuHandler()
{
    bool menuLeft = false;
    while(!menuLeft)
    {
        int menuChoice = menu();

        switch(menuChoice)
        {
            case 1: menuLeft = true; startGame(); break;        //start
            case 2: menuLeft = false; highScoreMenu(); break;   //Best times
            case 3: menuLeft = false; infoScreen(); break;      //info
            case 4: menuLeft = true; endGame(); break;          //end
            default: printf("Zla wartosc z menu\n"); endGame();
        }
    }
}

void Board::startGame()
{
    cout<<"Game Started"<<endl;
    currentLevelNumber_=1;
    bool restartedFlag = false;                     
    bool nextLevel;
    while(currentLevelNumber_<=Level::getNumberOfLevels())
    {
        initLevel(currentLevelNumber_);
        nextLevel = gameLoop();
        cout<<"Next level: "<<nextLevel<<endl;
        if(nextLevel)
        {
            currentLevelNumber_++;
            cleanMemory();
        }
        else
        {
            restartedFlag = true;
            restart();
        }
    }
    if(!restartedFlag && nextLevel)
    {
        endGame();
        hiScore((SDL_GetTicks() - gameTimer)/1000);
    }
}

Board::~Board()
{
	//endGame();
}

void Board::hiScore(int playerTime)
{
    cout<<"ENTERED HISCORE"<<endl;
    vector<hiScoreEntry> results;
    results.reserve(11);
    fstream file;
    hiScoreEntry tempEntry, playerEntry;
    string fileLine;

    /*Loading Hi-Score*/
    file.open("hiScore/hiScore.txt", std::fstream::in);
    for(int i=0;i<10;i++)
    {
        file>>tempEntry.name;
        file>>fileLine;
        tempEntry.time=atoi(fileLine.c_str());
        results.push_back(tempEntry);
    }
    file.close();

    playerEntry.time=playerTime;
    if(playerEntry.time < results.at(9).time)
    {
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nKim jestes? Jestes zwyciezca!"<<endl;
        cout<<"Podaj swoje imie: "<<endl;
        cin>>playerEntry.name;
        results.push_back(playerEntry);

        sort(results.begin(), results.end(),
            [](const hiScoreEntry& a, const hiScoreEntry& b)
                {
                   return a.time < b.time;
                }
            );
    }
    else
    {
        cout<<"Zbyt slaby wynik"<<endl;
    }

    /*Refreshing list and writing to file*/
    file.open("hiScore/hiScore.txt", std::fstream::out | std::fstream::trunc);
    cout<<"_Nr______________Imie___Czas"<<endl;
    for(int i=0;i<10;i++)
    {
        printf("%2d.%18s%7d\n", i, results.at(i).name.c_str(), results.at(i).time);
        file<<results.at(i).name<<"\t"<<results.at(i).time<<endl;
    }
    file.close();
    cout<<"Napisz cokolwiek, aby zakonczyc"<<endl;
    getchar();
}

void Board::endGame()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

void Board::initLevel(int levelNumber)
{
    currentLevelNumber_=levelNumber;
    fieldAccessible = new bool*[16];                                                //Alocate place
        for(int i=0;i<16;i++)
        {
            fieldAccessible[i] = new bool[9];
        }

    currentLevel_ = new Level(levelNumber);                                          //Reads map from file
    currentLevel_->getFieldAccessibility(fieldAccessible);                           //Read accessibility of fields
    field start = currentLevel_->getHeroStart();                                     //Reading hero init position
    hero = new Hero(start.fieldX, start.fieldY, renderer);                           //Creating hero 
    lampVector_ = Lamp::lampCreator(currentLevel_->getLampPositions(), renderer);    //Creating lamps
    Phantom::createInitialPhantoms(currentLevel_->getEnemyPositions(), enemyVector_, renderer);
    spawnVector_ = Spawn::createSpawns(currentLevel_->getSpawnPositions(), renderer); 
    mapImage_ = SDL_CreateTextureFromSurface(renderer , currentLevel_->getMapImage()); //Gets map texture
    delete(currentLevel_);                                                             //Dispose Level object
    hud = new Hud(renderer);
}

void Board::rendering(long frames, field mousePosition)
{
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, mapImage_, NULL, NULL);   //Background

        for(PortableLamp* lamp : hero->portableLampsOnMap)
        {
            lamp->draw(renderer);
        }

        hero->draw(renderer, frames, mousePosition);

        for( Lamp* lamp : lampVector_)
        {
            lamp->draw(renderer);
        }

        for(Enemy* enemy : enemyVector_)
        {
            enemy->draw(renderer, frames);
        }
        Enemy::displayDestruction(frames, renderer);

        for(Spawn* spawn: spawnVector_)
        {
            spawn->draw(renderer);
        }

        hud -> draw(Lamp::getLampNumber(),
                    PortableLamp::getLampsLeft(),
                    hero->flashlight->cooldownTimeLeft(),
                    hero->flashlight->getGhostKilled());

        SDL_RenderPresent(renderer);
}

bool Board::mechanicsHandler(long frames)
{
    vector<Enemy*>::iterator iter;

    //Movement
    hero->go(fieldAccessible);

    if(enemyVector_.size()>0)
    for(iter=enemyVector_.begin(); iter!=enemyVector_.end(); ++iter)
    {
        Enemy* enemy = *iter;
        enemy->go(fieldAccessible, hero->getPixelLocalization());
    }

    //Hero - Lamp
        int lampStatus = Lamp::testIfLampField(hero->getField());
        if(lampStatus == 9)
        {
            if(Lamp::checkIfDoorOpened())             
            {
                levelFinished_=true;                      /*Next lvl or game ends*/
                return true;                              /*breaks main loop*/
            }
        }
        else if(lampStatus>0)
        {
            lampVector_.at(lampStatus-1)->iluminate(); //"-1", lamps ID start on 1.
        }

    //Ghost-Lamp, then Ghost-hero
    for(iter=enemyVector_.begin(); iter!=enemyVector_.end(); ++iter)
    {
        Enemy* enemy = *iter;
        for(PortableLamp* lamp: hero->portableLampsOnMap)
        {
            if( WrapperSDL::fieldsEquality(lamp->getField(), enemy->getField()))
            {
                enemy->killed(renderer);            
                delete(enemy);
                enemyVector_.erase(iter);
                iter--;
                continue;
            }
        }

        if( WrapperSDL::fieldsEquality(hero->getField(), enemy->getField()))
        {
            return true;                            /*Hero killed*/
        }
    }

    for(Spawn* spawn : spawnVector_)
    {
        spawn->spawnAction(enemyVector_, frames, renderer);
    }


    return false;
}

bool Board::inputHandler(SDL_Event& e)
{
        hero->moveFlag=0;
        SDL_PumpEvents(); //Keyboard refresh

        ///Game exit
        if(keyboard[SDL_GetScancodeFromKey(SDLK_ESCAPE)]) return false;

        ///Hero movement
        if(keyboard[SDL_GetScancodeFromKey(SDLK_w)] &&  keyboard[SDL_GetScancodeFromKey(SDLK_a)]) hero->moveFlag=1;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_w)] &&  keyboard[SDL_GetScancodeFromKey(SDLK_d)]) hero->moveFlag=3;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_s)] &&  keyboard[SDL_GetScancodeFromKey(SDLK_a)]) hero->moveFlag=6;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_s)] &&  keyboard[SDL_GetScancodeFromKey(SDLK_d)]) hero->moveFlag=8;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_w)]) hero->moveFlag=2;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_s)]) hero->moveFlag=7;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_a)]) hero->moveFlag=4;
        else if(keyboard[SDL_GetScancodeFromKey(SDLK_d)]) hero->moveFlag=5;

        ///Atacks
        if(keyboard[SDL_GetScancodeFromKey(SDLK_LSHIFT)])
        {
            hero->flashlight->flash(enemyVector_, hero->getPixelLocalization(), renderer);
        }

        if(keyboard[SDL_GetScancodeFromKey(SDLK_SPACE)])
        {
            hero->putPortableLamp(renderer);
        }

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_MOUSEBUTTONUP)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                field mousePosition = {x, y};
                hero->flashlight->shoot(enemyVector_, renderer, hero->getPixelLocalization(), mousePosition);
                break; // Don't read events further
            }
        }

        return true;
}

int Board::gameLoop()
{
    levelFinished_=false;
    bool game = true;
    SDL_Event e;
    long startTime = SDL_GetTicks();
    long frames=0;
    long frameStart;
    long frameEnd;
    if(!gameTimer)                  /*Only at game start*/
        gameTimer = SDL_GetTicks();
    int mouseX,mouseY;
    while(game)
    {
        frameStart = SDL_GetTicks();

        game = inputHandler(e); //returns false if game closed

        if(mechanicsHandler(frames))
        {
            game = false;       //Collision with ghost
        }

        SDL_PumpEvents();
        SDL_GetMouseState(&mouseX,&mouseY);
        field mousePosition = {mouseX, mouseY};
        rendering(frames, mousePosition);

        ///Frame regulator: target 60FPS
        frameEnd = SDL_GetTicks();
        if( (frameEnd - frameStart) < FRAME_TIME )
                {
                    SDL_Delay( FRAME_TIME - (frameEnd - frameStart) );
                }

        frames++;
    }

    if(levelFinished_)
    {
        return 1;
    }
    else
        return 0; //restart
}

bool Board::highScoreMenu()
{
    SDL_SetRenderDrawColor(renderer,40,23,253,0);
    SDL_RenderClear(renderer);
    ifstream file;
    ostringstream stream;
    file.open("hiScore/hiScore.txt", std::fstream::in);
    string temp;
    for(int i=0;i<10;i++)
    {
        stream<<i<<".";
        WrapperSDL::applyText(50, 50+50*i, stream.str(), renderer, 0);
        stream.str("");
        file>>temp;
        stream<<temp;
        WrapperSDL::applyText(100, 50+50*i, stream.str(), renderer, 0);
        stream.str("");
        file>>temp;
        stream<<temp<<" s";
        WrapperSDL::applyText(450, 50+50*i, stream.str(), renderer, 0);
        stream.str("");
    }
    WrapperSDL::applyText(100, 600 , "(Nacisnij 'esc', zeby wrocic do menu.)", renderer, 1);
    file.close();
    SDL_Event e;
    SDL_RenderPresent(renderer);
    while(true)
    {
        while(SDL_PollEvent(&e))
        {
            if( e.type == SDL_KEYDOWN )
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    return true;
            }
        }
        SDL_Delay(10);
    }
    return false;
}

bool Board::infoScreen()
{
    SDL_RenderClear(renderer);
    SDL_Surface* temp = WrapperSDL::loadSingleImage("gameInfo");
    SDL_Texture* screen = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while(true)
    {
        while(SDL_PollEvent(&e))
        {
            if( e.type == SDL_KEYDOWN )
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                SDL_DestroyTexture(screen);
                return true;
            }
        }
        SDL_Delay(10);
    }
    return false;
}

int Board::menu()
{
    SDL_Surface* menuSurf = WrapperSDL::loadSingleImage("menu");
    SDL_Texture* menuTex = SDL_CreateTextureFromSurface(renderer, menuSurf );
    SDL_FreeSurface(menuSurf);

    int mouseX;
    int mouseY;

    const int buttonWidth = 300;
    const int buttonHeight = 100;
    Button* buttonNewGame = new Button(100, 170, buttonWidth, buttonHeight);
    Button* field buttonHiScore = new Button(100, 318, buttonWidth, buttonHeight);
    Button* field buttonInfo = new Button(100, 467, buttonWidth, buttonHeight);
    Button* field buttonQuit = new Button(915, 580, buttonWidth, buttonHeight);

    SDL_Event e;
    while(true)                             //Gets mouse state
    {
        SDL_PollEvent(&e);
        if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
        {
            SDL_RenderCopy(renderer, menuTex, NULL, NULL);          //Renders menu

            SDL_GetMouseState(&mouseX, &mouseY);

            if( buttonNewGame -> checkMouseHover(mouseX, mouseY) );
            {
                if( e.type == SDL_MOUSEBUTTONUP)
                {
                    return 1;
                }
            }

            if( (buttonHiScore -> checkMouseHover(mouseX, mouseY) )
            {
                if( e.type == SDL_MOUSEBUTTONUP)
                {
                    return 2;
                }
            }

            if( buttonInfo -> checkMouseHover(mouseX, mouseY) )
            {
                if( e.type == SDL_MOUSEBUTTONUP)
                {
                    return 3;
                }
            }

            if( buttonQuit -> checkMouseHover(mouseX, mouseY))
            {
                if( e.type == SDL_MOUSEBUTTONUP)
                {
                    endGame();
                    SDL_Quit();
                    exit(0);
                }
            }

            SDL_RenderPresent(renderer);
        }
        else if(e.type == SDL_QUIT)
        {
                endGame();
                SDL_Quit();
                exit(0);
        }

    SDL_Delay(20); //Menu framerate is not important
    }
    return 1;
}

int Board::cleanMemory()
{
    try
    {
        Lamp::closeDoor();

        delete(hero);

        for(Enemy* enemy : enemyVector_)
        {
            delete(enemy);
        }
        enemyVector_.clear();

        SDL_DestroyTexture(mapImage_);

        for(Lamp* a: lampVector_)
        {
            delete(a);
        }
        lampVector_.clear();

        for(int i=0; i<16; i++)
        {
            delete[] fieldAccessible[i];
        }
        delete[] fieldAccessible;

        for(Spawn* spawn: spawnVector_)
        {
            delete(spawn);
        }
        spawnVector_.clear();

    }
    catch(std::exception& e)
    {
        cout<<e.what()<<endl<<"Blad zwalaniania pamieci"<<endl;
        return -1;
    }
    return 0;
}

void Board::restart()
{
    cleanMemory();
    showGameOverInfo();
    gameTimer = 0;
    PortableLamp::resetLampsLeft();
    menuHandler();
}

void Board::showGameOverInfo()
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    WrapperSDL::applyText(300,300,"GAME OVER", renderer, 2);
    SDL_Delay(120);
    SDL_RenderPresent(renderer);
}
