#include <cstdint>
#include <string>

class Chip8 {

    public:

    Chip8();

    void loadROM(std::string filename);

    private:

    void loadFont();

    //storage
    uint8_t registers[16];                  //operands
    uint8_t memory[4096]; 
    const unsigned int MEM_START = 0x200;   //beginning of usable memory
    const unsigned int FONT_START = 0x050;  //arbitrary beginning of font memory

    const unsigned int font[16 * 5] = {     //16 font sprites, 5 rows each
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    //execution
    uint16_t I;                             //line number oprand register
    uint16_t PC;                            //current line number
    uint16_t stack[16];                     //execution stack
    uint8_t sp;                             //stack pointer
    uint8_t delay_timer;                    //60Hz decrementing delay timer
    uint8_t sound_timer;                    //60Hz decrementing sound timer

    // I/O
    bool keys[16];                          //current user input
    uint32_t display[64][32];               //pixel data (using 32-bit for SDL)

    //instruction
    uint16_t opcode;                        //current instruction
};