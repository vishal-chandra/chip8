#include <cstdint>
#include <string>
#include <random>
#include <chrono>

class Chip8 {

    public:

    Chip8();
    void loadROM(std::string filename);

    // I/O
    bool keys[16];                                      //current user input
    static const int DISPLAY_WIDTH = 64;
    static const int DISPLAY_HEIGHT = 32;
    uint32_t display[DISPLAY_HEIGHT * DISPLAY_WIDTH];   //pixel data (using 32-bit for SDL)

    //called once per tick
    void cycle();

    private:

    //util
    void loadFont();
    uint8_t randByte();

    //OPERATIONS
    void OP_00E0();

    void OP_1NNN();

    void OP_6XNN();

    void OP_7XNN();

    void OP_ANNN();

    void OP_DXYN();

    // EMULATED COMPONENTS

    //storage
    uint8_t registers[16];                  //operands
    uint8_t memory[4096]; 
    const unsigned int MEM_START = 0x200;   //beginning of usable memory
    const unsigned int FONT_START = 0x050;  //arbitrary beginning of font memory
    const unsigned int FONT_SIZE = 16 * 5;  //16 font sprites, 5 rows each

    const unsigned int font[16 * 5] = {     
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
    //uint16_t stack[16];                     //execution stack
    //uint8_t sp;                             //stack pointer
    uint8_t delay_timer;                    //60Hz decrementing delay timer
    uint8_t sound_timer;                    //60Hz decrementing sound timer
    uint16_t opcode;                        //current instruction

    //random
    std::mt19937 rand;
    std::uniform_int_distribution<uint8_t> byte_distr;
};