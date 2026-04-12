#include <SDL3/SDL.h>
#include <iostream>
#include "Body.h"
#include <vector>

void cleanup(SDL_Window* win, SDL_Renderer* ren){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    const float G = 5000.0f;

    SDL_Window* window = SDL_CreateWindow("Platformer", 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    bool running = true;
    SDL_Event event;

    Uint64 last = SDL_GetPerformanceCounter();

    std::vector<Body> bodies;
    bodies.emplace_back(640, 360, 200, 50, true);

    while(running){
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (double)(now - last) / SDL_GetPerformanceFrequency();
        last = now;
        if(dt > 0.05) dt = 0.05;
        // Updating
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT) running = false;
            if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    float mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    bodies.emplace_back(mouseX, mouseY, 50, 10, false);
                }
            }
        }

        for(auto& b : bodies) b.acc = Vector2D(0, 0);
        for(size_t i = 0; i < bodies.size(); i++){
            for(size_t j = 0; j < bodies.size(); j++){
                if(i == j) continue;
                if(bodies[i].isStar) continue;
                Vector2D direction = bodies[j].pos - bodies[i].pos;
                float dist = direction.mag();
                dist = std::max(dist, 5.0f);

                float forceMag = (G*bodies[i].mass*bodies[j].mass)/(dist*dist);
                Vector2D force = direction.normalize() * forceMag;

                bodies[i].acc = force/bodies[i].mass;
            }
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        for(auto& body : bodies){
            if(!body.isStar){
                body.update(dt);
            }
            body.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer);
    return 0;
}