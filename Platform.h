#include <cstdint>
#include <SDL2/SDL.h>

class Platform {

    public:
    Platform(
        char const * title, 
        int windowWidth, int windowHeight, 
        int imageWidth, int imageHeight,
        int pitch
    );
    ~Platform();

    void update(void const * pixels);
    bool processKeys(bool* keys);

    private:
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;

    int pitch;
};
