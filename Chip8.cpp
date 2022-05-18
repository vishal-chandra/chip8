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
    for(unsigned int i = 0; i < FONT_SIZE; ++i) {
        memory[FONT_START + i] = font[i];
    }
}

//UTIL Functions
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

//MAIN SIMULATOR CALL
void Chip8::cycle() {

    //read next instruction and increment
    opcode = (memory[PC] << 8u) | memory[PC + 1];
    PC += 2;

    //update counters
    if(sound_timer > 0) sound_timer--;
    if(delay_timer > 0) delay_timer--;

    switch(opcode & 0xF000u) {
        case 0x0000:
            OP_00E0();
            break;
        case 0x1000:
            OP_1NNN();
            break;
        case 0x2000:
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            OP_6XNN();
            break;
        case 0x7000:
            OP_7XNN();
            break;
        case 0x8000:
            break;
        case 0x9000:
            break;
        case 0xA000:
            OP_ANNN();
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            OP_DXYN();
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
    }

}

//OPERATIONS
//note: all bitshifts are in binary, so 4x num places in hex
//oparations used in the IBM test ROM are denoted by (IBM)

//clear display (IBM)
void Chip8::OP_00E0() {
    fill(display, display + DISPLAY_HEIGHT * DISPLAY_WIDTH, 0);
}

//return from subroutine
void Chip8::OP_00EE() {
    if(!callstack.size() > 0) return; //no subroutine to return from

    PC = callstack.top();
    callstack.pop();
}

//load program counter (IBM)
void Chip8::OP_1NNN() {
    PC = opcode & 0x0FFFu;
}

//enter subroutine
void Chip8::OP_2NNN() {
    uint16_t SR_loc = opcode & 0x0FFFu;

    callstack.push(SR_loc);
    PC = SR_loc;
}

//skip if register == X
void Chip8::OP_3XNN() {
    uint8_t X =  (opcode & 0x0F00u) >> 8;
    uint8_t NN = (opcode & 0x00FFu);

    if(registers[X] == NN) PC += 2;
}

//skip if register != X
void Chip8::OP_4XNN() {
    uint8_t X =  (opcode & 0x0F00u) >> 8;
    uint8_t NN = (opcode & 0x00FFu);

    if(registers[X] != NN) PC += 2;
}

//skip if registers are equal
void Chip8::OP_5XY0() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    if(registers[X] == registers[Y]) PC += 2;
}

//load VX with NN (IBM)
void Chip8::OP_6XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t NN = opcode & 0x00FFu;
    registers[X] = NN;
}

//add NN to VX (IBM)
void Chip8::OP_7XNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t NN = opcode & 0x00FFu;
    registers[X] += NN;
}

//set VX = VY
void Chip8::OP_8XY0() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[X] = registers[Y];
}

//set VX |= VY
void Chip8::OP_8XY1() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[X] |= registers[Y];
}

//set VX &= VY
void Chip8::OP_8XY2() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[X] &= registers[Y];
}

//set VX ^= VY
void Chip8::OP_8XY3() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[X] ^= registers[Y];
}

//set VX += VY with overflow detection
void Chip8::OP_8XY4() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;
    uint16_t S = X + Y;

    registers[0xF] = S > 255u ? 1 : 0;  //check for ovf
    registers[X] = S & 0x00FFu;         //truncate sum
}

//set VX = VX - VY with underflow detection
void Chip8::OP_8XY5() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[0xF] = (X > Y) ? 1 : 0;
    registers[X] -= registers[Y];
}

void Chip8::OP_8XY6() {
    uint8_t X = (opcode & 0x0F00u) >> 8;

    registers[0xF] = registers[X] & 0x01u; //capture shifted bit in VF
    registers[X] >>= 1;
}

//set VX = VY - VX with underflow detection
void Chip8::OP_8XY7() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[0xF] = (Y > X) ? 1 : 0;
    registers[X] = registers[Y] - registers[X];
}

void Chip8::OP_8XYE() { uint8_t X = (opcode & 0x0F00u) >> 8;

    //capture shifted bit in VF
    registers[0xF] = (registers[X] & 0x80u) >> 7; 
    registers[X] <<= 1;
}

void Chip8::OP_9XY0() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    if(registers[X] != registers[Y]) PC += 2;
}

//load index register with NNN (IBM)
void Chip8::OP_ANNN() {
    I = (opcode & 0x0FFFu);
}

//jump to instruction NNN + V0
void Chip8::OP_BNNN() {
    uint16_t NNN = opcode & 0x0FFFu;
    PC = NNN + registers[0];
}

//set VX = random & NN
void Chip8::OP_CXNN() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t NN = opcode & 0x00FFu;

    registers[X] = randByte() & NN;
}

//draw N-height sprite from I
//at (VX, VY) (IBM)
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
            if(xLoc + pos > 63) break;

            //move the mask bit and AND for one bit
            bool bit = (bool)(byte & (mask >> pos));

            //check bit against current display
            unsigned int flattened_pos = (xLoc + pos) + (yLoc * DISPLAY_WIDTH);
            bool curr_disp = (bool) display[flattened_pos];
            bool disp_val = curr_disp != bit;   //XOR
            registers[0xF] = curr_disp && bit; //VF shows sprite collision

            //write changes
            display[flattened_pos] = (disp_val ? 0xFFFFFFFF : 0x00000000);
        }

        ++yLoc;
        if(yLoc > 63) return;
    }
}

//skip next instruction if Key VX is pressed
void Chip8::OP_EX9E() {
    uint8_t X = (opcode & 0x0F00u) >> 8;

    if(keys[registers[X]]) PC += 2;
}

//skip next instruction if Kex VX is not pressed
void Chip8::OP_EXA1() {
    uint8_t X = (opcode & 0x0F00u) >> 8;

    if(!keys[registers[X]]) PC += 2;
}