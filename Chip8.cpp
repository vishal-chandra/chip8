#include "Chip8.h"
#include <fstream>

using namespace std;

Chip8::Chip8() 
    : rand(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
    //clear registers, keys, and display
    for(auto reg : registers) {
        reg = 0;
    }
    for(auto key : keys) {
        key = 0;
    }
    OP_00E0();

    //init execution vars
    PC = MEM_START;
    byte_distr = std::uniform_int_distribution<uint8_t>(0, 255U);

    //load fonts
    for(int i = 0; i < FONT_SIZE; ++i) {
        memory[FONT_START + i] = font[i];
    }
}

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
        for(long i = 0; i < length; ++i) {
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

uint8_t Chip8::randByte() {
    return byte_distr(rand);
}

void Chip8::setpx(int row, int col, bool val) {
    display[row][col] = val ? 0xFFFFFFFF : 0x00000000;
}

//OPERATIONS
//note: all bitshifts are in binary, so 4x num places in hex

//clear display
void Chip8::OP_00E0() {
    fill(display, display + 32 * 64, 0);
}

//load program counter
void Chip8::OP_1NNN() {
    PC = opcode & 0x0FFFu;
}

//load VX with NN
void Chip8::OP_6XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t NN = opcode & 0x00FFu;
    registers[X] = NN;
}

//add NN to VX
void Chip8::OP_7XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t NN = opcode & 0x00FFu;
    registers[X] += NN;
}

//load index register with NNN
void Chip8::OP_ANNN() {
    I = opcode & 0x0FFFu;
}

//draw N-height sprite from I
//at (x, y) = (VX, VY)
void Chip8::OP_DXYN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;
    uint8_t N = (opcode & 0x000Fu);

    //positions wrap, so mod
    uint8_t xLoc = registers[X] % 63;
    uint8_t yLoc = registers[Y] % 31;
    
    //no collision to begin with
    registers[0xF] = 0;

    for(uint8_t row = 0; row < N; ++row) {
        uint8_t byte = memory[I + row]; //get byte
        uint8_t mask = 0x80u;

        //loop over byte
        for(int pos = 0; pos < 8; ++pos) {

            //don't draw over the edge
            if(xLoc + pos > 31) break;

            //move the mask bit and AND for one bit
            uint8_t bit = (bool)(byte & (mask >> pos));

            //check bit against current display
            bool curr_disp = (bool) (display[xLoc + pos][yLoc]);
            bool disp_val = curr_disp != bit; //XOR
            registers[0xF] = curr_disp && bit; //VF shows sprite collision

            setpx(xLoc, yLoc, disp_val);
        }

        ++yLoc;
        if(yLoc > 63) return;
    }


}