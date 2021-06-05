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

void CPU::write(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

uint8_t CPU::read(uint16_t address)
{
    return memory[address];
}

// this function is meant to copy the constructor of the CPU class
// This function will be called whenever a reset of the processor is needed.
void CPU::reset()
{
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
    //this is the function we will use to get the current instruction
void CPU::emulate_one_cycle()
{
    //this will get the instruction that needs to be run
    uint16_t opcode = PC;
    uint16_t data = 0;
    //this switch statement will perform the decoding and execution part of the opcode
    switch(opcode)
    {
        case 0x00:  //NOP instruction (meant to do nothing)
        {
            //only uses one byte, which is the opcode byte
            //increment number of cycles by 4
            cycles = cycles + 4;
            PC = PC + 1;
            break;
        }

        case 0x01:  //LOAD 16-bit data onto BC
        {
            //uses 16 bits, so get the 16 bit data
            data = ((PC+2)<<8)|((PC+1));
            PC = PC + 3;
            //now load to BC
            BC = data;
            //increment number of cycles by 12
            cycles = cycles + 12; 
            break;
        }

        case 0x02:  //LOAD value of A onto BC
        {
            PC = PC + 1;
            //now load A to BC
            BC = (AF >> 8);
            //increment number of cycles by 8
            cycles = cycles + 8; 
            break;
        }

        case 0x11:  //LOAD 16-bit data onto DE
        {
            //uses 16 bits, so get the 16 bit data
            data = ((PC+2)<<8)|((PC+1));
            PC = PC + 3;
            //now load to DE
            DE = data;
            //increment number of cycles by 12
            cycles = cycles + 12;
            break; 
        }

        case 0x12:  //LOAD value of A onto DE
        {
            PC = PC + 1;
            //now load A to DE
            DE = (AF >> 8);
            //increment number of cycles by 8
            cycles = cycles + 8; 
            break;
        }

        case 0x21:  //LOAD 16-bit data onto HL
        {
            //uses 16 bits, so get the 16 bit data
            data = ((PC+2)<<8)|((PC+1));
            PC = PC + 3;
            //now load to HL
            HL = data;
            //increment number of cycles by 12
            cycles = cycles + 12; 
            break;
        }

        case 0x31:  //LOAD 16-bit data onto SP
        {
            //uses 16 bits, so get the 16 bit data
            data = ((PC+2)<<8)|((PC+1));
            PC = PC + 3;
            //now load to SP
            SP = data;
            //increment number of cycles by 12
            cycles = cycles + 12;
            break; 
        }

        case 0x47:  //LOAD value of A onto B
        {
            PC = PC + 1;
            //now load A to B
            BC = (AF & 0xFF00) | (BC & 0x00FF);
            //increment number of cycles by 4
            cycles = cycles + 4; 
            break;

        }
        case 0x77:  //LOAD value of A onto HL
        {
            PC = PC + 1;
            //now load A to HL
            HL = (AF >> 8);
            //increment number of cycles by 8
            cycles = cycles + 8; 
            break;
        }

        case 0xEA:  //LOAD value of A onto a 2 byte immediate value
        {
            //uses 16 bits, so get the 16 bit data
            data = ((PC+2)<<8)|((PC+1));
            PC = PC + 3;
            //now load A to to the address in memory
            write(data,(AF >> 8));
            //increment number of cycles by 8
            cycles = cycles + 8; 
            break;
        }
    }
}