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
#include <stdbool.h>


int ex = 0;
/*
long countert = 0;
int flag = 0;
int sec = 0;
int min = 0;
int hr = 0;
int hold = 0;
int holdhour = 0;
int holdmin = 0;

void irr4(void){
    ex = 1;
}

void irrtest(void){
    countert++;
    flag = 1;
}
*/
void irr4(void){
    ex = 1;
}
void endprocedure(int target, int time, int interval, int loadcell){
    
    // Runs at the end of a press run, ensuring press is reconfigured for another run
    
    
    
    /*
    TMR3_SetInterruptHandler(irrtest);
    TMR3_Start();
    lcd_ini();
    char hours[2];
    char minutes[2];
    char seconds[2];
    char timess[50];
    cursor_on();
    write_to_LCD("RunTime 00:00:00");
    
    
    while(1){
        
        while(flag == 0);
        while(flag == 1){
            
            hold = countert;
            
            hr = hold / 3600;
            int t1 = hr*3600;
            hold -= t1;
            min = hold / 60;
            int t2 = min*60;
            hold -= t2;
            
            if (hr == holdhour){
                if (min == holdmin){
                    sprintf(timess, "%-2.2u", hold);
                    shift_cursor_left(2);
                }
                else{
                    sprintf(timess, "%-2.2u:%-2.2u", min, hold);
                    shift_cursor_left(5);
                    holdmin++;
                    holdmin = holdmin % 60;
                }
            }
            else{
                sprintf(timess,"%-2.2u:%-2.2u:%-2.2u", hr, min, hold);
                shift_cursor_left(8);
                holdhour++;
            }
            write_to_LCD(timess);
            flag = 0;
        }
        
    }
    
    */
    
    
    HOME_BUTTON_SetInterruptHandler(irr4);
    INTERRUPT_GlobalEnable();
    lcd_ini();
    clear_LCD();
    write_to_LCD("Homing...");
    home();
    clear_LCD();
    write_to_LCD("Run finished. To");
    second_line();
    write_to_LCD("exit, press Home");
    while(ex == 0);
    return;
}
/*
void runtime(int totalseconds){
    int hold = totalseconds;
    char timess[50];
    int hr = 0;
    int min = 0;
    second_line();
    shift_cursor_right(16);
            
            hr = hold / 3600;
            int t1 = hr*3600;
            hold -= t1;
            min = hold / 60;
            int t2 = min*60;
            hold -= t2;
            
            if (hr == holdhour){
                if (min == holdmin){
                    sprintf(timess, "%-2.2u", hold);
                    shift_cursor_left(2);
                }
                else{
                    sprintf(timess, "%-2.2u:%-2.2u", min, hold);
                    shift_cursor_left(5);
                    holdmin++;
                    holdmin = holdmin % 60;
                }
            }
            else{
                sprintf(timess,"%-2.2u:%-2.2u:%-2.2u", hr, min, hold);
                shift_cursor_left(8);
                holdhour++;
            }
            write_to_LCD(timess);
            first_line();
}
*/