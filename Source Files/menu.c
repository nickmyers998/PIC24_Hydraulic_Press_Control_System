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


unsigned int upcountm = 0;
unsigned int downcountm = 0;
unsigned int backcountm = 0;
unsigned int selectcountm = 0;
unsigned int menucountm = 1;

void resetm(void);
int menu(void);

void upm(void){          //UP BUTTON INTERRUPT TO INCREASE FORCE ENTRY
    __delay_ms(10);
    if (upcountm != 2){
        upcountm = 1;
    }
    downcountm = 0;
    __delay_ms(10);
}

void downm(void){        //DOWN BUTTON INTERRUPT TO DECREASE FORCE ENTRY
    __delay_ms(10);
    if (downcountm != 2){
        downcountm = 1;
    }
    upcountm = 0;
    TEST_LED_SetHigh();
    __delay_ms(10);
}

void backm(void){        //BACK BUTTON INTERRUPT TO RETURN FROM MENU
    __delay_ms(10);
    backcountm = 1;
    __delay_ms(10);
}

void selectm(void){      //SELECT BUTTON INTERRUPT TO ADVANCE MENU
    __delay_ms(10);
    selectcountm = 1;
    __delay_ms(10);
}

int menu(void){
    
    // Allows user to select what mode to run the press in
    
    UP_BUTTON_SetInterruptHandler(upm);
    DOWN_BUTTON_SetInterruptHandler(downm);
    BACK_BUTTON_SetInterruptHandler(backm);
    SELECT_BUTTON_SetInterruptHandler(selectm);
    INTERRUPT_GlobalEnable();
    UP_BUTTON_SetDigitalInput();
    DOWN_BUTTON_SetDigitalInput();
    BACK_BUTTON_SetDigitalInput();
    SELECT_BUTTON_SetDigitalInput();
    resetm();
    
    char optionslist[4][50] = {"Manual Mode", "Routines", "Jog", " "};
    //int sizes[4] = {11, 8, 3, 1};
    int option = 0;
    
    lcd_ini();
    clear_LCD();
    cursor_on();
    write_to_LCD(">>");
    write_to_LCD(optionslist[0]);
    second_line();
    write_to_LCD(optionslist[option+1]);
    first_line();
    cursor_on();
    shift_cursor_right(1);
    
    
    
    while(1){
        __delay_ms(1);
        if (downcountm == 1){
            upcountm = 0;
            downcountm = 0;
            option++;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(optionslist[option]);
            second_line();
            write_to_LCD(optionslist[option+1]);
            first_line();
            cursor_on();
            shift_cursor_right(1);
            
        }
        else if (upcountm == 1){
            upcountm = 0;
            downcountm = 0;
            option--;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(optionslist[option]);
            second_line();
            write_to_LCD(optionslist[option+1]);
            cursor_on();
            shift_cursor_right(1);
        }
        else if (backcountm == 1){
            backcountm = 0;
            selectcountm = 0;
            menucountm--;
        }
        else if (selectcountm == 1){
            backcountm = 0;
            selectcountm = 0;
            menucountm++;
        }
        else{
            Nop();
        }
        
        if (option == 2){
            downcountm = 2;
        }
        else if (option == 0){
            upcountm = 2;
        }
        else{
            upcountm = 0;
            downcountm = 0;
        }
        
        if (menucountm == 2){
            menucountm = 1;
            return option;
        }
        else if (menucountm == 0){
            menucountm = 1;
            return 100;
        }
        else{
            Nop();
        }
        
    }
}

void resetm(void){
    upcountm = 0;
    downcountm = 0;
    backcountm = 0;
    selectcountm = 0;
    menucountm = 1;
}