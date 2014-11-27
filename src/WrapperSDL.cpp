#include "WrapperSDL.h"

SDL_Surface* WrapperSDL::r = nullptr;
SDL_Texture* WrapperSDL::highlight = nullptr;
TTF_Font* WrapperSDL::font = nullptr;
TTF_Font* WrapperSDL::font2 = nullptr;
TTF_Font* WrapperSDL::font3 = nullptr;
SDL_Color WrapperSDL::fontColor;
SDL_Rect WrapperSDL::parametrs;

WrapperSDL::WrapperSDL()
{
    //ctor
}

WrapperSDL::~WrapperSDL()
{
    //dtor
}

void WrapperSDL::SDL_Wrapper_init(SDL_Renderer* renderer)
{
    if(r == nullptr)
    {
        r = loadSingleImage("highlight");
    }
    if (highlight == nullptr)
    {
        highlight = SDL_CreateTextureFromSurface(renderer, r);
    }
    SDL_FreeSurface(r);

    font = TTF_OpenFont("verdana.ttf", 28);
    font2 = TTF_OpenFont("verdana.ttf", 20);
    font3 = TTF_OpenFont("verdana.ttf", 128);
    fontColor.a=0;
    fontColor.r=255;
    fontColor.b=255;
    fontColor.g=255;
}

SDL_Surface* WrapperSDL::loadSingleImage(string name)
{
    std::ostringstream path;
    path<<"graphics/"<< name <<".png";
    SDL_Surface* image = IMG_Load( path.str().c_str() );
    if ( image == nullptr)
    {
        cout<<"Image loading error Level::loadSingleImage \nRequested file: "<<name.c_str()<<"\n\n";
    }
    else
    {
        return image;
    }
    return nullptr;
}

void WrapperSDL::applyText(int x, int y, string text, SDL_Renderer* renderer, int fontSize)
{
    SDL_Surface* stats = nullptr;
    switch(fontSize)
    {
        case 0: stats = TTF_RenderText_Solid(font, text.c_str(), fontColor); break;
        case 1: stats = TTF_RenderText_Solid(font2, text.c_str(), fontColor); break;
        case 2: stats = TTF_RenderText_Solid(font3, text.c_str(), fontColor); break;
        default: cout<<"Zly rozmiar fontu"<<endl;
    }
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, stats);
    WrapperSDL::putImage(x,y,fontTexture,renderer);
    SDL_FreeSurface(stats);
    SDL_DestroyTexture(fontTexture);
}

void WrapperSDL::putImage(int x, int y, int dx, int dy, SDL_Texture* source, SDL_Renderer* destination)
{
    SDL_Rect draw;
    draw.h=80;
    draw.w=80;
    draw.x=dx;
    draw.y=dy;
    parametrs.x = x;
    parametrs.y = y;
    parametrs.h = 80;
    parametrs.w = 80;
    SDL_RenderCopy(destination, source, &draw, &parametrs);
}

void WrapperSDL::putImage(int x, int y, SDL_Texture* source, SDL_Renderer* destination)
{
    parametrs.x = x;
    parametrs.y = y;
    SDL_QueryTexture(source, NULL, NULL, &parametrs.w, &parametrs.h);
    SDL_RenderCopy(destination, source, NULL, &parametrs);
}

void WrapperSDL::putImage(int x, int y, int h, int w, SDL_Surface* source, SDL_Surface* destination)
{
    parametrs.x = x;
    parametrs.y = y;
    parametrs.h = h;
    parametrs.w = w;
    SDL_BlitSurface(source, NULL, destination, &parametrs);
}

void WrapperSDL::putImage(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
    parametrs.x = x;
    parametrs.y = y;
    SDL_BlitSurface(source, NULL, destination, &parametrs);
}

void WrapperSDL::highlightField(field target, SDL_Renderer* destination)
{
    int x = target.fieldX*80;
    int y = target.fieldY*80;
    SDL_Rect shape = {x,y,80,80};
    SDL_SetTextureAlphaMod( highlight , 100 );
    SDL_RenderCopy(destination , highlight, NULL,&shape);
}

void WrapperSDL::highlightField(int x1, int y1, SDL_Renderer* destination)
{
    int x = x1*80;
    int y = y1*80;
    SDL_Rect shape = {x,y,80,80};
    SDL_SetTextureAlphaMod( highlight , 100 );
    SDL_RenderCopy(destination , highlight, NULL , &shape);
}

bool WrapperSDL::checkIfInRadius(field a, field b, int range)
{
    int test = (int) sqrt(
                          pow((a.fieldX - b.fieldX), 2) +
                          pow((a.fieldY - b.fieldY), 2)
                          );
    return test<range ? true : false ;
}

bool WrapperSDL::fieldsEquality(field a, field b)
{
    return (a.fieldX == b.fieldX) && (a.fieldY == b.fieldY) ? true : false ;
}

int* WrapperSDL::positionTokenizer ( string line )
{
	int positions[2];
	string locTokenized[2];
	char* temp = new char [line.length()+1];
        strcpy(temp, line.c_str());
        int token=0;
        for(int i=0;i<line.length();i++)
        {
            if(temp[i] != ',')
            {
                locTokenized[token]+=temp[i];
            }
           	else
            {
                token=1;
            }
        }
        positions[0] = atoi(locTokenized[0].c_str());
        positions[1] = atoi(locTokenized[1].c_str());
        delete(temp);
	return positions;
}

double WrapperSDL::calculateAngleInDegrees(double x1, double y1, double x2, double y2)
{
    if(x2==x1) {x2++;} //prevents /0;
    const double converter = 180.0 / 3.14159;
    if(x1 < x2)
        return atan( (y2-y1) / (x2-x1) ) * converter + 90.00;
    else
        return ( atan( (y2-y1) / (x2-x1) ) * converter ) + 270.00;
}

bool WrapperSDL::lineIntesectsSquare(field lineStart, field lineEnd, field squareLeftUpCorner, int squareHeight, int squareWidth) throw(string)
{
    ///Line
    double x1, x2, y1, y2;
    x1 = lineStart.fieldX;
    y1 = lineStart.fieldY;
    x2 = lineEnd.fieldX;
    y2 = lineEnd.fieldY;
    double lineAngle = calculateAngleInDegrees(x1, y1, x2, y2);

    ///Corners of enemy
    double RectX1, RectX2, RectY1, RectY2;
    RectX1 = squareLeftUpCorner.fieldX;
    RectX2 = squareLeftUpCorner.fieldX + squareWidth;
    RectY1 = squareLeftUpCorner.fieldY;
    RectY2 = squareLeftUpCorner.fieldY + squareHeight;

    ///Relationship: location of player and enemy
    bool above, below, onLeft, onRight;
    above = y1 < RectY1 ? true : false ;
    below = y1 > RectY2 ? true : false ;
    onLeft = x1 < RectX1 ? true : false ;
    onRight = x1 > RectX2 ? true : false ;

    /*  Test cases
    *   1   2   3
    *   4   E   5
    *   6   7   8
    */
    short testCase;

    if( !onRight && !onLeft )       //case 2 i 7
    {
        if( above )
        {
            testCase = 2;
        }
        else
            testCase = 7;
    }
    else if ( !above && !below )    //case 4 i 5
    {
        if(onLeft)
        {
            testCase = 4;
        }
        else
            testCase = 5;
    }
    else if ( onLeft )              //case 1 i 6
    {
        if(above)
        {
            testCase = 1;
        }
        if(below)
        {
            testCase = 6;
        }
    }
    else                            //case 3 i 8
    {
        if(above)
        {
            testCase = 3;
        }
        if(below)
        {
            testCase = 8;
        }
    }

    //Calculates angle between player and all sprite corners 
    double angleToCompare1, angleToCompare2;
    switch(testCase)
    {
        case 1: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX2, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX1, RectY2); break;
        case 2: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX1, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX2, RectY1); break;
        case 3: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX1, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX2, RectY2); break;
        case 4: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX1, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX1, RectY2); break;
        case 5: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX2, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX2, RectY2); break;
        case 6: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX1, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX2, RectY2); break;
        case 7: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX1, RectY2); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX2, RectY2); break;
        case 8: angleToCompare1 = calculateAngleInDegrees(x1, y1, RectX2, RectY1); angleToCompare2 = calculateAngleInDegrees(x1, y1, RectX1, RectY2); break;
        default: throw("Cos poszlo nie tak, obliczanie przypadku testowego. (przecinanie pola)");
    }

    //Comparing results
    if(abs(angleToCompare1 - angleToCompare2)<180)
    {
        if( angleToCompare1 < angleToCompare2)
            {
                if(lineAngle > angleToCompare1 && lineAngle < angleToCompare2)
                {
                    return true;
                }
            }
            else
            {
                if(lineAngle < angleToCompare1 && lineAngle > angleToCompare2)
                {
                    return true;
                }
            }
    }
    else
        if( lineAngle > 90 )
        {
            if(lineAngle > angleToCompare1 && lineAngle > angleToCompare2)
            {
                return true;
            }
        }
        else
        {
            if(lineAngle < angleToCompare1 && lineAngle < angleToCompare2)
            {
                return true;
            }
        }
    return false;
}


