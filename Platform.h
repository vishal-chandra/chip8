#include <cstdint>
#include <SDL2/SDL.h>

class Platform {

    public:
    Platform(
        char const * title, 
        int windowWidth, int windowHeight, 
        int imageWidth, int imageHeight
    );
    ~Platform();

    void update(void const * pixels, int pitch);
    void processKeys(bool* keys);

    private:
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;

};
