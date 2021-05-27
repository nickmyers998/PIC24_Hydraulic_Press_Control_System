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


unsigned int upcountr = 0;
unsigned int downcountr = 0;
unsigned int backcountr = 0;
unsigned int selectcountr = 0;
unsigned int menucountr = 1;
unsigned int totalroutines = 5;

void resetr(void);
void routines(void);
int checkloadcell(long loadc);

void upr(void){          //UP BUTTON INTERRUPT TO INCREASE FORCE ENTRY
    __delay_ms(10);
    if (upcountr != 2){
        upcountr = 1;
    }
    downcountr = 0;
    __delay_ms(10);
}

void downr(void){        //DOWN BUTTON INTERRUPT TO DECREASE FORCE ENTRY
    __delay_ms(10);
    if (downcountr != 2){
        downcountr = 1;
    }
    upcountr = 0;
    //TEST_LED_SetHigh();
    __delay_ms(10);
}

void backr(void){        //BACK BUTTON INTERRUPT TO RETURN FROM MENU
    __delay_ms(10);
    backcountr = 1;
    __delay_ms(10);
}

void selectr(void){      //SELECT BUTTON INTERRUPT TO ADVANCE MENU
    __delay_ms(10);
    selectcountr = 1;
    __delay_ms(10);
}

void routines(void){
    
    // This function allows users to select from a predetermined list of press
    // routines available by scrolling through the lcd.
    UP_BUTTON_SetInterruptHandler(upr);
    DOWN_BUTTON_SetInterruptHandler(downr);
    BACK_BUTTON_SetInterruptHandler(backr);
    SELECT_BUTTON_SetInterruptHandler(selectr);
    INTERRUPT_GlobalEnable();
    UP_BUTTON_SetDigitalInput();
    DOWN_BUTTON_SetDigitalInput();
    BACK_BUTTON_SetDigitalInput();
    SELECT_BUTTON_SetDigitalInput();
    
    char routlist[6][50] = {"Routine 0", "Routine 1", "Test Routine", "Routine 3", "Null Routine", " "};
    long routs[5][4] = { {200, 10000, 10, 500}, {25000, 10000, 10, 50000}, {17000, 10000 , 10, 25000}, {2560, 10000, 100, 5000}, {455, 10000, 100, 500} };
    int routlength[6] = {9, 9, 12, 9, 12, 1};
    int routnum = 0;
    
    long loadrout = 0;
    long totaltimerout = 0;
    long timeintrout = 0;
    long loadcellrout = 0;
    
    
    lcd_ini();
    clear_LCD();
    cursor_on();
    write_to_LCD(">>");
    write_to_LCD(routlist[0]);
    second_line();
    write_to_LCD(routlist[routnum+1]);
    first_line();
    cursor_on();
    shift_cursor_right(1);
    
    shift_cursor_left(9);
    
    while(1){
        __delay_ms(1);
        if (downcountr == 1){
            upcountr = 0;
            downcountr = 0;
            routnum++;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(routlist[routnum]);
            second_line();
            write_to_LCD(routlist[routnum+1]);
            first_line();
            cursor_on();
            shift_cursor_right(1);
            
        }
        else if (upcountr == 1){
            upcountr = 0;
            downcountr = 0;
            routnum--;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(routlist[routnum]);
            second_line();
            write_to_LCD(routlist[routnum+1]);
            cursor_on();
            shift_cursor_right(1);
        }
        else if (backcountr == 1){
            backcountr = 0;
            selectcountr = 0;
            menucountr--;
        }
        else if (selectcountr == 1){
            backcountr = 0;
            selectcountr = 0;
            menucountr++;
        }
        else{
            Nop();
        }
        
        if (routnum == 4){
            downcountr = 2;
        }
        else if (routnum == 0){
            upcountr = 2;
        }
        else{
            upcountr = 0;
            downcountr = 0;
        }
        
        if (menucountr == 2){
            menucountr = 1;
            
            loadrout = routs[routnum][0];
            totaltimerout = routs[routnum][1];
            timeintrout = routs[routnum][2];
            loadcellrout = routs[routnum][3];
            int check = checkloadcell(loadcellrout);
            if (check == 1){
                run(loadrout, totaltimerout, timeintrout, loadcellrout);
                return;
            }
            else {
                return;
            }
            
        }
        else if (menucountr == 0){
            menucountr = 1;
            return;
        }
        else{
            Nop();
        }
        
    }
}

void resetr(void){
    upcountr = 0;
    downcountr = 0;
    backcountr = 0;
    selectcountr = 0;
    menucountr = 1;
}

int checkloadcell(long loadc){
    // ensures user is selecting proper load cell for routine
    resetr();
    BACK_BUTTON_SetInterruptHandler(backr);
    SELECT_BUTTON_SetInterruptHandler(selectr);
    char s[4] = "";
    sprintf(s, "%-3.3lu", loadc);
    //__delay_ms(200);
    clear_LCD();
    write_to_LCD("Press SELECT if");
    second_line();
    write_to_LCD(s);
    write_to_LCD("lb LC used");
    resetr();
    __delay_ms(200);
    while(1){
        if (selectcountr == 1){
            backcountr = 0;
            selectcountr = 0;
            return 1;
        }
        else if (backcountr == 1){
            backcountr = 0;
            selectcountr = 0;
            return 0;
        }
        else{
            continue;
        }
    }
    return 2;
}