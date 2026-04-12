#include <SDL3/SDL.h>
#include "Vector2D.h"

class Body{
    private:
    SDL_FRect rect;

    public:
    Vector2D pos;
    Vector2D vel;
    Vector2D acc;
    float mass;
    bool isStar;

    Body() : rect{0, 0, 10, 10} {}
    Body(float x, float y, float m, float r, bool isStar) : rect{x-r, y-r, r*2, r*2}, mass(m), isStar(isStar) {
        pos.x = rect.x;
        pos.y = rect.y;
    }

    void render(SDL_Renderer* ren){
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        SDL_RenderFillRect(ren, &rect);
    }

    void update(float dt){

        vel += acc * dt;
        pos += vel * dt;
        rect.x = pos.x;
        rect.y = pos.y;
    }
};