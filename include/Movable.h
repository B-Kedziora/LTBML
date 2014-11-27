#ifndef MOVABLE_H
#define MOVABLE_H
#include <WrapperSDL.h>

/**
*	Abstract class inherited by enemies and Hero class
*/
class Movable
{
    public:
        virtual ~Movable();
        field getField();
		field getPixelLocalization();

		/**
		*	Check whether object can move in given direction
		*/
        bool checkCanMove(short& direction, bool** accessible);

        /**
        *	Move object
        */
        virtual void go(bool** accessible, field HeroPosition);

        /**
        *	Render object
        *	@param renderer - canvas reference
        *	@param GlobalFrame - frames elapsed since game start. For animations
        */
        virtual void draw(SDL_Renderer* renderer, long& GlobalFrame);

        int localeX;
        int localeY;
    protected:
		Movable(int startX, int startY, string Name, SDL_Renderer* renderer);

		/**
		*	Move object with given speed
		*/
		void updatePosition(int direction, float speedMultiplier);
		void animate();

        SDL_Texture* imageTexture;
        SDL_Rect drawInfo;
        int animationFrames;
        int currentFrame;
        SDL_Rect drawDestination;
    private:
};

#endif // MOVABLE_H
