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

void CPU::write(uint8_t address, uint8_t value)
{
    memory[address] = value;
}

uint8_t CPU::read(uint8_t address)
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

        case 0x0C:  //INC (increment) the value of C
        {
            //increment number of cycles by 4
            INC_n(BC,4,false,false);
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

        case 0x1C:  //INC (increment) the value of E
        {
            //increment number of cycles by 4
            INC_n(DE,4,false,false);
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

        case 0x2C:  //INC (increment) the value of L
        {
            //increment number of cycles by 4
            INC_n(HL,4,false,false);
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

        case 0x34:  //INC (increment) the value of HL
        {
            //increment number of cycles by 12
            INC_n(HL,12,true,false);
        }

        case 0x3C:  //INC (increment) the value of A
        {
            //increment number of cycles by 4
            INC_n(AF,4,false,true);
            break;
        }

        case 0x47:  //LOAD value of A onto B
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,true); 
            break;
        }

        case 0x4F:  //LOAD value of A onto C
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,false); 
            break;
        }

        case 0x57:  //LOAD value of A onto D
        {
            //increment number of cycles by 4
            LD_n_A(DE,4,false,true); 
            break;
        }

        case 0x5F:  //LOAD value of A onto E
        {
            //increment number of cycles by 4
            LD_n_A(DE,4,false,false); 
            break;
        }
        
        case 0x67:  //LOAD value of A onto H
        {
            //increment number of cycles by 4
            LD_n_A(HL,4,false,true); 
            break;
        }

        case 0x6F:  //LOAD value of A onto L
        {
            //increment number of cycles by 4
            LD_n_A(BC,4,false,false); 
            break;
        }

        case 0x77:  //LOAD value of A onto HL
        {
            //increment number of cycles by 8
            LD_n_A(HL,8,true,false);
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
        uint16_t data = ((PC+2)<<8)|((PC+1));
        PC = PC + 3;
        //now load A to to the address in memory
        write(data, AF.high_byte);
        return;
    }
    PC = PC + 1;
    //check to see if you are combining using a combination of registers
    if(combined == true)
    {
        //now load A to the register
        reg.set_register_combined(AF.high_byte);
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
    //check to see if we are using a combined register
    if(combined == true)
    {
        reg.set_register_combined(reg.combined_register+1);
        
        //check to see if the result of the register after incrementing is zero
        if((reg.combined_register)==0)
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
        if(reg.combined_register==0x0010)
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

//16 BIT LOADS
//16 bit load opcode (0x01, 0x11, 0x21, 0x31)
void CPU::LD_n_nn(Register reg, int num_cycles)
{
    //uses 16 bits, so get the 16 bit data
    uint16_t data = ((PC+2)<<8)|((PC+1));
    PC = PC + 3;
    //now load data to the register
    reg.set_register_combined(data);
    cycles = cycles + num_cycles;
}     

//16 BIT ARITHMETIC
void CPU::INC_nn(Register reg, int num_cycles)
{
    PC = PC + 1;
    //increment reg
    reg .set_register_combined(reg.combined_register+1);
    //increment number of cycles
    cycles = cycles + num_cycles;   
}