/*
HELPFUL LINKS:
https://gbdev.io/pandocs/
https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

*/
#include <cstdint>
#include <array>
using namespace std;

class CPU
{
    public:
        /*
    CPU REGISTERS
    There are two 16 bit registers, SP & PC (the stack pointer and program counter, respectively), and 
    there are 8 8-bit registers (A,B,C,D,E,F,H and L). However, some instructions can allow you to use
    them as 16 bit registers in this way.
              _____________________________
 BIT NO. -->  |     15-8    |   7-0        |
              |       A     |    F         |           
              |       B     |    C         |
              |       D     |    E         |
              |       H     |    L         |
              |_____________|______________|
-> The stack pointer tells us where we are on the stack, which is memory that is used for temporary storage
of data. You would have data added to the stack when doing things like going into functions, and data 
removed from the stack doing things like coming out of functions.

-> The program counter tells us what instruction is to be executed next. 

-> The A register is used as the accumulator, which is a special register used to store immediate values of 
data from operations temporarily.

-> The F register is a flag register used to give information about the status of the CPU.
Bit 7 of the F register -> Z, or the Zero Flag
Bit 6 of the F register -> N, or the Subtraction Flag (BCD)
Bit 5 of the F register -> H, or the Half Carry Flag (BCD)
Bit 4 of the F register -> C, or the Carry Flag

-> The other registers are general purpose register used to store data.

*/
uint16_t AF, BC, DE, HL, SP, PC;

// here are the flags that are being used in the flag register
enum FLAGS
{
    ZERO_FLAG = (1<<7), //zero bit
    SUBTRACTION_FLAG = (1<<6), //subraction bit (BCD)
    HALF_CARRY_FLAG = (1<<5), //half carry bit (BCD)
    CARRY_FLAG = (1<<4) //carry bit
};

void emulate_one_cycle();   //used to emulate one CPU cycle
CPU();                      //constructor used to initialize an object of the CPU class
~CPU();                     //destructor used to destroy an object of the CPU class
void reset();               //used to reset the CPU if needed 
//dummy memory array, will replace later
array<uint8_t,0xFFFF> memory;

    private:
        bool CPU_running;   //meant to be used by the special commands like STOP and HALT
        unsigned int cycles;     //count the number of cycles the CPU has run for
        uint8_t read(uint8_t address);    //meant to read memory and other peripherals on the bus
        void write(uint8_t address, uint8_t value);   //meant to write to memory and other peripherals on the bus
};