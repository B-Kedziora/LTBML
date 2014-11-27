#ifndef WrapperSDL_H
#define WrapperSDL_H

#include <SDL.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <SDL_image.h>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <SDL_ttf.h>
#include <exception>
#include <ctime>
#include <cmath>

using namespace std;

//Additional data types
struct hiScoreEntry
{
    string name;
    int time;
};

struct field
{
    int fieldX;
    int fieldY;
};

enum keyPressed
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LAMP,
    END
};

/**
*	Provides easy access to most commonly used SDL functions
*/
class WrapperSDL
{
    public:
        virtual ~WrapperSDL();

        static void SDL_Wrapper_init(SDL_Renderer*);
        static SDL_Surface* loadSingleImage(string name);

        static void putImage(int x, int y, int h, int w, SDL_Texture* source, SDL_Renderer* destination); //Texture
        static void putImage(int x, int y, SDL_Texture* source, SDL_Renderer* destination); //Texture

        static void putImage(int x, int y, int h, int w, SDL_Surface* source, SDL_Surface* destination); //Surface
        static void putImage(int x, int y, SDL_Surface* source, SDL_Surface* destination); //Surface

        static void highlightField(field target, SDL_Renderer* destination);
        static void highlightField(int x1, int y1, SDL_Renderer* destination);

        static void applyText(int x, int y, string text, SDL_Renderer* renderer, int fontSize);

        static bool checkIfInRadius(field a, field b, int range);

        static bool fieldsEquality (field a, field b);

        static int* positionTokenizer( string line );

        /**
        * Sprawdza czy linia przecina pole.
        * Wyznacza 2 krawedzie z pola, ktore powinny byc przeciete (widoczne z punktu widzenia bohatera).
        * Oblicza katy 2 koncowych punktów tej lamanej. Jesli kat lini zawiera sie pomiedzy nimi, wtedy przecina pole.
        */
        static bool lineIntesectsSquare(field lineStart, field lineEnd, field squareLeftUpCorner, int squareHeight, int squareWidth) throw(string);

        static double calculateAngleInDegrees(double x1, double y1, double x2, double y2);

    protected:
    private:
        static SDL_Surface* r;
        static TTF_Font* font;
        static TTF_Font* font2;
        static TTF_Font* font3;
        static SDL_Texture* highlight;
        static SDL_Color fontColor;
        static SDL_Rect parametrs;
        WrapperSDL();
};

#endif // WrapperSDL_H
