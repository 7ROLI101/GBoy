/*
HELPFUL LINKS:
https://gbdev.io/pandocs/
*/
#include <cstdint>
using namespace std;
class CPU
{
    
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

In this case, I will treat them as their original 8 and 16 bits to keep the implementation true.
It could be potentially easier to just make them all 16 bits.
*/
uint8_t A,B,C,D,E,F,H,L;
uint16_t SP,PC;

};