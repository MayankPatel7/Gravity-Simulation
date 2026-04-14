#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "Vector2D.h"

class Body{
    private:
    SDL_FRect rect;

    public:
    Vector2D pos;
    Vector2D vel;
    Vector2D acc;
    float mass;
    float radius;
    bool isStar;
    SDL_Texture* texture;

    Body() : rect{0, 0, 10, 10} {}
    Body(float x, float y, float m, float r, bool isStar, SDL_Renderer* ren, float initVelX, float initVelY):
        rect{x-r, y-r, r*2, r*2},
        mass(m),
        radius(r),
        isStar(isStar),
        texture(IMG_LoadTexture(ren, isStar ? "assets/star.png" : "assets/planet.png")),
        vel(initVelX, initVelY)

        {
            pos.x = rect.x;
            pos.y = rect.y;
        }

    void render(SDL_Renderer* ren){
        SDL_RenderTexture(ren, texture, NULL, &rect);
    }

    void update(float dt){
        if(isStar) return;
        vel += acc * dt;
        pos += vel * dt;
        rect.x = pos.x;
        rect.y = pos.y;
    }
};