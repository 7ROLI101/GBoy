#include <CPU.h>

    //This function is the constructor used for initializing the CPU.
CPU::CPU()
{
    /*this is meant to simulate what the CPU does when powering up, after
    the internal ROM check passes. This is when the cartridge program executes.
    THIS IS TAKEN FROM SECTION 2.7.1(POWER UP SEQUENCE)
    */ 
    cycles = 0;     //setting the number of cycles to 0
    PC.SetRegisterValue(0x0100);    //at start-up, program counter is set to 0x0100 in ROM 
    SP.SetRegisterValue(0xFFFE);    //at start-up, the program counter is set to 0xFFFE in ROM
    //now setting the values of the internal CPU registers (for the original GameBoy)
    AF.SetRegisterValue(0x01B0);
    BC.SetRegisterValue(0x0013);
    DE.SetRegisterValue(0x00D8);
    HL.SetRegisterValue(0x014D);
   
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
    PC.SetRegisterValue(0x0100);    //at start-up, program counter is set to 0x0100 in ROM 
    SP.SetRegisterValue(0xFFFE);    //at start-up, the program counter is set to 0xFFFE in ROM
    //now setting the values of the internal CPU registers (for the original GameBoy)
    AF.SetRegisterValue(0x01B0);
    BC.SetRegisterValue(0x0013);
    DE.SetRegisterValue(0x00D8);
    HL.SetRegisterValue(0x014D);

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
    opcode = PC.GetRegisterValue();
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
            //increment number of cycles by 4
            DEC_n(BC,4,false,true);
            break;
        }

        case 0x06: //LOAD value of n to B
        {
            //increment number of cycles by 8
            LD_nn_n(B,8);
            break;
        }

        
        case 0x08:  //LOAD value of SP to address pointed to by 2-byte value
        {
            //increment number of cycles by 20
            LD_n_SP(SP, 20);
            break;
        }
        case 0x0A:  //LOAD the value of the address pointed to by BC into register A
        {
            //increment number of cycles by 8
            LD_r1_r2(BC, AF, 8, true, false, false, true);
            break;
        }
        case 0x0B:  //DEC (decrement) the value of BC
        {
            //increment number of cycles by 8
            DEC_nn(BC,8); 
            break;
        }
        case 0x0C:  //INC (increment) the value of C
        {
            //increment number of cycles by 4
            INC_n(BC,4,false,false);
        }
        case 0x0D:  //DEC (decrement) the value of C
        {
            //increment number of cycles by 4
            DEC_n(BC,4,false,false);
            break;
        }

        case 0x0E: //LOAD value of n to C
        {
            //increment number of cycles by 8
            LD_nn_n(C,8);
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
            //increment number of cycles by 4
            DEC_n(DE,4,false,true);
            break;
        }

        case 0x16: //LOAD value of n to D
        {
            //increment number of cycles by 8
            LD_nn_n(D,8);
            break;
        }

        case 0x1A:  //LOAD the value of the address pointed to by DE into register A
        {
            //increment number of cycles by 8
            LD_r1_r2(DE, AF, 8, true, false, false, true);
            break;
        }

        case 0x1B:  //DEC (decrement) the value of DE
        {
            //increment number of cycles by 8
            DEC_nn(DE,8); 
            break;
        }

        case 0x1C:  //INC (increment) the value of E
        {
            //increment number of cycles by 4
            INC_n(DE,4,false,false);
        }

        case 0x1D:  //DEC (decrement) the value of E
        {
            //increment number of cycles by 4
            DEC_n(DE,4,false,false);
            break;
        }
        case 0x1E: //LOAD value of n to E
        {
            //increment number of cycles by 8
            LD_nn_n(E,8);
            break;
        }

        case 0x21:  //LOAD 16-bit data onto HL
        {
            //increment number of cycles by 12 
            LD_n_nn(HL,12);
            break;
        }

        case 0x22: //LDI HL from A
        {
            LDI_LDD_A_HL_HL_A(AF,HL,8,true,true);
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
            //increment number of cycles by 4
            DEC_n(HL,4,false,true);
            break;
        }

        case 0x26: //LOAD value of n to H
        {
            //increment number of cycles by 8
            LD_nn_n(H,8);
            break;
        }

        case 0x2A: //LDI A from HL
        {
            LDI_LDD_A_HL_HL_A(HL,AF,8,false,true);
            break;
        }

        case 0x2B:  //DEC (decrement) the value of HL
        {
            //increment number of cycles by 8
            DEC_nn(HL,8); 
            break;
        }

        case 0x2C:  //INC (increment) the value of L
        {
            //increment number of cycles by 4
            INC_n(HL,4,false,false);
        }

        case 0x2D:  //DEC (decrement) the value of L
        {
            //increment number of cycles by 4
            DEC_n(HL,4,false,false);
            break;
        }

        case 0x2E: //LOAD value of n to L
        {
            //increment number of cycles by 8
            LD_nn_n(L,8);
            break;
        }

        case 0x31:  //LOAD 16-bit data onto SP
        {
            //increment number of cycles by 12
            LD_n_nn(SP,12);
            break; 
        }
        case 0x32: //LDI HL from A
        {
            LDI_LDD_A_HL_HL_A(AF,HL,8,true,false);
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
            //increment number of cycles by 4
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

        case 0x3A: //LDI A from HL
        {
            LDI_LDD_A_HL_HL_A(HL,AF,8,false,false);
            break;
        }

        case 0x3B:  //DEC (decrement) the value of SP
        {
            //increment number of cycles by 8
            DEC_nn(SP,8); 
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
            //increment number of cycles by 4
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
    PC.SetRegisterValue(PC.GetRegisterValue()+1);
}

//8 BIT LOADS
//8 bit load opcode 
// 0x02, 0x12, 0x47, 0x4F, 0x57, 0x5F,
// 0x67, 0x6F, 0x77, 0x7F, 0xEA

void CPU::LD_n_A(CombinedRegister reg, int num_cycles, bool combined, bool msb)
{
    if (opcode==0xEA)
    {
        //uses 16 bits, so get the 16 bit data
        uint8_t temp_msb = read(PC.GetRegisterValue()+2);
        uint8_t temp_lsb = read(PC.GetRegisterValue()+1);
        uint16_t data = (temp_msb<<8)|(temp_lsb);
        PC.SetRegisterValue(PC.GetRegisterValue() + 3);
        //now load A to to the address in memory
        write(data, A.GetRegisterValue());
    }
    else
    {
        PC.SetRegisterValue(PC.GetRegisterValue() + 1);
        //check to see if you are combining using a combination of registers
        if(combined == true)
        {
            //now load A to the address given by the value in the register
            write(reg.GetRegisterValue(), A.GetRegisterValue());
        }
        //if you are not, check to see if you are using the MSB or LSB of the register
        else
        {
            if(msb == true)
            {
                //now load A to the most significant byte
                reg.HighRegister->SetRegisterValue(A.GetRegisterValue());    
            }
            else
            {
                //now load A to the least significant byte
                reg.LowRegister->SetRegisterValue(A.GetRegisterValue());
            }
        }
    }
    //increment number of cycles
    cycles = cycles + num_cycles; 
}


//8 bit load opcodes
// 0x0A,
// 0x1A,
// 0x36
// 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
// 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
// 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 
// 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,0x7F

void CPU::LD_r1_r2(CombinedRegister src, CombinedRegister dest, int num_cycles, bool src_comb, bool src_msb, bool dest_comb, bool dest_msb)    //put value r2 into r1 (A,B,C,D,E,H,L,HL)
{
    if(opcode == 0x36)
    {
        uint8_t data = read(PC.GetRegisterValue()+1);
        PC.SetRegisterValue(PC.GetRegisterValue()+2) ;
        //load the value of n into the address pointed to by HL
        write(dest.GetRegisterValue(),data);
    }
    else
    {
        PC.SetRegisterValue(PC.GetRegisterValue() + 1);
        //check to see if the source register is a pointer
        if(src_comb == true)
        {
            //check to see if the destination msb was passed
            if(dest_msb == true)
            {
                dest.HighRegister->SetRegisterValue(read(src.GetRegisterValue()));
            }
            //this is if the destination lsb was passed
            else
            {
                dest.LowRegister->SetRegisterValue(read(src.GetRegisterValue()));
            }
        }
        //check to see if destination register is a pointer
        else if(dest_comb == true)
        {
            //check to see if the source msb was passed
            if(src_msb == true)
            {
                write(dest.GetRegisterValue(),src.HighRegister->GetRegisterValue());
            }
            //this is if the source lsb was passed
            else
            {
                write(dest.GetRegisterValue(),src.LowRegister->GetRegisterValue());
            }   
        }
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}

//8 bit load opcode 
// 0x06, 0x0E,
// 0x16, 0x1E, 
// 0x26, 0x2E
void CPU::LD_nn_n(Register dest,int num_cycles)
{   
    //get the 8 bit data that needs to be read in 
    uint8_t data=read(PC.GetRegisterValue()+1);
    //increment the program counter to prepare to read in the next opcode
    PC.SetRegisterValue(PC.GetRegisterValue()+2);
    //no flags need to be changed, so move on 
    //write the data to the register sent in
    dest.SetRegisterValue(data);
    //increment number of cycles
    cycles = cycles + num_cycles;
}
//8 bit load opcode 
// 0x22, 0x2A,
// 0x32, 0x3A
void CPU::LDI_LDD_A_HL_HL_A(CombinedRegister src, CombinedRegister dest,int num_cycles, bool dest_combined,bool increment)
{
    PC.SetRegisterValue(PC.GetRegisterValue()+1);
    // perform 0x22 and 0x2A operations
    if (increment==true)
    {
        // HL+ is the destination and A is the source
        if (dest_combined==true)
        {
            // get the value of A and write that to the address pointed to by HL
            write(dest.GetRegisterValue(),src.HighRegister->GetRegisterValue());
            // increment HL
            dest.SetRegisterValue(dest.GetRegisterValue()+1);
        }
        // A is the destination and HL+ is the source
        else
        {
            // get the value in the address of HL and write that to A
            write(dest.HighRegister->GetRegisterValue(),read(src.GetRegisterValue()));
            // increment HL
            src.SetRegisterValue(src.GetRegisterValue()+1);
        }
    }
    //perform 0x32 and 0x3A operations
    else
    {
        // HL+ is the destination and A is the source
        if (dest_combined==true)
        {
            // get the value of A and write that to the address pointed to by HL
            write(dest.GetRegisterValue(),src.HighRegister->GetRegisterValue());
            // decrement HL
            dest.SetRegisterValue(dest.GetRegisterValue()-1);
        }
        // A is the destination and HL+ is the source
        else
        {
            // get the value in the address of HL and write that to A
            write(dest.HighRegister->GetRegisterValue(),read(src.GetRegisterValue()));
            // increment HL
            src.SetRegisterValue(src.GetRegisterValue()-1);
        }
    }
    cycles = cycles + num_cycles;
}


//8 BIT ALU
// Increment opcode
//  0x04, 0x0C, 
//  0x14, 0x1C,  
//  0x24, 0x2C
//  0x34, 0x3C,  

void CPU::INC_n(CombinedRegister reg, int num_cycles, bool combined, bool msb)
{
    PC.SetRegisterValue(PC.GetRegisterValue()+1);
    //reset the N flag
    F.SetRegisterValue(F.GetRegisterValue() & CLR_SUBTRACTION_FLAG);

    //increment the register
    //check to see if we are using a pointer (HL)
    if(combined == true)
    {
        write(reg.GetRegisterValue(), reg.GetRegisterValue()+1);
        //check to see if the result after incrementing is zero
        if((read(reg.GetRegisterValue()))==0)
        {
            //set the Z flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after incrementing
        if(read(reg.GetRegisterValue())==0x0010)
        {
            //set the H flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()|CLR_HALF_CARRY_FLAG);
        }        
    }
    //if not, just increment the register
    else
    {
        Register* changed_reg;
        if (msb==true)
        {
            changed_reg = (reg.HighRegister);
        }
        else
        {
            changed_reg = (reg.LowRegister);
        }
            
        changed_reg->SetRegisterValue(changed_reg->GetRegisterValue()+1);
        //check to see if the result of the register after incrementing is zero
        if((changed_reg->GetRegisterValue())==0)
        {
            //set the Z flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after incrementing
        if(changed_reg->GetRegisterValue()==0x10)
        {
            //set the H flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()|CLR_HALF_CARRY_FLAG);
        }          
        
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}


//Decrement opcode
// 0x05, 0x0D,
// 0x15, 0x1D,
// 0x25, 0x2D,
// 0x35, 0x3D
void CPU::DEC_n(CombinedRegister reg, int num_cycles, bool combined, bool msb)
{
    PC.SetRegisterValue(PC.GetRegisterValue() + 1);
    //reset the N flag
    F.SetRegisterValue(F.GetRegisterValue() & SET_SUBTRACTION_FLAG);

    //decrement the register
    //check to see if we are using a pointer (HL)
    if(combined == true)
    {
        write(reg.GetRegisterValue(), reg.GetRegisterValue()-1);
        //check to see if the result after decrementing is zero
        if((read(reg.GetRegisterValue()))==0)
        {
            //set the Z flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after decrementing
        if(read(reg.GetRegisterValue())==0x0010)
        {
            //set the H flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()|CLR_HALF_CARRY_FLAG);
        }        
    }
    //if not, just decrement the register
    else
    {
        Register* changed_reg;
        if(msb==true)    
        {
            changed_reg = reg.HighRegister;
        }
        else
        {
            changed_reg = reg.LowRegister;
        }
        
        changed_reg->SetRegisterValue(changed_reg->GetRegisterValue()-1);
            
        //check to see if the result of the register after decrementing is zero
        if((changed_reg->GetRegisterValue())==0)
        {
            //set the Z flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_ZERO_FLAG);
        }
        //if it isn't 0, then clear the Z flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()&CLR_ZERO_FLAG); 
        }

        //check to see if the result produces a half carry after decrementing
        if(changed_reg->GetRegisterValue()==0x10)
        {
            //set the H flag
            F.SetRegisterValue(F.GetRegisterValue()|SET_HALF_CARRY_FLAG);
        }
        //if it doesn't produce a half carry, clear the half carry flag
        else
        {
            F.SetRegisterValue(F.GetRegisterValue()|CLR_HALF_CARRY_FLAG);
        }          
    }
    //increment number of cycles 
    cycles = cycles + num_cycles; 
}


//16 BIT LOADS
//16 bit load opcode 
// 0x01, 
// 0x11, 
// 0x21,
// 0x31
template <typename t>
void CPU::LD_n_nn(t reg, int num_cycles)
{
    //uses 16 bits, so get the 16 bit data
    uint8_t temp_msb = read(PC.GetRegisterValue()+2);
    uint8_t temp_lsb = read(PC.GetRegisterValue()+1);
    uint16_t data = (temp_msb<<8)|(temp_lsb);
    PC.SetRegisterValue(PC.GetRegisterValue() + 3);
    //now load data to the register
    reg.SetRegisterValue(data);
    cycles = cycles + num_cycles;
}     

//16 bit load opcode (0x08)
void CPU::LD_n_SP(SpecialRegister reg, int num_cycles)
{
    //uses 16 bits, so get the 16 bit data
    uint8_t temp_msb = read(PC.GetRegisterValue()+2);
    uint8_t temp_lsb = read(PC.GetRegisterValue()+1);
    uint16_t address = (temp_msb<<8)|(temp_lsb);
    PC.SetRegisterValue(PC.GetRegisterValue() + 3);    
    //now load data to the register
    write_2bytes(address,reg.GetRegisterValue());
    reg.SetRegisterValue(address);
    cycles = cycles + num_cycles;
}     

//16 BIT ARITHMETIC
//16 bit increment 
// 0x03, 
// 0x13, 
// 0x23, 
// 0x33
template <typename t>
void CPU::INC_nn(t reg, int num_cycles)
{
    PC.SetRegisterValue(PC.GetRegisterValue()+1);
    //increment reg
    reg.SetRegisterValue(reg.GetRegisterValue()+1);
    //increment number of cycles
    cycles = cycles + num_cycles;   
}

//16 bit decrement 
// 0x0B, 
// 0x1B, 
// 0x2B, 
// 0x3B
template <typename t>
void CPU::DEC_nn(t reg, int num_cycles)
{
    PC.SetRegisterValue(PC.GetRegisterValue()+1);
    //decrement reg
    reg.SetRegisterValue(reg.GetRegisterValue()-1);
    //increment number of cycles
    cycles = cycles + num_cycles;   
}

void CPU::HALT(int num_cycles)
{
    // halt CPU operation
    STOP_CPU =  true;
    NOP(num_cycles);
}