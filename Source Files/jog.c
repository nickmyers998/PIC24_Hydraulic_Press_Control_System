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

unsigned int upcount = 0;
unsigned int downcount = 0;
unsigned int backcount = 0;
unsigned int selectcount = 0;
unsigned int leftcount = 0;
unsigned int rightcount = 0;
unsigned int menucount = 1;


void jog(int load_cell);
long get_force(void);
long get_time(void);
long get_interval(void);
void reset(void);
void throw_lc_err(void);

void up(void){          //UP BUTTON INTERRUPT TO INCREASE FORCE ENTRY
    __delay_ms(10);
    if (upcount != 2){
        upcount = 1;
    }
    downcount = 0;
    __delay_ms(10);
}

void down(void){        //DOWN BUTTON INTERRUPT TO DECREASE FORCE ENTRY
    __delay_ms(10);
    if (downcount != 2){
        downcount = 1;
    }
    upcount = 0;
    TEST_LED_SetHigh();
    __delay_ms(10);
}

void left(void){        //LEFT BUTTON INTERRUPT TO SHIFT CURSOR LEFT
    __delay_ms(10);
    if (leftcount != 2){
        leftcount = 1;
    }
    __delay_ms(10);
}

void right(void){       //RIGHT BUTTON INTERRUPT TO SHIFT CURSOR RIGHT
    __delay_ms(10);
    if (rightcount != 2){
        rightcount = 1;
        upcount = 0;
        downcount = 0;
    }
    __delay_ms(10);
}

void back(void){        //BACK BUTTON INTERRUPT TO RETURN FROM MENU
    __delay_ms(10);
    backcount = 1;
    __delay_ms(10);
}

void select(void){      //SELECT BUTTON INTERRUPT TO ADVANCE MENU
    __delay_ms(10);
    selectcount = 1;
    __delay_ms(10);
}

/*      jog() DESCRIPTION:
 *      FUNCTION TO ALLOW USER TO PROGRAM PRESS ROUTINE WITH TARGET FORCE,
 *      FORCE INTERVAL TIME, AND TOTAL TIME. ONLY ALLOWS FOR PROGRAMMING
 *      STEP FUNCTION. 
 */


void jog(int load_cell){
    
    // initialize interrupts
    UP_BUTTON_SetInterruptHandler(up);
    DOWN_BUTTON_SetInterruptHandler(down);
    LEFT_BUTTON_SetInterruptHandler(left);
    RIGHT_BUTTON_SetInterruptHandler(right);
    BACK_BUTTON_SetInterruptHandler(back);
    SELECT_BUTTON_SetInterruptHandler(select);
    INTERRUPT_GlobalEnable();
    UP_BUTTON_SetDigitalInput();
    DOWN_BUTTON_SetDigitalInput();
    LEFT_BUTTON_SetDigitalInput();
    RIGHT_BUTTON_SetDigitalInput();
    BACK_BUTTON_SetDigitalInput();
    SELECT_BUTTON_SetDigitalInput();
    
    TEST_LED_SetDigitalOutput();
    
    unsigned int maxlbs;
    unsigned int maxinterval;
    unsigned int maxtime;
    
    unsigned long time = 0;
    unsigned long load_lbs = 0;
    unsigned long time_int = 0;
    float load_max = load_cell * 0.95;  //set maximum load
    lcd_ini();
    int men = 0;
    reset();
    
    while(1){
        //menu selection loop
        reset();
        if (men == 0){
            load_lbs = get_force();
            if (load_lbs == 0){
                return;
            }
            else if (load_lbs >= load_max){
                throw_lc_err();
                continue;
            }
        }
        men = 0;
        reset();
        time_int = get_interval();
        if (time_int == 0){
            continue;
        }
        reset();
        time = get_time();
        if (time == 0){
            men = 1;
            continue;
        }
        break;
    }
    
    run(load_lbs, time, time_int, load_cell);
    return;
    
    
}

long get_force(void){
    // gets force input from user
    // range 0-50000
    long load_lbs = 0;
    int i;
    unsigned char numsf[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    volatile unsigned char tensf[5] = {'0', '0', '0', '0', '0'};
    volatile unsigned int tensnumf[5] = {0, 0, 0, 0, 0};
    volatile unsigned int positionf = 0;
    volatile unsigned int currentnumf = 0;
    lcd_ini();
    clear_LCD();
    write_to_LCD("Force(lbs):00000");
    cursor_on();
    shift_cursor_right(11);
    while(1){
        __delay_ms(1);
        if(upcount == 1){
            upcount = 0;
            downcount = 0;
            currentnumf++;
            tensf[positionf] = numsf[currentnumf];
            tensnumf[positionf] = currentnumf;
            shift_cursor_left(positionf);
            write_to_LCD(tensf);
            shift_cursor_left(5 - positionf);
            TEST_LED_SetLow();
        }
        else if(downcount == 1){
            //TEST_LED_SetHigh();
            upcount = 0;
            downcount = 0;
            currentnumf--;
            tensf[positionf] = numsf[currentnumf];
            tensnumf[positionf] = currentnumf;
            shift_cursor_left(positionf);
            write_to_LCD(tensf);
            shift_cursor_left(5 - positionf);
            TEST_LED_SetLow();
        }
        else if(leftcount == 1){
            leftcount = 0;
            rightcount = 0;
            positionf--;
            currentnumf = tensnumf[positionf];
            shift_cursor_left(1);
            
        }
        else if(rightcount == 1){
            leftcount = 0;
            rightcount = 0;
            positionf++;
            currentnumf = tensnumf[positionf];
            shift_cursor_right(1);
        }
        else if(backcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount--;
        }
        else if(selectcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount++;
            load_lbs = (tensnumf[0]*10000) + (tensnumf[1] * 1000) + (tensnumf[2] * 100) + (tensnumf[3] * 10) + (tensnumf[4]);
        }
        else{
            Nop();
        }
        
        if (currentnumf == 9){
            upcount = 2;
        }
        else if (currentnumf == 0){
            downcount = 2;
        }
        else{
            upcount = 0;
            downcount = 0;
        }
        
        if (positionf == 4){
            rightcount = 2;
        }
        else if (positionf == 0){
            leftcount = 2;
        }
        else{
            Nop();
        }
        
        if (menucount == 2){
            menucount = 1;
            return load_lbs;
        }
        else if (menucount == 0){
            menucount = 1;
            return 0;
        }
    }
}

long get_time(void){
    // gets time input from user
    // range 0-6039 minutes
    long time = 0;
    unsigned char numst[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    unsigned char tenst[5] = {'0', '0', '/', '0', '0'};
    unsigned int tensnumt[5] = {0, 0, 0, 0, 0};
    unsigned int positiont = 0;
    unsigned int currentnumt = 0;
    lcd_ini();
    clear_LCD();
    write_to_LCD("Time(h/mn):00/00");
    cursor_on();
    shift_cursor_right(11);
    while(1){
        
        if(upcount == 1){
            upcount = 0;
            downcount = 0;
            currentnumt++;
            tenst[positiont] = numst[currentnumt];
            tensnumt[positiont] = currentnumt;
            shift_cursor_left(positiont);
            write_to_LCD(tenst);
            shift_cursor_left(5 - positiont);
        }
        else if(downcount == 1){
            upcount = 0;
            downcount = 0;
            currentnumt--;
            tenst[positiont] = numst[currentnumt];
            tensnumt[positiont] = currentnumt;
            shift_cursor_left(positiont);
            write_to_LCD(tenst);
            shift_cursor_left(5 - positiont);
        }
        else if(leftcount == 1){
            leftcount = 0;
            rightcount = 0;
            if (positiont == 3){
                positiont = 1;
                shift_cursor_left(2);
            }
            else {
                positiont--;
                currentnumt = tensnumt[positiont];
                shift_cursor_left(1);
            }
            
            
        }
        else if(rightcount == 1){
            leftcount = 0;
            rightcount = 0;
            if (positiont == 1){
                positiont = 3;
                shift_cursor_right(2);
            }
            else {
                positiont++;
                currentnumt = tensnumt[positiont];
                shift_cursor_right(1);
            }
        }
        else if(backcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount--;
        }
        else if(selectcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount++;
            time = (tensnumt[0]*36000) + (tensnumt[1] * 3600) + (tensnumt[3] * 600) + (tensnumt[4] * 60);
        }
        else{
            Nop();
        }
        
        if (currentnumt == 9){
            upcount = 2;
        }
        else if (currentnumt == 0){
            downcount = 2;
        }
        else{
            Nop();
        }
        
        if (positiont == 4){
            rightcount = 2;
        }
        else if (positiont == 0){
            leftcount = 2;
        }
        else{
            Nop();
        }
        
        if (menucount == 2){
            menucount = 1;
            return time;
        }
        else if (menucount == 0){
            menucount = 1;
            return 0;
        }
    }
}

long get_interval(void){
    // gets time interval from user
    // range 0-6940 minutes
    long time_int = 0;
    int i;
    unsigned char nums[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    unsigned char tens[5] = {'0', '0', '/', '0', '0'};
    unsigned int tensnum[5] = {0, 0, 0, 0, 0};
    unsigned int position = 0;
    unsigned int currentnum = 0;
    lcd_ini();
    clear_LCD();
    write_to_LCD("Int(h/min):00/00");
    cursor_on();
    shift_cursor_right(11);
    while(1){
        
        if(upcount == 1){
            upcount = 0;
            downcount = 0;
            currentnum++;
            tens[position] = nums[currentnum];
            tensnum[position] = currentnum;
            shift_cursor_left(position);
            write_to_LCD(tens);
            shift_cursor_left(5 - position);
        }
        else if(downcount == 1){
            upcount = 0;
            downcount = 0;
            currentnum--;
            tens[position] = nums[currentnum];
            tensnum[position] = currentnum;
            shift_cursor_left(position);
            write_to_LCD(tens);
            shift_cursor_left(5 - position);
        }
        else if(leftcount == 1){
            leftcount = 0;
            rightcount = 0;
            if (position == 3){
                position = 1;
                shift_cursor_left(2);
            }
            else {
                position--;
                currentnum = tensnum[position];
                shift_cursor_left(1);
            }
        }
        else if(rightcount == 1){
            leftcount = 0;
            rightcount = 0;
            if (position == 1){
                position = 3;
                shift_cursor_right(2);
            }
            else {
                position++;
                currentnum = tensnum[position];
                shift_cursor_right(1);
            }
        }
        else if(backcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount--;
        }
        else if(selectcount == 1){
            backcount = 0;
            selectcount = 0;
            menucount++;
            time_int = (tensnum[0]*36000) + (tensnum[1] * 3600) + (tensnum[3] * 600) + (tensnum[4] * 60);
        }
        else{
            Nop();
        }
        
        if (currentnum == 9){
            upcount = 2;
        }
        else if (currentnum == 0){
            downcount = 2;
        }
        else{
            Nop();
        }
        
        if (position == 4){
            rightcount = 2;
        }
        else if (position == 0){
            leftcount = 2;
        }
        else{
            Nop();
        }
        
        if (menucount == 2){
            menucount = 1;
            return time_int;
        }
        else if (menucount == 0){
            menucount = 1;
            return 0;
        }
    }
}

void reset(void){
    // resets counter values
    upcount = 0;
    downcount = 0;
    backcount = 0;
    selectcount = 0;
    leftcount = 0;
    rightcount = 0;
    menucount = 1;
}

void throw_lc_err(void){
    // called when force entered is too large for load cell
    lcd_ini();
    clear_LCD();
    write_to_LCD("Force too large");
    second_line();
    write_to_LCD("for load cell");
    __delay_ms(500);
    return;
}

