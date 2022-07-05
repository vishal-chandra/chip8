#include "Chip8.h"
#include <fstream>
#include <iostream>

using namespace std;

Chip8::Chip8() 
    : rand(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
    //clear registers, keys, and display
    for(auto &reg : registers) {
        reg = 0;
    }
    for(auto &key : keys) {
        key = false;
    }
    OP_00E0();

    //init execution vars
    PC = MEM_START;
    byte_distr = std::uniform_int_distribution<uint8_t>(0, 255U);

    //load fonts
    for(unsigned int i = 0; i < FONT_SIZE; ++i) {
        memory[FONT_START + i] = font[i];
    }

    draw_flag = false;
    push_frame();
}

Chip8::~Chip8() {
    while(!past_frames.empty()) {
        pop_frame();
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

void Chip8::push_frame() {
    uint32_t * frame = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    memcpy(frame, display, sizeof(display));
    past_frames.push_front(frame);
}

void Chip8::pop_frame() {
    if(past_frames.size() > 0) {
        delete[] past_frames.back();
        past_frames.pop_back();
    }
}

void Chip8::decrement_sound() {
    if(sound_timer > 0) sound_timer--;
}

void Chip8::decrement_delay() {
    if(delay_timer > 0) delay_timer--;
}

uint32_t * Chip8::get_buffered_display() {
    uint32_t * buff = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    memcpy(buff, display, sizeof(display));

    for(auto& frame : past_frames) {
        for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; ++i) {
            buff[i] |= frame[i];
        }
    }

    return buff;
}

//MAIN SIMULATOR CALL
void Chip8::cycle() {

    //read next instruction and increment
    opcode = (memory[PC] << 8u) | memory[PC + 1];
    PC += 2;

    switch(opcode & 0xF000u) {
        case 0x0000:
            switch(opcode & 0x000Fu) {
                case 0x0:
                    OP_00E0();
                    break;
                case 0xE:
                    OP_00EE();
                    break;
            }
            break;
        case 0x1000:
            OP_1NNN();
            break;
        case 0x2000:
            OP_2NNN();
            break;
        case 0x3000:
            OP_3XNN();
            break;
        case 0x4000:
            OP_4XNN();
            break;
        case 0x5000:
            OP_5XY0();
            break;
        case 0x6000:
            OP_6XNN();
            break;
        case 0x7000:
            OP_7XNN();
            break;
        case 0x8000:
            switch(opcode & 0x000Fu) {
                case 0x0:
                    OP_8XY0();
                    break;
                case 0x1:
                    OP_8XY1();
                    break;
                case 0x2:
                    OP_8XY2();
                    break;
                case 0x3:
                    OP_8XY3();
                    break;
                case 0x4: 
                    OP_8XY4();
                    break;
                case 0x5:
                    OP_8XY5();
                    break;
                case 0x6:
                    OP_8XY6();
                    break;
                case 0x7:
                    OP_8XY7();
                    break;
                case 0xE:
                    OP_8XYE();
                    break;
            }
            break;
        case 0x9000:
            OP_9XY0();
            break;
        case 0xA000:
            OP_ANNN();
            break;
        case 0xB000:
            OP_BNNN();
            break;
        case 0xC000:
            OP_CXNN();
            break;
        case 0xD000:
            OP_DXYN();
            break;
        case 0xE000:
            switch(opcode & 0x000Fu) {
                case 0x1:
                    OP_EXA1();
                    break;
                case 0xE:
                    OP_EX9E();
                    break;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00FFu) {
                case 0x07:
                    OP_FX07();
                    break;
                case 0x0A:
                    OP_FX0A();
                    break;
                case 0x15:
                    OP_FX15();
                    break;
                case 0x18:
                    OP_FX18();
                    break;
                case 0x1E:
                    OP_FX1E();
                    break;
                case 0x29:
                    OP_FX29();
                    break;
                case 0x33:
                    OP_FX33();
                    break;
                case 0x55:
                    OP_FX55();
                    break;
                case 0x65:
                    OP_FX65();
                    break;
            }
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
    if(!(callstack.size() > 0)) return; //no subroutine to return from

    PC = callstack.top();
    callstack.pop();
}

//jump: load program counter (IBM)
void Chip8::OP_1NNN() {
    PC = opcode & 0x0FFFu;
}

//enter subroutine
void Chip8::OP_2NNN() {
    uint16_t SR_loc = opcode & 0x0FFFu;

    callstack.push(PC);
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
    uint16_t S = registers[X] + registers[Y];

    registers[0xF] = S > 255 ? 1 : 0;   //check for ovf
    registers[X] = S & 0x00FFu;         //truncate sum
}

//set VX = VX - VY with underflow detection
void Chip8::OP_8XY5() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[0xF] = (registers[X] > registers[Y]) ? 1 : 0;
    registers[X] -= registers[Y];
}

//VX = VY >> 1 and capture shifted bit
void Chip8::OP_8XY6() {
    uint8_t X = (opcode & 0x0F00u) >> 8;

    registers[0xF] = registers[X] & 0x01u; //capture shifted bit in VF
    registers[X] >>= 1;
}

//set VX = VY - VX with underflow detection
void Chip8::OP_8XY7() {
    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;

    registers[0xF] = (registers[Y] > registers[X]) ? 1 : 0;
    registers[X] = registers[Y] - registers[X];
}

//VX = VY << 1 and capture shifted bit
void Chip8::OP_8XYE() { 
    uint8_t X = (opcode & 0x0F00u) >> 8;

    //capture shifted bit in VF
    registers[0xF] = (registers[X] & 0x80u) >> 7; 
    registers[X] <<= 1;
}

//skip next instruction if VX != VY
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

    uint8_t random = randByte();
    registers[X] = random & NN;
}

//draw N-height sprite from I
//at (VX, VY) (IBM)
void Chip8::OP_DXYN() {

    //before drawing, save this frame
    pop_frame();
    push_frame();

    uint8_t X = (opcode & 0x0F00u) >> 8;
    uint8_t Y = (opcode & 0x00F0u) >> 4;
    uint8_t N = (opcode & 0x000Fu);

    //positions wrap, so mod
    uint8_t xLoc = registers[X] % DISPLAY_WIDTH;
    uint8_t yLoc = registers[Y] % DISPLAY_HEIGHT;
    
    //no collision to begin with
    registers[0xF] = 0;

    for(uint8_t row = 0; row < N; ++row) {
        uint8_t byte = memory[I + row]; //get byte

        //loop over byte
        for(int col = 0; col < 8; ++col) {

            //move the mask bit and remove rest
            uint8_t bit = byte & (0x80u >> col);
            unsigned int flattened_pos = ((xLoc + col) % DISPLAY_WIDTH)
            + ((yLoc + row) % DISPLAY_HEIGHT) * DISPLAY_WIDTH;

            if(bit) { //write changes if need to
                if(display[flattened_pos]) registers[0xF] = 1;
                display[flattened_pos] ^= 0xFFFFFFFF;
            }
        }
    }

    draw_flag = true;
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

//store delay timer in VX
void Chip8::OP_FX07() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    registers[X] = delay_timer;
}

//wait for key press and store in VX
void Chip8::OP_FX0A() {

    uint8_t X = (opcode & 0x0F00u) >> 8; 

    //search all keys for one that's on
    for(uint8_t key = 0; key < 16; ++key) {
        if(keys[key]) {
            registers[X] = key;
            return; //end operation
        }
    }

    PC -= 2; //loop instr if no keys pressed
}

//store VX in delay timer
void Chip8::OP_FX15() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    delay_timer = registers[X];
}

//store VX in sound timer
void Chip8::OP_FX18() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    sound_timer = registers[X];
}

//I += VX with overflow checking
void Chip8::OP_FX1E() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    I += registers[X];
    //registers[0xF] = (I > 0x0FFF) ? 1 : 0; //set ovf flag
}

//I = address of char in VX
void Chip8::OP_FX29() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    I = FONT_START + 5 * registers[X];
}

//hex to decimal conversion
void Chip8::OP_FX33() {
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t num = registers[X];

    memory[I + 2] = num % 10;
    num /= 10;
    memory[I + 1] = num % 10;
    num /= 10;
    memory[I]     = num % 10;
}

//load X registers to memory, starting at I
void Chip8::OP_FX55() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    for(uint8_t reg = 0; reg <= X; ++reg) {
        memory[I + reg] = registers[reg];
    }
}

//read X registers from memory, starting at I
void Chip8::OP_FX65() {
    uint8_t X = (opcode & 0x0F00) >> 8;

    for(uint8_t reg = 0; reg <= X; ++reg) {
        registers[reg] = memory[I + reg];
    }
}
