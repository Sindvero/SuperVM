// @uthor Aurelien

// A LC_3 VM

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>




typedef enum {
    BR = 0, //branch
    ADD,    //add 
    LD,     //load
    ST,     //store
    JPR,    //jump register
    AND,    //bitwise and
    LDR,    //load register
    STR,    //store register
    RTI,    //unused
    NOT,    //bitwise not
    LDI,    //load indirect
    STI,    //store indirect
    JMP,    //jump
    RES,    //reserved
    LEA,    //load effective address
    TRAP    //execute trap
} InstructionSet;

typedef enum {
    P = 1 << 0,  //Positive
    Z = 1 << 1,  //NULL
    N = 1 << 2   //Negative
} Flags;

typedef enum {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    PC,
    COND,
    COUNT
} Register;

enum {
    MR_KBSR = 0xFE00, //Keyboard status
    MR_KBDR = 0xFE02  //Keyboard data
};



uint16_t memory[UINT16_MAX];
uint16_t reg[COUNT];

uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }

    return x;
}

void mem_write(uint16_t address, uint16_t val) {
    memory[address] = val;
}

uint16_t mem_read(uint16_t address) {

    if (address == MR_KBSR){
        if (check_key()) {
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        } else {
            memory[MR_KBSR] = 0;
        }
        
    }

    return memory[address];
}

void update_flags(uint16_t r) {
    if (reg[r] == 0) {
        reg[COND] = Z;
    }

    else if (reg[r] >> 15){
        reg[COND] = N;
    }

    else {
        reg[COND] = P;
    }
}

int main(int argc, const char* argv[]){
    //Load argument


    //Setup


    //PC starting position
    enum{PC_start = 0x3000};
    reg[PC] = PC_start;


    int running = 1;
    while (running) {

        //Fetch instructions
        uint16_t instructions = mem_read(reg[PC]++);
        uint16_t OP = instructions >> 12;

        //DR
        uint16_t r0 = (instructions >> 9) & 0x07;
        //SR1
        uint16_t r1 = (instructions >> 6) & 0x07;
        //flag immediate mode ?
        uint16_t immediate_flag = (instructions >> 5) & 0x01;
        uint16_t immediate5 = sign_extend(instructions & 0x1F, 5);

        uint16_t r2 = instructions & 0x07;
        //PC offset
        uint16_t pc_offset = sign_extend(instructions & 0x1FF, 9);
        //condition flag
        uint16_t cond_flag = (instructions >> 9) & 0x07;


        switch (OP)
        {
        case ADD:
            if (immediate_flag) {
                immediate5 = sign_extend(instructions & 0x1F, 5);
                reg[r0] = reg[r1] + immediate5;
            } else {
                r2 = instructions & 0x07;
                reg[r0] = reg[r1] + reg[r2];
            }

            update_flags(r0);

            break;
        
        case AND:

            if (immediate_flag) {
                immediate5 = sign_extend(instructions & 0x1F, 5);
                reg[r0] = reg[r1] & immediate5;
            } else {
                r2 = instructions & 0x07;
                reg[r0] = reg[r1] & reg[r2];
            }

            update_flags(r0);
            break;
        
        case NOT:

            reg[r0] = -reg[r1];

            update_flags(r0);
            break;
        
        case BR:

            if (cond_flag & reg[COND]) {
                reg[PC] += pc_offset;
            }

            break;
        
        case LD:

            reg[r0] = mem_read(reg[PC] + pc_offset);
            update_flags(r0);
            break;
        
        case ST:

            mem_write(reg[PC] + pc_offset, reg[r0]);
            break;
        
        case JPR:
            uint16_t long_pc_offset = sign_extend(instructions & 0x7FF, 11);
            uint16_t long_flag = (instructions >> 11) & 1;

            reg[R7] = reg[PC];

            if (long_flag) {
                reg[PC] +=  long_pc_offset;
            } else {
                reg[PC] = reg[r1];
            }
            break;
        
        case LDR:

            uint16_t offset = sign_extend(instructions & 0x3F, 6);
            reg[r0] = mem_read(reg[r1] + offset);
            update_flags(r0);
            break;
        
        case STR:

            uint16_t offset = sign_extend(instructions & 0x3F, 6);
            mem_write(reg[r1] + offset, reg[r0]);
            break;
        
        case LDI:

            reg[r0] = mem_read( mem_read(reg[PC] + pc_offset) );
            update_flags(r0);

            break;
        
        case STI:

            mem_write(mem_read(reg[PC] + pc_offset), reg[r0]);
            break;
        
        case JMP:

            reg[PC] = reg[r1];

            break;
        
        case LEA:

            reg[r0] = reg[PC] + pc_offset;
            update_flags(r0);
            break;
        
        case TRAP:
            break;

        case RES:
        case RTI:
        default:
            abort();
            break;
        }


    }









}