#include "Level.h"

int Level::numberOfLevels=1;

const int WIDTH = 1280;
const int HEIGHT = 720;


enum gfx
{
    PATH=0,
    WALL=1,
    ROOF=2,
    WINDOW=3,
    VOID_FIELD=4,
    ROOF_WALL=5,
    DOOR=6,
    WINDOW2=7,
    WINDOW3=8,
    HEDGE=9
};

void Level::checkNumberOfLevels()
{
    for(int i=1; i<10000; i++)
    {   ///Preparing names
        std::ostringstream tempName;
        tempName<< "levels/level" << i << ".lvl";
        ifstream mapFile;
        mapFile.open(tempName.str());
        if(mapFile)                          
        {
            mapFile.close();
            numberOfLevels = i;
        }
        else
        {
            break;
        }
    }
}

Level::Level(int levelNumber)
{
    ///Checks how many levels is availble
    if(numberOfLevels == 1)
    {
        checkNumberOfLevels();
    }

    fieldAccesible = new bool*[16];
    for(int i=0;i<16;i++)
    {
        fieldAccesible[i] = new bool[9];
    }

    mapImage = IMG_Load("graphics/level_bg.png"); //Canvas for map

    ///Open map file
    std::ostringstream tempName;
    tempName<< "levels/level" << levelNumber << ".lvl";
    mapFile.open(tempName.str());
    if(!mapFile.is_open())
    {
        cout<<"Nie ma takiego pliku .lvl, niedobrze. \n";
    }
    else
    {
        loadMap();                          
    }


}

Level::~Level()
{
    try
    {
        for(int i=0;i<16;i++)
        {
            delete(fieldAccesible[i]);
        }
        delete(fieldAccesible);
        SDL_FreeSurface(mapImage);
    }
    catch(exception& love)
    {
        cout<<love.what()<<endl;
    }
}

void Level::loadGraphics(vector<SDL_Surface*> &graphicsMatrix)
{

    for(int i=0;i<10;i++)
    {
        std::ostringstream iterate;
        iterate<<i;
        SDL_Surface* img = WrapperSDL::loadSingleImage(iterate.str());
        graphicsMatrix.push_back(img);
    }
}

field Level::getHeroStart()
{
    return heroStart;
}

void Level::loadMap()
{
    srand(time(NULL));
    vector<SDL_Surface*> GraphicsArray;
    //Reading tiles graphics

        try
        {
            loadGraphics(GraphicsArray);
        }
        catch(...)
        {
            cout<<"Graphics loading error. \n";
        }

    ///Reading map from file
    short mapFromFile[16][9];
    try
    {
        ///Map image
        for(int i = 0; i < 9; i++)
        {
            for (int j = 0; j<16; j++ )
            {
                mapFile>>mapFromFile[j][i];
            }
        }
        ///Hero coords
        string loc;
        mapFile>>loc;
        int* positions = WrapperSDL::positionTokenizer ( loc );
        heroStart.fieldX = positions[0]*80;
        heroStart.fieldY = positions[1]*80;
        delete(positions);

        ///Loading lamps
        string temp2;
        mapFile>>temp2;
        if (temp2 == "<Lamp>")
        {
            mapFile>>temp2;
            while( temp2 != "</Lamp>")
            {
                lampPositions.push_back(temp2);
                mapFile>>temp2;
            }
            mapFile>>temp2;
            if(temp2 == "<Door>")
            {
                mapFile>>temp2;
                lampPositions.push_back(temp2);
                mapFile>>temp2;
                if(temp2!="</Door>")
                {
                    cout<<"There can only be one exit \n"<<endl;
                }
            }
            else
            {
                cout<<"Level file currupted. Lamp loading error."<<endl;
                throw(1);
            }
        }
        else
        {
            cout<<"Level file currupted. Lamp loading error."<<endl;
            throw(1);
        }

        ///Initial enemies
        mapFile>>temp2;
        if (temp2 == "<Enemy>")
        {
            mapFile>>temp2;
            while( temp2 != "</Enemy>")
            {
                enemyPositions.push_back(temp2);
                mapFile>>temp2;
            }
        }
        else
        {
            cout<<"Level file currupted. Enemy loading error."<<endl;
            throw(1);
        }

        ///Loading spawns
        mapFile>>temp2;
        if (temp2 == "<Spawn>")
        {
            mapFile>>temp2;
            while( temp2 != "</Spawn>")
            {
                spawnPositions.push_back(temp2);
                mapFile>>temp2;
            }
        }
    }
    ///Finish reading file
    catch(...)
    {
        cout<<"Map Loading Error, dafuq? \n";
        exit(1);
    }

    /// MapDrawing
    try
    {
        for(int i = 0; i < 9; i++)
        {
            for (int j = 0; j<16; j++ )
            {
                switch( mapFromFile[j][i] )
                {
                    case gfx(PATH):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(PATH)), mapImage);
                        fieldAccesible[j][i]=true;
                        break;
                    case gfx(WALL):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(WALL)), mapImage);
                        fieldAccesible[j][i]=false;
                        break;
                    case gfx(ROOF):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(ROOF)), mapImage);
                        fieldAccesible[j][i]=false;
                        break;
                    case gfx(WINDOW):
                        {
                            int windowSelect = rand()%7;
                            if(windowSelect<3)
                                WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(WINDOW)), mapImage);
                            else if(windowSelect<6)
                                WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(WINDOW2)), mapImage);
                            else
                                WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(WINDOW3)), mapImage);
                            fieldAccesible[j][i]=false;
                        }
                        break;
                    case gfx(VOID_FIELD):
                        fieldAccesible[j][i]=false;
                        break;
                    case gfx(ROOF_WALL):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(ROOF_WALL), mapImage);
                        fieldAccesible[j][i]=false;
                        break;
                    case gfx(DOOR):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(DOOR), mapImage);
                        fieldAccesible[j][i]=false;
                        break;
                    case gfx(HEDGE):
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(gfx(PATH)), mapImage);
                        WrapperSDL::putImage(80*j, 80*i, GraphicsArray.at(HEDGE), mapImage);
                        fieldAccesible[j][i]=false;
                        break;
                }
            }
        }

        ///HUD
        SDL_Surface* panel = WrapperSDL::loadSingleImage("InfoPanel");
        WrapperSDL::putImage(1119, 1 , panel , mapImage);
        SDL_FreeSurface(panel);
    }
    catch(exception& e)
    {
        cout<<"Map Drawing Error, meh? \n"<< e.what();
    }

    //Dealocates tiles graphics
    for(SDL_Surface* destroyer : GraphicsArray)
    {
        SDL_FreeSurface(destroyer);
    }

}

void Level::getFieldAccessibility(bool** table)
        {
            for(int i=0;i<16;i++)
            {
                for(int j=0;j<9;j++)
                {
                    table[i][j]=fieldAccesible[i][j];
                }
            }
        }
