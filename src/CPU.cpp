#include <CPU.h>

    //This function is the constructor used for initializing the CPU.
CPU::CPU()
{
    /*this is meant to simulate what the CPU does when powering up, after
    the internal ROM check passes. This is when the cartridge program executes.
    THIS IS TAKEN FROM SECTION 2.7.1(POWER UP SEQUENCE)
    */ 
    cycles = 0;     //setting the number of cycles to 0
    PC = 0x0100;    //at start-up, program counter is set to 0x0100 in ROM 
    SP = 0xFFFE;    //at start-up, the program counter is set to 0xFFFE in ROM
    //now setting the values of the internal CPU registers (for the original GameBoy)
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    // write out all of the values for the memory locations specified when powering up
    write(0xFF05,0x00); write(0xFF06,0x00); write(0xFF07,0x00); write(0xFF10,0x80); write(0xFF11,0xBF); write(0xFF12,0xF3);
    write(0xFF14,0xBF); write(0xFF16,0x3F); write(0xFF17,0x00); write(0xFF19,0xBF); write(0xFF1A,0x7F); write(0xFF1B,0xFF);
    write(0xFF1C,0x9F); write(0xFF1E,0xBF); write(0xFF20,0xFF); write(0xFF21,0x00); write(0xFF22,0x00); write(0xFF23,0xBF);
    write(0xFF24,0x77); write(0xFF25,0xF3); write(0xFF26,0xF1); write(0xFF40,0x91); write(0xFF42,0x00); write(0xFF43,0x00); 
    write(0xFF45,0x00); write(0xFF47,0xFC); write(0xFF48,0xFF); write(0xFF49,0xFF); write(0xFF4A,0x00); write(0xFF4B,0x00);
    write(0xFFFF,0x00);
    }

    //
void CPU::fetch()
{
    //this will get the instruction that needs to be run
    opcode = (PC<<12) | ((PC+1)<<8) | ((PC+2)<<4) | (PC+3);
    //increment the program counter to the next instruction to be executed
    PC = PC + 4;
}

void CPU::write(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

uint8_t CPU::read(uint16_t address)
{
    return memory[address];
}