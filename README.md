# Chip8 Emulator

A basic emulator (interpreter) for the [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) system from the 1970s. <br>
Topics: Bitmasking, bytecode, low-level implementations of loops, functions, and instruction cycles. <br><br>

![pong](./demo/pong.mov) <br>
Pong running at 540Hz with level 3 antiflicker.

## Usage
`make` to compile, `./chip8 <scale> <clock speed> <antiflicker=2> <ROM PATH>` to run. <br>
The antiflicker parameter is optional and controls how many frames are stored and OR'd together when displaying. Higher levels of antiflicker will produce smoother video, but at the cost of runtime and motion blur.

## Testing
The makefile for this project is set up to automatically run all test ROMs in `./ROMs/tests` relative to the test binary. After one test window is closed, the next will open.

Current test catalog: <br>
- [BonCoder](https://github.com/cj1128/chip8-emulator/blob/master/rom/BC_test.ch8) <br>
- [Corax89](https://github.com/corax89/chip8-test-rom) <br>
- [IBM Logo](https://github.com/loktar00/chip8/blob/master/roms/IBM%20Logo.ch8) <br>
- [Mikolay Delay Test](https://github.com/loktar00/chip8/blob/master/roms/Delay%20Timer%20Test%20%5BMatthew%20Mikolay%2C%202010%5D.ch8) <br>
- [Metteo](https://github.com/metteo/chip8-test-rom) <br>
- [Particles](https://github.com/metteo/chip8-test-rom) <br>
- [Skosulor](https://github.com/Skosulor/c8int/tree/master/test) <br>
- [Timendus](https://github.com/Timendus/chip8-test-suite) <br>

## References

1. [Autin Morlan's Guide](https://austinmorlan.com/posts/chip8_emulator/) for SDL and Driver setup
2. [Tobias Langhoff's Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) for full system specs and opcode descriptions