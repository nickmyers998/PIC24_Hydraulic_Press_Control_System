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

struct Menu {
    char charnum[10];
    int inttens;
    int value;
       
};

void structtest(void){
    lcd_ini();
    clear_LCD();
    cursor_on();
    write_to_LCD("Testing123");
    __delay_ms(500);
    shift_cursor_left(3);
    write_to_LCD("X");
    while(1);
}
