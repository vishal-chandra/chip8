# Chip8 Emulator

A basic emulator (interpreter) for the [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) system from the 1970s. <br>
Topics: Bitmasking, bytecode, low-level implementations of loops, functions, and instruction cycles.

## TODO
- [x] Add draw flag for display refresh
- [ ] Restrict display refresh to 60hz / add VBI
- [x] Change command line arg #3 to be clock speed
- [x] Add automatic testing instructions and test catalog
- [x] Investigate Metteo/Tronix test failure

## Testing
The makefile for this project is set up to automatically run all test ROMs in `./ROMs/tests` relative to the test binary. After one test window is closed, the next will open.

Current test catalog: <br>
-[BonCoder](https://github.com/cj1128/chip8-emulator/blob/master/rom/BC_test.ch8) <br>
-[Corax89](https://github.com/corax89/chip8-test-rom) <br>
-[IBM Logo](https://github.com/loktar00/chip8/blob/master/roms/IBM%20Logo.ch8) <br>
-[Mikolay Delay Test](https://github.com/loktar00/chip8/blob/master/roms/Delay%20Timer%20Test%20%5BMatthew%20Mikolay%2C%202010%5D.ch8) <br>
-[Metteo](https://github.com/metteo/chip8-test-rom) <br>
-[Particles](https://github.com/metteo/chip8-test-rom) <br>
-[Skosulor](https://github.com/Skosulor/c8int/tree/master/test) <br>
-[Timendus](https://github.com/Timendus/chip8-test-suite) <br>

## References

1. [Autin Morlan's Guide](https://austinmorlan.com/posts/chip8_emulator/) for SDL and Driver setup
2. [Tobias Langhoff's Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) for full system specs and opcode descriptions