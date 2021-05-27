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


void welcome(void){
    // Called on press startup
    // Initializes press punch
    
    lcd_ini();
    clear_LCD();
    write_to_LCD("Welcome!");
    second_line();
    write_to_LCD("Initializing...");
    home();
    clear_LCD();
    write_to_LCD("Ready!");
    __delay_ms(300);
    return;
    
}
