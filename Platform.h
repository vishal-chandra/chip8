#include <cstdint>
#include <SDL2/SDL.h>

class Platform {

    public:
    Platform();
    ~Platform();

    void update();
    void processKeys(bool* keys);
    bool quitPressed();

    private:
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;

};
