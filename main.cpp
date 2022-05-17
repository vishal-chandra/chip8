#include <iostream>
#include <chrono>
#include "Chip8.h"
#include "Platform.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc != 4) {
        cerr << "Usage: " << argv[0] << " <scale> <delay> <ROM PATH>\n";
        exit(1);
    }

    int scale = stoi(argv[1]);
    int delay = stoi(argv[2]);

    Platform platform(
        "Emulator", 
        Chip8::DISPLAY_WIDTH * scale, 
        Chip8::DISPLAY_HEIGHT * scale, 
        Chip8::DISPLAY_WIDTH, 
        Chip8::DISPLAY_HEIGHT
    );

    Chip8 system;
    system.loadROM(argv[3]);

    //runtime vars
    int pitch = sizeof(system.display[0]) * Chip8::DISPLAY_WIDTH; //num bytes wide
    bool quit = false;
    auto lastTime = chrono::high_resolution_clock::now();

    while(!quit) {
        quit = platform.processKeys(system.keys);

        auto currentTime = chrono::high_resolution_clock::now();
        float diff = chrono::duration<float, chrono::milliseconds::period>(currentTime - lastTime).count();
        if(diff > delay) {
            lastTime = currentTime;
            //system.cycle();
            platform.update(system.display, pitch);
        }
    }
}