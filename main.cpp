#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <Board.h>
#include <SDL_ttf.h>

/*
#ifdef _WIN64
   #include <getUserWindows.cpp>
#elif _WIN32
   #include <getUserWindows.cpp>
#elif __linux
    // linux
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif
*/

int main( int argc, char* args[] )
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    cout << "SDL nie bangla: " << SDL_GetError() << endl;
    return -1;
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image nie bangla %s\n", IMG_GetError() );
    }

    if(TTF_Init()!=0)
    {
        printf("TTF nie bangla \n");
    }

    printf("Rdzeni: %d\n", SDL_GetCPUCount());
    printf("Size of int, short: %d %d \nSize of pointer: %d \n", sizeof(int), sizeof(short), sizeof(int*));
    //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    ///Board zajmuje sie obsluga calej gry
    Board* game = new Board();
 return 0;
}
