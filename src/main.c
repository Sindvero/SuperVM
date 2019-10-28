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


uint16_t memory[UINT16_MAX];
uint16_t reg[COUNT];

uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }

    return x;
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


        switch (OP)
        {
        case ADD:
            break;
        
        case AND:
            break;
        
        case NOT:
            break;
        
        case BR:
            break;
        
        case LD:
            break;
        
        case ST:
            break;
        
        case JPR:
            break;
        
        case LDR:
            break;
        
        case STR:
            break;
        
        case LDI:
            break;
        
        case STI:
            break;
        
        case JMP:
            break;
        
        case LEA:
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