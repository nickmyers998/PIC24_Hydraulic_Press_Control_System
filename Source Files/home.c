#define FCY 32000000UL
#define PWM_PERIOD 62500

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

//void home();


void home(){
    
    //INSERT HOME FUNCTION --- BRING PRESS TO MAXIMUM HEIGHT
    OC2_Start();
    OC1_Start();
    OC1R = 0xFFFF;
    OC2R = 0;
    __delay_ms(2000);
    OC1R = 0;
    return;
}
