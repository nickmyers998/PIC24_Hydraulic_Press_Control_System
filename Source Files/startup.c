#define FCY 32000000UL
#define PWM_PERIOD 62500

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
//#include "home.c"
#include <stdlib.h>
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

void startup(){
    
    lcd_ini();
    clear_LCD();
    write_to_LCD("Load: ");
    cursor_on();
    shift_cursor_right(6);
    ADC1_Enable();          //ENABLE ANALOG TO DIGITAL CONVERTER
    ADC1_ChannelSelect( TEST );
    int conversion = 0;
    
    char s[4];
    
    while(1){
        ADC1_SoftwareTriggerEnable();
        __delay_ms(10);
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(TEST));
        conversion = ADC1_ConversionResultGet(TEST);
        sprintf(s, "%-4.4u", conversion);
        write_to_LCD(s);
        shift_cursor_left(1);
        write_to_LCD("lbs");
        shift_cursor_left(8);
    }
    
}
