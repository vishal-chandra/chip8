#include "Chip8.h"
#include <fstream>

using namespace std;

void Chip8::loadROM(string filename) {

    //open the file as binary
    ifstream is(filename, ifstream::binary);

    //read ROM into buffer
    if(is) {

        //get length of file
        is.seekg(0, is.end);
        long length = is.tellg();
        is.seekg(0, is.beg);

        //allocate and fill buffer
        char* buffer = new char[length];
        is.read(buffer, length);

        //copy buffer to chip8 memory
        for(long i = 0; i < length; i++) {
            memory[MEM_START + i] = buffer[i];
        }

        //cleanup
        delete[] buffer;
        is.close();
    }
}

void Chip8::loadFont() {
    for(long i = 0; i < 80; ++i) {
        memory[FONT_START + i] = font[i];
    }
}