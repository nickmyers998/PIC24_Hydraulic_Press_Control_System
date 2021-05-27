#define FCY 32000000UL
#define PWM_PERIOD 62500


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <libpic30.h>
#include <math.h>
#include <xc.h>

void datasend(int load, int time){
    
    // Sends data to UART module through MPLABX built-in printf() function
    
    char s[6];
    char x[7] = "X";
    sprintf(s, "%-5.5u", load);
    strcat(x, s);
    printf(x);
}

void enddatasend(void){
    // Alerts Python script that the routine is terminated
    printf("Q");
}