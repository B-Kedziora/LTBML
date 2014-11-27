#ifndef DESTRUCTIONSPRITE_H
#define DESTRUCTIONSPRITE_H
#include "WrapperSDL.h"

/**
*	Shows temporary effect on screen after killing enemy.
*/
class DestructionSprite
        {
            public:
                DestructionSprite( field place, SDL_Renderer* renderer);

                /**
                *	Render effect
                *	@param frames - how many frames left after creating effect.
                *	Determines which animation frame to show
                *	@param renderer - canvas reference
                */
                bool drawEffect( long frames, SDL_Renderer* renderer );
            private:
                static SDL_Texture* ghostDestructionTexture_;

                ///How long will effect be visible
                short leftTime_;

                ///Place to show effect
                field place_;
        };

#endif // DESTRUCTIONSPRITE_H
