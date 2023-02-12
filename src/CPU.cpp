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
    SP.set_register_combined(0xFFFE);    //at start-up, the program counter is set to 0xFFFE in ROM
    //now setting the values of the internal CPU registers (for the original GameBoy)
    AF.set_register_combined(0x01B0);
    BC.set_register_combined(0x0013);
    DE.set_register_combined(0x00D8);
    HL.set_register_combined(0x014D);
   
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

void CPU::write_2bytes(uint16_t address, uint16_t value)
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
    SP.set_register_combined(0xFFFE);    //at start-up, the program counter is set to 0xFFFE in ROM
    //now setting the values of the internal CPU registers (for the original GameBoy)
    AF.set_register_combined(0x01B0);
    BC.set_register_combined(0x0013);
    DE.set_register_combined(0x00D8);
    HL.set_register_combined(0x014D);

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
    opcode = PC;
    uint16_t data = 0;
    bool msb, combined;
    //this switch statement will perform the decoding and execution part of the opcode
    switch(opcode)
    {
        case 0x00:  //NOP instruction (meant to do nothing)
        {
            //increment number of cycles by 4
            NOP(4);
            break;
        }

        case 0x01:  //LOAD 16-bit data onto BC
        {
            //increment number of cycles by 12
            LD_n_nn(BC, 12);
            break;
        }

        case 0x02:  //LOAD value of A onto BC
        {
            //increment number of cycles by 8
            LD_n_A(BC,8,true,false);
            break;
        }

        case 0x03:  //INC (increment) the value of BC
        {
            //increment number of cycles by 8
            INC_nn(BC,8); 
            break;
        }

        case 0x04:  //INC (increment) the value of B
        {
            //increment number of cycles by 4
            INC_n(BC,4,false,true);
            break;
        }
        case 0x05:  //DEC (decrement) the value of B
        {
            //decrement number of cycles by 4
            DEC_n(BC,4,false,true);
            break;
        }
        
        case 0x08:  //LOAD value of SP to address pointed to by 2-byte value
        {
            //increment number of cycles by 20
            LD_n_nn(SP, 20, true);
            break;
        }

        case 0x0C:  //INC (increment) the value of C
        {
            //increment number of cycles by 4
            INC_n(BC,4,false,false);
        }
        case 0x0D:  //DEC (decrement) the value of C
        {
            //decrement number of cycles by 4
            DEC_n(BC,4,false,false);
            break;
        }

        case 0x11:  //LOAD 16-bit data onto DE
        {
            //increment number of cycles by 12
            LD_n_nn(DE,12);
            break;
        }

        case 0x12:  //LOAD value of A onto DE
        {
            //increment number of cycles by 8
            LD_n_A(DE,8,true,false);
            break;
        }

        case 0x13:  //INC (increment) the value of DE
        {
            //increment number of cycles by 8
            INC_nn(DE,8); 
            break;
        }

        case 0x14:  //INC (increment) the value of D
        {
            //increment number of cycles by 4
            INC_n(DE,4,false,true);
        }

        case 0x15:  //DEC (decrement) the value of D
        {
            //decrement number of cycles by 4
            DEC_n(DE,4,false,true);
            break;
        }

        case 0x1C:  //INC (increment) the value of E
        {
            //increment number of cycles by 4
            INC_n(DE,4,false,false);
        }

        case 0x1D:  //DEC (decrement) the value of E
        {
            //decrement number of cycles by 4
            DEC_n(DE,4,false,false);
            break;
        }

        case 0x21:  //LOAD 16-bit data onto HL
        {
            //increment number of cycles by 12 
            LD_n_nn(HL,12);
            break;
        }

        case 0x23:  //INC (increment) the value of HL
        {
            //increment number of cycles by 8
            INC_nn(HL,8); 
            break;
        }

        case 0x24:  //INC (increment) the value of H
        {
            //increment number of cycles by 4
            INC_n(HL,4,false,true);
        }
        case 0x25:  //DEC (decrement) the value of H
        {
            //decrement number of cycles by 4
            DEC_n(HL,4,false,true);
            break;
        }

        case 0x2C:  //INC (increment) the value of L
        {
            //increment number of cycles by 4
            INC_n(HL,4,false,false);
        }

        case 0x2D:  //DEC (decrement) the value of L
        {
            //decrement number of cycles by 4
            DEC_n(HL,4,false,false);
            break;
        }

        case 0x31:  //LOAD 16-bit data onto SP
        {
            //increment number of cycles by 12
            LD_n_nn(SP,12);
            break; 
        }

        case 0x33:  //INC (increment) the value of SP
        {
            //increment number of cycles by 8
            INC_nn(SP,8); 
            break;
        }

        case 0x34:  //INC (increment) the value of the address in HL
        {
            //increment number of cycles by 12
            INC_n(HL,12,true,false);
            break;
        }

        case 0x35:  //DEC (decrement) the value of the address in HL
        {
            //decrement number of cycles by 4
            DEC_n(HL,12,true,false);
            break;
        }

        case 0x36:  //LOAD the value of n into the address in HL
        {
            //increment number of cycles by 12
            //this won't do anything to AF, and no need to pass anything 
            //inisde of the rest of the parameters since 0x36 is a special case
            LD_r1_r2(AF, HL, 12, false, false, false, false);
            break;
        }
        
        case 0x3C:  //INC (increment) the value of A
        {
            //increment number of cycles by 4
            INC_n(AF,4,false,true);
            break;
        }

        case 0x3D:  //DEC (decrement) the value of A
        {
            //decrement number of cycles by 4
            DEC_n(AF,4,false,true);
            break;
        }

        case 0x40:  //LOAD the value of register B into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, BC, 4, false, true, false, true);
            break;
        }

        case 0x41:  //LOAD the value of register C into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, BC, 4, false, false, false, true);
            break;
        }

        case 0x42:  //LOAD the value of register D into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, BC, 4, false, true, false, true);
            break;
        }

        case 0x43:  //LOAD the value of register E into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, BC, 4, false, false, false, true);
            break;
        }

        case 0x44:  //LOAD the value of register H into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, BC, 4, false, true, false, true);
            break;
        }

        case 0x45:  //LOAD the value of register L into register B
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, BC, 4, false, false, false, true);
            break;
        }

        case 0x46:  //LOAD the value of the address pointed to by HL into register B
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, BC, 8, true, false, false, true);
            break;
        }

        case 0x47:  //LOAD value of A onto B
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,true); 
            break;
        }

        case 0x48:  //LOAD the value of register B into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, BC, 4, false, true, false, false);
            break;
        }

        case 0x49:  //LOAD the value of register C into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, BC, 4, false, false, false, false);
            break;
        }

        case 0x4A:  //LOAD the value of register D into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, BC, 4, false, true, false, false);
            break;
        }

        case 0x4B:  //LOAD the value of register E into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, BC, 4, false, false, false, false);
            break;
        }

        case 0x4C:  //LOAD the value of register H into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, BC, 4, false, true, false, false);
            break;
        }

        case 0x4D:  //LOAD the value of register L into register C
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, BC, 4, false, false, false, false);
            break;
        }

        case 0x4E:  //LOAD the value of the address pointed to by HL into register C
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, BC, 8, true, false, false, false);
            break;
        }

        case 0x4F:  //LOAD value of A onto C
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,false); 
            break;
        }

        case 0x50:  //LOAD the value of register B into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, DE, 4, false, true, false, true);
            break;
        }

        case 0x51:  //LOAD the value of register C into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, DE, 4, false, false, false, true);
            break;
        }

        case 0x52:  //LOAD the value of register D into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, DE, 4, false, true, false, true);
            break;
        }

        case 0x53:  //LOAD the value of register E into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, DE, 4, false, false, false, true);
            break;
        }

        case 0x54:  //LOAD the value of register H into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, DE, 4, false, true, false, true);
            break;
        }

        case 0x55:  //LOAD the value of register L into register D
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, DE, 4, false, false, false, true);
            break;
        }

        case 0x56:  //LOAD the value of the address pointed to by HL into register D
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, DE, 8, true, false, false, true);
            break;
        }

        case 0x57:  //LOAD value of A onto D
        {
            //increment number of cycles by 4
            LD_n_A(DE,4,false,true); 
            break;
        }

        case 0x58:  //LOAD the value of register B into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, DE, 4, false, true, false, false);
            break;
        }

        case 0x59:  //LOAD the value of register C into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, DE, 4, false, false, false, false);
            break;
        }

        case 0x5A:  //LOAD the value of register D into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, DE, 4, false, true, false, false);
            break;
        }

        case 0x5B:  //LOAD the value of register E into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, DE, 4, false, false, false, false);
            break;
        }

        case 0x5C:  //LOAD the value of register H into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, DE, 4, false, true, false, false);
            break;
        }

        case 0x5D:  //LOAD the value of register L into register E
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, DE, 4, false, false, false, false);
            break;
        }

        case 0x5E:  //LOAD the value of the address pointed to by HL into register E
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, DE, 8, true, false, false, false);
            break;
        }

        case 0x5F:  //LOAD value of A onto E
        {
            //increment number of cycles by 4
            LD_n_A(DE,4,false,false); 
            break;
        }

        case 0x60:  //LOAD the value of register B into register H
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, true, false, true);
            break;
        }

        case 0x61:  //LOAD the value of register C into register H
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, false, false, true);
            break;
        }

        case 0x62:  //LOAD the value of register D into register H
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, true, false, true);
            break;
        }

        case 0x63:  //LOAD the value of register E into register H
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, false, false, true);
            break;
        }

        case 0x64:  //LOAD the value of register H into register H
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, true, false, true);
            break;
        }

        case 0x65:  //LOAD the value of register L into registerH
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, false, false, true);
            break;
        }

        case 0x66:  //LOAD the value of the address pointed to by HL into register D
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, HL, 8, true, false, false, true);
            break;
        }

        case 0x67:  //LOAD value of A onto H
        {
            //increment number of cycles by 4
            LD_n_A(HL,4,false,true); 
            break;
        }

        case 0x68:  //LOAD the value of register B into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, true, false, false);
            break;
        }

        case 0x69:  //LOAD the value of register C into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, false, false, false);
            break;
        }

        case 0x6A:  //LOAD the value of register D into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, true, false, false);
            break;
        }

        case 0x6B:  //LOAD the value of register E into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, false, false, false);
            break;
        }

        case 0x6C:  //LOAD the value of register H into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, true, false, false);
            break;
        }

        case 0x6D:  //LOAD the value of register L into register L
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, false, false, false);
            break;
        }

        case 0x6E:  //LOAD the value of the address pointed to by HL into register L
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, HL, 8, true, false, false, false);
            break;
        }

        case 0x6F:  //LOAD value of A onto L
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,false); 
            break;
        }

        case 0x70:  //LOAD the value of register B into the address pointed to by HL
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, true, true, false);
            break;
        }

        case 0x71:  //LOAD the value of register C into the address pointed to by HL
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, HL, 4, false, false, true, false);
            break;
        }

        case 0x72:  //LOAD the value of register D into the address pointed to by HL
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, true, true, false);
            break;
        }

        case 0x73:  //LOAD the value of register E into the address pointed to by HL
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, HL, 4, false, false, false, true);
            break;
        }

        case 0x74:  //LOAD the value of register H into the address pointed to by HL
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, true, true, false);
            break;
        }

        case 0x75:  //LOAD the value of register L into the address pointed to by HL
        {    
            //increment number of cycles by 4
            LD_r1_r2(HL, HL, 4, false, false, true, false);
            break;
        }

        case 0x76:  // HALTs the processor operation until an interrupt occurs
        {
            //stop CPU operation until interrupt occurs
            HALT(4);
            break;
        }
        case 0x77:  //LOAD value of A onto HL
        {
            //increment number of cycles by 8
            LD_n_A(HL,8,true,false);
            break;
        }

        case 0x78:  //LOAD the value of register B into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, AF, 4, false, true, false, true);
            break;
        }

        case 0x79:  //LOAD the value of register C into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(BC, AF, 4, false, false, false, true);
            break;
        }

        case 0x7A:  //LOAD the value of register D into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, AF, 4, false, true, false, true);
            break;
        }

        case 0x7B:  //LOAD the value of register E into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(DE, AF, 4, false, false, false, true);
            break;
        }

        case 0x7C:  //LOAD the value of register H into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, AF, 4, false, true, false, true);
            break;
        }

        case 0x7D:  //LOAD the value of register L into register A
        {
            //increment number of cycles by 4
            LD_r1_r2(HL, AF, 4, false, false, false, true);
            break;
        }

        case 0x7E:  //LOAD the value of the address pointed to by HL into register A
        {
            //increment number of cycles by 8
            LD_r1_r2(HL, HL, 8, true, false, false, true);
            break;
        }

        case 0x7F:  //LOAD value of A onto A
        {
            //increment number of cycles by 4
            LD_n_A(AF,4,false,true);
            break;
        }

        case 0xEA:  //LOAD value of A onto a 2 byte immediate value
        {
            //won't do anything to AF
            //increment number of cycles by 16
            LD_n_A(AF, 16,true,false);
            break;
        }
    }
}

//delay opcode (0x00)
void CPU::NOP(int num_cycles)
{
    //only uses one byte, which is the opcode byte
    cycles = cycles + num_cycles;
    PC = PC + 1;
}

//8 BIT LOADS
//8 bit load opcode (0x7F, 0x47, 0x4F, 0x57, 0x5F, 0x67,
//0x6F, 0x02, 0x12, 0x77, 0xEA)
void CPU::LD_n_A(Register reg, int num_cycles, bool combined, bool msb)
{
    if (opcode==0xEA)
    {
        //uses 16 bits, so get the 16 bit data
        uint8_t temp_msb = read(PC+2);
        uint8_t temp_lsb = read(PC+1);
        uint16_t data = (temp_msb<<8)|(temp_lsb);
        PC = PC + 3;
        //now load A to to the address in memory
        write(data, AF.high_byte);
    }
    else
    {
        PC = PC + 1;
        //check to see if you are combining using a combination of registers
        if(combined == true)
        {
            //now load A to the address given by the value in the register
            write(reg.combined_register, AF.high_byte);
        }
        //if you are not, check to see if you are using the MSB or LSB of the register
        else
        {
            if(msb == true)
            {
                //now load A to the most significant byte
                reg.set_high_byte(AF.high_byte);    
            }
            else
            {
                //now load A to the least significant byte
                reg.set_low_byte(AF.high_byte);
            }
        }
    }
    //increment number of cycles
    cycles = cycles + num_cycles; 
}
//8 bit load opcodes (0x7F, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 
//0x7E, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x48, 0x49, 0x4A
//0x4B, 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56
//0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x61, 0x62, 0x63,
//0x64, 0x65, 0x66, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x70,
//0x71, 0x72, 0x73, 0x74, 0x75, 0x36)
void CPU::LD_r1_r2(Register src, Register dest, int num_cycles, bool src_comb, bool src_msb, bool dest_comb, bool dest_msb)    //put value r2 into r1 (A,B,C,D,E,H,L,HL)
{
    if(opcode == 0x36)
    {
        uint8_t data = read(PC+1);
        PC = PC + 2;
        //load the value of n into the address pointed to by HL
        write(dest.combined_register,data);
    }
    else
    {
        PC = PC + 1;
        //check to see if the source register is a pointer using HL
        if(src_comb == true)
        {
            //check to see if the destination msb was passed
            if(dest_msb == true)
            {
                dest.set_high_byte(read(src.combined_register));
            }
            //this is if the destination lsb was passed
            else
            {
                dest.set_low_byte(read(src.combined_register));
            }
        }
        //check to see if destination register is a pointer using HL
        else if(dest_comb == true)
        {
            //check to see if the source msb was passed
            if(src_msb == true)
            {
                write(dest.combined_register,src.high_byte);
            }
            //this is if the source lsb was passed
            else
            {
                write(dest.combined_register,src.low_byte);
            }   
        }
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}

//8 BIT ALU
//Increment opcode (0x3C, 0x04, 0x0c, 0x14, 
//0x1C, 0x24, 0x2C, 0x34)
void CPU::INC_n(Register reg, int num_cycles, bool combined, bool msb)
{
    PC = PC + 1;
    //reset the N flag
    AF.set_low_byte(AF.low_byte & CLR_SUBTRACTION_FLAG);

    //increment the register
    //check to see if we are using a pointer (HL)
    if(combined == true)
    {
        write(reg.combined_register, reg.combined_register+1);
        //check to see if the result after incrementing is zero
        if((read(reg.combined_register))==0)
        {
            //set the Z flag
            AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after incrementing
        if(read(reg.combined_register)==0x0010)
        {
            //set the H flag
            reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
        }        
    }
    //if not, just increment the register
    else
    {
        if(msb==true)    
        {
            reg.set_high_byte(reg.high_byte+1);
            
            //check to see if the result of the register after incrementing is zero
            if((reg.high_byte)==0)
            {
                //set the Z flag
                AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
            }
            //if it isn't 0, then clear the Z flag
            else
            {
                AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
            }

            //check to see if the result produces a half carry after incrementing
            if(reg.high_byte==0x10)
            {
                //set the H flag
                reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
            }
            //if it doesn't produce a half carry, clear the half carry flag
            else
            {
                reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
            }          
        }
        else
        {
            reg.set_low_byte(reg.low_byte+1);
            //check to see if the result of the register after incrementing is zero
            if((reg.low_byte)==0)
            {
                //set the Z flag
                AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
            }
            //if it isn't 0, then clear the Z flag
            else
            {
                AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
            }

            //check to see if the result produces a half carry after incrementing
            if(reg.low_byte==0x0010)
            {
                //set the H flag
                reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
            }
            //if it doesn't produce a half carry, clear the half carry flag
            else
            {
                reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
            }        
        }
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}


//Decrement opcode()
void CPU::DEC_n(Register reg, int num_cycles, bool combined, bool msb)
{
    PC = PC + 1;
    //reset the N flag
    AF.set_low_byte(AF.low_byte & SET_SUBTRACTION_FLAG);

    //decrement the register
    //check to see if we are using a pointer (HL)
    if(combined == true)
    {
        write(reg.combined_register, reg.combined_register-1);
        //check to see if the result after decrementing is zero
        if((read(reg.combined_register))==0)
        {
            //set the Z flag
            AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after decrementing
        if(read(reg.combined_register)==0x0010)
        {
            //set the H flag
            reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
        }        
    }
    //if not, just decrement the register
    else
    {
        if(msb==true)    
        {
            reg.set_high_byte(reg.high_byte-1);
            
            //check to see if the result of the register after decrementing is zero
            if((reg.high_byte)==0)
            {
                //set the Z flag
                AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
            }
            //if it isn't 0, then clear the Z flag
            else
            {
                AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
            }

            //check to see if the result produces a half carry after decrementing
            if(reg.high_byte==0x10)
            {
                //set the H flag
                reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
            }
            //if it doesn't produce a half carry, clear the half carry flag
            else
            {
                reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
            }          
        }
        else
        {
            reg.set_low_byte(reg.low_byte-1);
            //check to see if the result of the register after decrementing is zero
            if((reg.low_byte)==0)
            {
                //set the Z flag
                AF.set_low_byte(AF.low_byte|SET_ZERO_FLAG);
            }
            //if it isn't 0, then clear the Z flag
            else
            {
                AF.set_low_byte(AF.low_byte&CLR_ZERO_FLAG); 
            }

            //check to see if the result produces a half carry after decrementing
            if(reg.low_byte==0x0010)
            {
                //set the H flag
                reg.set_low_byte(AF.low_byte|SET_HALF_CARRY_FLAG);
            }
            //if it doesn't produce a half carry, clear the half carry flag
            else
            {
                reg.set_low_byte(AF.low_byte|CLR_HALF_CARRY_FLAG);
            }        
        }
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}


//16 BIT LOADS
//16 bit load opcode (0x01, 0x08, 0x11, 0x21, 0x31)
void CPU::LD_n_nn(Register reg, int num_cycles, bool pointer=false)
{
    //uses 16 bits, so get the 16 bit data
    uint8_t temp_msb = read(PC+2);
    uint8_t temp_lsb = read(PC+1);
    uint16_t data = (temp_msb<<8)|(temp_lsb);
    PC = PC + 3;
    //now load data to the register
    if (pointer == true)
    {
        write_2bytes(data,read(reg.combined_register));
    }
    else
    {
        reg.set_register_combined(data);
    }
    cycles = cycles + num_cycles;
}     

//16 BIT ARITHMETIC
//16 bit increment (0x03, 0x13, 0x23, 0x33)
void CPU::INC_nn(Register reg, int num_cycles)
{
    PC = PC + 1;
    //increment reg
    reg.set_register_combined(reg.combined_register+1);
    //increment number of cycles
    cycles = cycles + num_cycles;   
}

void CPU::HALT(int num_cycles)
{
    // halt CPU operation
    STOP_CPU =  true;
    NOP(num_cycles);
}