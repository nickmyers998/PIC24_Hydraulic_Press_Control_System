#define FCY 32000000UL
#define PWM_PERIOD 62500
#define constant_lc 50000

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

unsigned int lctestget(void);

unsigned int upcountc = 0;
unsigned int downcountc = 0;
unsigned int backcountc = 0;
unsigned int selectcountc = 0;
unsigned int leftcountc = 0;
unsigned int rightcountc = 0;
unsigned int menucountc = 1;
//const unsigned long t = 50000;
unsigned int cellvals[4] = {500, 5000, 20000, 30000};

void upc(void){          //UP BUTTON INTERRUPT TO INCREASE FORCE ENTRY
    //__delay_ms(10);
    if (upcountc != 2){
        upcountc = 1;
    }
    downcountc = 0;
    //__delay_ms(10);
}

void downc(void){        //DOWN BUTTON INTERRUPT TO DECREASE FORCE ENTRY
    //__delay_ms(10);
    if (downcountc != 2){
        downcountc = 1;
    }
    upcountc = 0;
    //__delay_ms(10);
}

void leftc(void){        //LEFT BUTTON INTERRUPT TO SHIFT CURSOR LEFT
    //__delay_ms(10);
    if (leftcountc != 2){
        leftcountc = 1;
    }
    //__delay_ms(10);
}

void rightc(void){       //RIGHT BUTTON INTERRUPT TO SHIFT CURSOR RIGHT
    //__delay_ms(10);
    if (rightcountc != 2){
        rightcountc = 1;
        upcountc = 0;
        downcountc = 0;
    }
    //__delay_ms(10);
}

void backc(void){        //BACK BUTTON INTERRUPT TO RETURN FROM MENU
    //__delay_ms(10);
    backcountc = 1;
    //__delay_ms(10);
}

void selectc(void){      //SELECT BUTTON INTERRUPT TO ADVANCE MENU
    //__delay_ms(10);
    selectcountc = 1;
    //__delay_ms(10);
}


/*
int getloadcell(void){
    
    
   
    int conversion = 0;
    LOAD_CELLS_SetDigitalInput();     
    ADC1_Enable();          //ENABLE ANALOG TO DIGITAL CONVERTER
    ADC1_ChannelSelect( LOAD_CELLS );  //ADC1 CHANNEL SET TO RB3 PIN
    while(1){
    ADC1_SoftwareTriggerEnable();
    __delay_ms(10);
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(TEST));
    conversion = ADC1_ConversionResultGet(TEST);
    
    if ((conversion > 2200)){
        return 50000;
    }
    else if ((conversion > 1500) && (conversion <= 2200)){
        return 20000;
    }
    else if ((conversion > 800) && (conversion <= 1500)){
        return 5000;
    }
    else if ((conversion > 200) && (conversion <= 800))
        return 500;
    else{
        lcd_ini();
        clear_LCD();
        write_to_LCD("PLEASE SELECT");
        second_line();
        write_to_LCD("LOAD CELL");
    }
    
    }
}
*/
unsigned int lctestget(void){
    
    UP_BUTTON_SetInterruptHandler(upc);
    DOWN_BUTTON_SetInterruptHandler(downc);
    LEFT_BUTTON_SetInterruptHandler(leftc);
    RIGHT_BUTTON_SetInterruptHandler(rightc);
    BACK_BUTTON_SetInterruptHandler(backc);
    SELECT_BUTTON_SetInterruptHandler(selectc);
    INTERRUPT_GlobalEnable();
    UP_BUTTON_SetDigitalInput();
    DOWN_BUTTON_SetDigitalInput();
    LEFT_BUTTON_SetDigitalInput();
    RIGHT_BUTTON_SetDigitalInput();
    BACK_BUTTON_SetDigitalInput();
    SELECT_BUTTON_SetDigitalInput();
    
    lcd_ini();
    clear_LCD();
    write_to_LCD("Please Select");
    second_line();
    write_to_LCD("Load Cell");
    char lclist[5][50] = {"500lbs Cell", "5,000lbs Cell", "20,000lbs Cell", "50,000lbs Cell", ""};
    unsigned int lcnum = 0;
    
    cursor_on();
    write_to_LCD(">>");
    write_to_LCD(lclist[0]);
    second_line();
    write_to_LCD(lclist[lcnum+1]);
    first_line();
    cursor_on();
    shift_cursor_right(1);
    
    shift_cursor_left(9);
    
    while(1){
        __delay_ms(1);
        if (downcountc == 1){
            upcountc = 0;
            downcountc = 0;
            lcnum++;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(lclist[lcnum]);
            second_line();
            write_to_LCD(lclist[lcnum+1]);
            first_line();
            cursor_on();
            shift_cursor_right(1);
            
        }
        else if (upcountc == 1){
            upcountc = 0;
            downcountc = 0;
            lcnum--;
            //shift_cursor_left(2);
            clear_LCD();
            first_line();
            cursor_on();
            write_to_LCD(">>");
            write_to_LCD(lclist[lcnum]);
            second_line();
            write_to_LCD(lclist[lcnum+1]);
            cursor_on();
            shift_cursor_right(1);
        }
        else if (backcountc == 1){
            backcountc = 0;
            selectcountc = 0;
            menucountc--;
        }
        else if (selectcountc == 1){
            backcountc = 0;
            selectcountc = 0;
            menucountc++;
        }
        else{
            Nop();
        }
        
        if (lcnum == 3){
            downcountc = 2;
        }
        else if (lcnum == 0){
            upcountc = 2;
        }
        else{
            upcountc = 0;
            downcountc = 0;
        }
        
        if (menucountc == 2){
            menucountc = 1;
            int cll = cellvals[lcnum];
            return cll;
            
        }
        else{
            Nop();
        }   
    }
}