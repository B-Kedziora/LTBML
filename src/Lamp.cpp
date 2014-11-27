#include "Lamp.h"
SDL_Texture* Lamp::lightOffTexture = nullptr;
SDL_Texture* Lamp::lightOnTexture = nullptr;
SDL_Texture* Lamp::doorOpenedTexture = nullptr;
int Lamp::lampNumber = 0;
bool Lamp::doorOpened = false;
int** Lamp::lampMap = nullptr;

Lamp::Lamp(int x, int y, SDL_Renderer* renderer, int lampID)
{
    ///Texture loading for lamps. Only once
    if( !lightOffTexture || !lightOnTexture )
    {
        SDL_Surface* temp1 = WrapperSDL::loadSingleImage("lampOff");
        SDL_Surface* temp2 = WrapperSDL::loadSingleImage("lampOn");
        SDL_Surface* temp3 = WrapperSDL::loadSingleImage("DoorOpenedTexture");
        lightOffTexture = SDL_CreateTextureFromSurface(renderer, temp1);
        lightOnTexture = SDL_CreateTextureFromSurface(renderer, temp2);
        doorOpenedTexture = SDL_CreateTextureFromSurface(renderer, temp3);
        SDL_FreeSurface(temp1);
        SDL_FreeSurface(temp2);
        SDL_FreeSurface(temp3);
        if(!lightOffTexture || !lightOnTexture || !doorOpenedTexture)
        {
            cout<<"Lamp gfx loading error. So sad."<<endl;
        }
    }

    ///Alocates place for lamp map
    if( lampMap == nullptr )
    {
        lampMap = new int*[16];
        for(int i=0;i<16;i++)
        {
            lampMap[i] = new int[9];
            for(int j=0; j<9 ; j++)
            {
                lampMap[i][j]=0;
            }
        }
    }

    lampNumber++;
    lit = false;
    this->lampID = lampID;
    drawInfo.h = 160;
    drawInfo.w = 80;
    drawInfo.x = x * 80;
    drawInfo.y = (y-1) * 80; //Lamp take to fields vertically 
}

Lamp::Lamp(int x, int y, SDL_Renderer* renderer)
{
    lampID = 9;
    SDL_QueryTexture(doorOpenedTexture, NULL, NULL, &drawInfo.w, &drawInfo.h);
    drawInfo.x = x * 80;
    drawInfo.y = (y-2) * 80;
}

void Lamp::iluminate()
{
    if(lit==false)
    {
        lit = true;
        lampNumber--;
        if(!lampNumber)             //All lamps on. Can end level
        {
            doorOpened = true;
        }
    }
}

void Lamp::draw(SDL_Renderer* renderer)
{
    if(lampID!=9)
    {
        if( lit )
        {
            SDL_RenderCopy(renderer, lightOnTexture, NULL, &drawInfo );
        }
        else
        {
            SDL_RenderCopy(renderer, lightOffTexture, NULL, &drawInfo );
        }
    }
    else if(doorOpened)
    {
        SDL_RenderCopy(renderer, doorOpenedTexture, NULL, &drawInfo);
    }
}

int Lamp::testIfLampField(field HeroPosition)
{
    if( lampMap[HeroPosition.fieldX][HeroPosition.fieldY])
    {
        return (lampMap[HeroPosition.fieldX][HeroPosition.fieldY]);
    }
    else return -1;
}

vector<Lamp*> Lamp::lampCreator (vector<string> lampPositions, SDL_Renderer* renderer)
{
    vector<Lamp*> lamps;
    int lampID = 1;   
    string temp;        //Lines from lampPosition
    int* locXY = new int[2];         //Coords

    swap(lampPositions.front(), lampPositions.back()); //Door is last element

    while( !lampPositions.empty() )
    {
        ///Parsing string
        temp = lampPositions.back();
        locXY = WrapperSDL::positionTokenizer( temp );
        int x = locXY[0]; 
        int y = locXY[1]; 

        if(lampPositions.size() > 1)
        {
            lamps.push_back(new Lamp(x, y, renderer, lampID));

            //Mark lamp on lampMap
            lampMap[x][y] = lampID++;
        }
        else /*Door*/
        {
            lamps.push_back(new Lamp(x,y,renderer));
            lampMap[x][y] = 9;
        }

        lampPositions.pop_back();
    }

    delete(locXY);
    return lamps;
}

void Lamp::printLampMap()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<16; j++)
        {
            cout<<lampMap[j][i]<<" ";
        }
        cout<<endl;
    }
}

Lamp::~Lamp()
{
    lampNumber=0;
    if(lampMap)
    {
        for(int i=0;i<16;i++)
        {
            delete[] lampMap[i];
        }
        delete[] lampMap;
        lampMap=nullptr;
    }
}
