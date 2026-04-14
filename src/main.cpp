#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
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
    bodies.reserve(100);
    float req_vel = sqrt(G*200/100);
    bodies.emplace_back(540, 360, 10, 10, false, renderer, 0, req_vel);
    bodies.emplace_back(640, 360, 200, 50, true, renderer, 0, 0);

    bool spawning = false;
    float init_mouseX, init_mouseY;
    float vel_scale = 2.0f;

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
                    spawning = true;
                    SDL_GetMouseState(&init_mouseX, &init_mouseY);
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    float mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    bodies.emplace_back(mouseX, mouseY, 200, 50, true, renderer, 0, 0);
                }
            }
            if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    spawning = false;
                    float mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    float dx = mouseX-init_mouseX;
                    float dy = mouseY-init_mouseY;
                    float distance = sqrt(dx*dx + dy*dy);
                    if (distance > 0.1f) { 
                        float velX = -1*(dx * vel_scale);
                        float velY = -1*(dy * vel_scale);
                        bodies.emplace_back(init_mouseX, init_mouseY, 10, 10, false, renderer, velX, velY);
                    }
                    init_mouseX = 0;
                    init_mouseY = 0;
                }
            }
        }

        for(auto& b : bodies) b.acc = Vector2D(0, 0);
        for(size_t i = 0; i < bodies.size(); i++){
            for(size_t j = 0; j < bodies.size(); j++){
                if(i == j) continue;
                Vector2D direction = Vector2D((bodies[j].pos.x+bodies[j].radius) - (bodies[i].pos.x+bodies[i].radius), (bodies[j].pos.y+bodies[j].radius) - (bodies[i].pos.y+bodies[i].radius));
                float dist = direction.mag();
                dist = std::max(dist, 5.0f);

                float forceMag = (G*bodies[i].mass*bodies[j].mass)/(dist*dist);
                Vector2D force = direction.normalize() * forceMag;

                bodies[i].acc += force/bodies[i].mass;
            }
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        for(auto& body : bodies){
            body.update(dt);
            body.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer);
    return 0;
}