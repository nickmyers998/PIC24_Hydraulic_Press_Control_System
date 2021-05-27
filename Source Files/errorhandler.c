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


void errorhandler(void){
    
    // Called when press experiences unexpected error
    lcd_ini();
    clear_LCD();
    write_to_LCD("ERROR: RESET NOW");
    estop();
    while(1);
    
}
