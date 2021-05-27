#define FCY 32000000UL
#define PWM_PERIOD 62500

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

int h = 0;

void irr2(void){
    
    home();
    h = 1;
}

void estop(void){
    
    // Immediately terminates press run
    
    HOME_BUTTON_SetInterruptHandler(irr2);
    INTERRUPT_GlobalEnable();
    
    OC2_Start();            //ENABLE OSCILLATOR 2
    OC1_Start();            //ENABLE OSCILLATOR 1
    OC2R = 0;
    OC1R = 0;
    lcd_ini();
    write_to_LCD("E-STOP ACTIVATED");
    second_line();
    write_to_LCD("HOME WHEN READY");
    while(h == 0);
    return;
}
