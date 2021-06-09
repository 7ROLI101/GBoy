/*
HELPFUL LINKS:
https://gbdev.io/pandocs/
https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

*/
#include <cstdint>
#include <array>
using namespace std;

// here are the flags that are being used in the flag register
enum SET_FLAGS
{
    SET_ZERO_FLAG = (1<<7), //zero bit
    SET_SUBTRACTION_FLAG = (1<<6), //subraction bit (BCD)
    SET_HALF_CARRY_FLAG = (1<<5), //half carry bit (BCD)
    SET_CARRY_FLAG = (1<<4) //carry bit
};

enum CLR_FLAGS
{
    CLR_ZERO_FLAG = 0x7F, //zero bit
    CLR_SUBTRACTION_FLAG = 0xBF, //subraction bit (BCD)
    CLR_HALF_CARRY_FLAG = 0xDF, //half carry bit (BCD)
    CLR_CARRY_FLAG = 0xEF //carry bit
};

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

uint16_t SP, PC;
uint8_t A,B,C,D,E,F,H,L;
class Register
{
    public:
        uint16_t combined_register;
        uint8_t high_byte;
        uint8_t low_byte;

        void set_register_combined(uint16_t value)
        {
            combined_register = value;
            high_byte = (value&0xFF00)>>8;
            low_byte = (value&0x00FF);
        }
        void set_high_byte(uint8_t value)
        {
            high_byte = value;
            combined_register = (high_byte<<8) | (low_byte);
        }
        void set_low_byte(uint8_t value)
        {
            low_byte = value;
            combined_register = (high_byte<<8) | (low_byte);
        }
};

Register AF,BC,DE,HL,SP;

void emulate_one_cycle();   //used to emulate one CPU cycle
CPU();                      //constructor used to initialize an object of the CPU class
~CPU();                     //destructor used to destroy an object of the CPU class
void reset();               //used to reset the CPU if needed 
//dummy memory array, will replace later
array<uint8_t,0xFFFF> memory;
    private:
        bool CPU_running;   //meant to be used by the special commands like STOP and HALT
        unsigned int cycles;     //count the number of cycles the CPU has run for
        uint8_t read(uint16_t address);    //meant to read memory and other peripherals on the bus
        void write(uint16_t address, uint8_t value);   //meant to write to memory and other peripherals on the bus
        uint16_t opcode;
        // OPCODE instructions
        void NOP(int num_cycles);
        
        //8-bit loads
        void LD_n_A(Register reg, int num_cycles, bool combined, bool msb);  //put value of A into n (A,B,C,D,E,H,L,BC,DE,HL, or a two byte immediate value)
        void LD_r1_r2(Register src, Register dest, int num_cycles, bool src_comb, bool src_msb, bool dest_comb, bool dest_msb);    //put value of r1 into r2 (A,B,C,D,E,H,L,HL)
        //8-bit ALU
        void INC_n(Register reg, int num_cycles, bool combined, bool msb);   //increment nn (BC,DE,HL,SP)

        //16-bit loads
        void LD_n_nn(Register reg, int num_cycles); //put immediate value nn into register n (BC,DE,HL,SP)

        //16-bit arithmetic
        void INC_nn(Register reg, int num_cycles);  //increment nn (BC,DE,HL,SP)
};

