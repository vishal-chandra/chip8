#include "Platform.h"
#include <iostream>

using namespace std;

Platform::Platform(
    char const * title, 
    int windowWidth, int windowHeight, 
    int imageWidth, int imageHeight,
    int pitch
) : pitch(pitch)
{

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        imageWidth,
        imageHeight
    );
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(const void * pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::processKeys(bool * keys) {
    SDL_Event e;

    while(SDL_PollEvent(&e)) { //loop over all queued keypresses
        switch(e.type) {
            case SDL_QUIT:
                return true;

            case SDL_KEYDOWN:
                switch(e.key.keysym.scancode) {
                    case SDL_SCANCODE_X:
                        keys[0] = true;
                        break;
                    case SDL_SCANCODE_1:
                        keys[1] = true;
                        break;
                    case SDL_SCANCODE_2:
                        keys[2] = true;
                        break;
                    case SDL_SCANCODE_3:
                        keys[3] = true;
                        break;
                    case SDL_SCANCODE_Q:
                        keys[4] = true;
                        break;
                    case SDL_SCANCODE_W:
                        keys[5] = true;
                        break;
                    case SDL_SCANCODE_E:
                        keys[6] = true;
                        break;
                    case SDL_SCANCODE_A:
                        keys[7] = true;
                        break;
                    case SDL_SCANCODE_S:
                        keys[8] = true;
                        break;
                    case SDL_SCANCODE_D:
                        keys[9] = true;
                        break;
                    case SDL_SCANCODE_Z:
                        keys[0xA] = true;
                        break;
                    case SDL_SCANCODE_C:
                        keys[0xB] = true;
                        break;
                    case SDL_SCANCODE_4:
                        keys[0xC] = true;
                        break;
                    case SDL_SCANCODE_R:
                        keys[0xD] = true;
                        break;
                    case SDL_SCANCODE_F:
                        keys[0xE] = true;
                        break;
                    case SDL_SCANCODE_V:
                        keys[0xF] = true;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(e.key.keysym.scancode) {
                    case SDL_SCANCODE_X:
                        keys[0] = false;
                        break;
                    case SDL_SCANCODE_1:
                        keys[1] = false;
                        break;
                    case SDL_SCANCODE_2:
                        keys[2] = false;
                        break;
                    case SDL_SCANCODE_3:
                        keys[3] = false;
                        break;
                    case SDL_SCANCODE_Q:
                        keys[4] = false;
                        break;
                    case SDL_SCANCODE_W:
                        keys[5] = false;
                        break;
                    case SDL_SCANCODE_E:
                        keys[6] = false;
                        break;
                    case SDL_SCANCODE_A:
                        keys[7] = false;
                        break;
                    case SDL_SCANCODE_S:
                        keys[8] = false;
                        break;
                    case SDL_SCANCODE_D:
                        keys[9] = false;
                        break;
                    case SDL_SCANCODE_Z:
                        keys[0xA] = false;
                        break;
                    case SDL_SCANCODE_C:
                        keys[0xB] = false;
                        break;
                    case SDL_SCANCODE_4:
                        keys[0xC] = false;
                        break;
                    case SDL_SCANCODE_R:
                        keys[0xD] = false;
                        break;
                    case SDL_SCANCODE_F:
                        keys[0xE] = false;
                        break;
                    case SDL_SCANCODE_V:
                        keys[0xF] = false;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
    return false;
}