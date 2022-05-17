#include "Platform.h"

using namespace std;

Platform::Platform(
    char const * title, 
    int windowWidth, int windowHeight, 
    int imageWidth, int imageHeight
) {

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        imageWidth,
        imageWidth
    );
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(void const * pixels, int pitch) {
    SDL_UpdateTexture(texture, nullptr, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Platform::processKeys(bool * keys) {
    SDL_Event e;

    while(SDL_PollEvent(&e)) { //loop over all queued keypresses
        switch(e.type) {
            case SDL_QUIT:
                break;

            //we will flip the key value on change event
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                switch(e.key.keysym.scancode) {
                    case SDL_SCANCODE_X:
                        keys[0] = !keys[0];
                        break;
                    case SDL_SCANCODE_1:
                        keys[1] = !keys[1];
                        break;
                    case SDL_SCANCODE_2:
                        keys[2] = !keys[2];
                        break;
                    case SDL_SCANCODE_3:
                        keys[3] = !keys[3];
                        break;
                    case SDL_SCANCODE_Q:
                        keys[4] = !keys[4];
                        break;
                    case SDL_SCANCODE_W:
                        keys[5] = !keys[5];
                        break;
                    case SDL_SCANCODE_E:
                        keys[6] = !keys[6];
                        break;
                    case SDL_SCANCODE_A:
                        keys[7] = !keys[7];
                        break;
                    case SDL_SCANCODE_S:
                        keys[8] = !keys[8];
                        break;
                    case SDL_SCANCODE_D:
                        keys[9] = !keys[9];
                        break;
                    case SDL_SCANCODE_Z:
                        keys[0xA] = !keys[0xA];
                        break;
                    case SDL_SCANCODE_C:
                        keys[0xB] = !keys[0xB];
                        break;
                    case SDL_SCANCODE_4:
                        keys[0xC] = !keys[0xC];
                        break;
                    case SDL_SCANCODE_R:
                        keys[0xD] = !keys[0xD];
                        break;
                    case SDL_SCANCODE_F:
                        keys[0xE] = !keys[0xE];
                        break;
                    case SDL_SCANCODE_V:
                        keys[0xF] = !keys[0xF];
                        break;
                }
        }
    }
}