#ifndef ENEMY_H
#define ENEMY_H

#include <Movable.h>
#include <DestructionSprite.h>

/**
*	Abstract class with common elements of phantoms and ghosts
*/
class Enemy : public Movable
{
    public:
        virtual ~Enemy();

        /**
        *	Clears vector when all effects finished displaying
        */
        static void clearDestructionVector();

        /**
        *	Adds effect after killing enemy
        *	@param renderer - canvas reference
        */
        void killed(SDL_Renderer* renderer)
        {
            effectVector_.push_back(new DestructionSprite(getPixelLocalization(), renderer));
        }

        /**
        *	Calls drawEffect() for each DestructionSprite in effectVector_;
        */
        static void displayDestruction(long frames, SDL_Renderer* renderer );

    protected:
        Enemy(int startX, int startY, string name, SDL_Renderer* renderer);

        /**
        *	Sets rush flag. Check whether to change movement type.
        */
        short rush(field HeroPosition, field ownPosition);
        bool rushFlag_;         //sprawia, ze normalny ruch zmienia sie w rush().
    private:
        static vector<DestructionSprite*> effectVector_;
};

#endif // ENEMY_H
