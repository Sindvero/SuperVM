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