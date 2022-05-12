#include <cstdint>
#include <stack>

class Chip8 {

    public:

    Chip8();

    void loadROM();

    private:

    uint8_t registers[16];      //operands
    uint8_t memory[4096];
    uint16_t index;             //line number oprand register
    uint16_t pc;                //current line number
    uint16_t stack[16];         //execution stack
    uint8_t sp;                 //stack pointer
    uint8_t delay_timer;        //60Hz decrementing delay timer
    uint8_t sound_timer;        //60Hz decrementing sound timer

    bool keys[16];              //current user input
    uint32_t display[64][32];   //pixel data

    uint16_t opcode;            //current instruction
};