#include <iostream>
#include <chrono>
#include "Chip8.h"
#include "Platform.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        cerr << "Usage: " << argv[0] << " <scale> <clock speed> <antiflicker=2> <ROM PATH>\n";
        exit(1);
    }

    int scale = stoi(argv[1]);

    //timing
    int clock = stoi(argv[2]);
    int delay = 1000.0 / clock;
    int counter_delay = 16; //hardcoded at ~60Hz

    //I/O
    Chip8 system; //core
    if(argc == 4) {
        system.loadROM(argv[3]);
        system.setAntiflicker(2);
    }
    else {
        system.loadROM(argv[4]);
        system.setAntiflicker(stoi(argv[3]));
    }

    int pitch = sizeof(system.display[0]) * Chip8::DISPLAY_WIDTH; // num bytes wide
    Platform platform(
        "Emulator",
        Chip8::DISPLAY_WIDTH * scale,
        Chip8::DISPLAY_HEIGHT * scale,
        Chip8::DISPLAY_WIDTH,
        Chip8::DISPLAY_HEIGHT,
        pitch
    );

    // runtime vars, two separate "threads" for CPU and counters
    bool quit = false;
    auto lastTime = chrono::high_resolution_clock::now();
    auto lastCounterTime = chrono::high_resolution_clock::now();

    while (!quit) {
        quit = platform.processKeys(system.keys);

        auto currentTime = chrono::high_resolution_clock::now();
        float diff = chrono::duration<float, chrono::milliseconds::period>(currentTime - lastTime).count();
        float counterDiff = chrono::duration<float, chrono::milliseconds::period>(currentTime - lastCounterTime).count();

        if(diff > delay) {
            lastTime = currentTime;
            system.cycle();
            if (system.draw_flag) {
                uint32_t * disp = system.getBufferedDisplay();
                platform.update(disp);
                delete[] disp;
            }
        }
        if(counterDiff > counter_delay) {
            lastCounterTime = currentTime;
            system.decrement_sound();
            system.decrement_delay();
        }
    }
}