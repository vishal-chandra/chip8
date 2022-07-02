# Chip8 Emulator

A basic emulator (interpreter) for the [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) system from the 1970s. <br>
Topics: Bitmasking, C++ `BitmaskType`, probability distributions in C++, bytecode and CPU instructions, clocking emulated CPUs, low-level implementations of loops and functions.

## TODO
- [x] Add draw flag for display refresh
- [ ] Restrict display refresh to 60hz
- [ ] Update timing command line arg to be clock speed
- [x] Add automatic testing instructions and test catalog
- [ ] Investigate Metteo/Tronix test failure

## Testing
The makefile for this project is set up to automatically run all test ROMs in `./ROMs/tests` relative to the test binary. After one test window is closed, the next will open.

Current test catalog:
-BonCoder
-Corax
-IBM Logo
-Mikolay Delay Test
-Metteo/Tronix
-Particles
-Skosulor
-Timendus

## References

1. [Autin Morlan's Guide](https://austinmorlan.com/posts/chip8_emulator/) for SDL and Driver setup
2. [Tobias Langhoff's Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) for full system specs and opcode descriptions