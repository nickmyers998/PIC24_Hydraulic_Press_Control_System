#define FCY 32000000UL
#define PWM_PERIOD 62500

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>


int counterman = 0;
int conversion = 0;
int exitint = 0;

void manualmode(long loadcell);

void irr(void){
    //Emergency stop button interrupt
    estop();
}

void exitmanual(void){
    //
    exitint = 1;
}

void irrdata(void){
    //timer interrupt to send data to UART transmitter
    counterman++;
    datasend(conversion, counterman);
}

void manualmode(long loadcell){
    
    /*
     * THIS FUNCTION IS CALLED WHEN MANUAL OPERATION IS REQUESTED.
     * VOLTAGE ACROSS THROTTLE POTENTIOMETER IS FORWARDED TO ADC AND TRANSLATED
     * TO PROPORTIONAL PWM VOLTAGE OUTPUT. PWM OUTPUT IS CONNECTED TO 
     * 2 NPN MOSFET
     * TRANSISTORS CONFIGURED AS TRANSCONDUCTANCE AMPLIFIERS.
     * AMPLIFIER OUTPUTS ARE CONNECTED TO 2 SOLENOID VALVES WHICH CORRESPOND
     * TO OPEN AND CLOSE VALVE STATES RESPECTIVELY.
     */
    TMR3_SetInterruptHandler(irrdata);
    E_STOP_BUTTON_SetInterruptHandler(irr);
    BUTTON_SetInterruptHandler(exitmanual);
    INTERRUPT_GlobalEnable();
    //BUTTON_SetIOCInterruptHandler(IrrTest);
    BUTTON_SetDigitalInput();
    SW_SetDigitalInput();
    
    int alt = 1;
    int alt1 = 1;
    int newc1 = 0;
    int newc2 = 0;
    volatile int hexr1;     //CONVERTS conversion TO PWM RATIO FOR OSC1
    volatile int hexr2;     //CONVERTS conversion TO PWM RATIO FOR OSC2
    THROTTLE_POT_SetDigitalInput();     //SET THROTTLE_POT TO DIGITAL INPUT
    //SW_SetDigitalInput();
    ADC1_Enable();          //ENABLE ANALOG TO DIGITAL CONVERTER
    ADC1_ChannelSelect( TEST );  //ADC1 CHANNEL SET TO RB3 PIN
    OC2_Start();            //ENABLE OSCILLATOR 2
    OC1_Start();            //ENABLE OSCILLATOR 1
    
    lcd_ini();
    clear_LCD();
    cursor_on();
    second_line();
    //write_to_LCD("RunTime: 00h 00m");
    first_line();
    write_to_LCD("Load: ");
    shift_cursor_right(6); //what??
    /*
    lcd_ini();
    clear_LCD();
    cursor_on();
    first_line();
    write_to_LCD("Press Select to");
    second_line();
    write_to_LCD("End Manual Mode");
    TMR3_Start();
    */
    float scalerm = 0.0;
    float cutoff = 0.0;
    volatile long senseloadm = 0;
    float k = 0;
    char s[4] = "";
    
    switch(loadcell){       
        // provides scaler and cutoff values for selected load cell
        case 500:
            scalerm = 0.122;
            cutoff = 475;
            k = 137;
            break;
        case 5000:
            scalerm = 1.220;
            cutoff = 4750;
            k = 13.7;
            break;
        case 20000:
            scalerm = 4.667;
            cutoff = 19000;
            k = 3.44;
            break;
        case 50000:
            scalerm = 12.200;
            cutoff = 47500;
            k = 1.37;
            break;
        //default:
        //    return;
    }
    
    
    
    while(1){               //MAIN LOOP
        
        if(exitint == 1){
            // exits manual mode
            enddatasend();
            break;
        }
        
        ADC1_SoftwareTriggerEnable();
        __delay_ms(10);
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(TEST));
        conversion = ADC1_ConversionResultGet(TEST);
        
        if(SW_GetValue() == 1){     
            // activates reduced sensitivity on throttle button press
            alt = 8;
            alt1 = 64;
        }
        else{
            alt = 32;
            alt1 = 32;
        }
        
        if(conversion > 2056){      
            newc2 = conversion - 2056;
            hexr2 = newc2 * 32;        
            OC2R = hexr2;   //pwm output to osc 2
            OC1R = 0; 
            __delay_ms(2);
        }
        else if(conversion < 2040){
            newc1 = conversion * 32;
            hexr1 = 65280 - newc1;        
            OC2R = 0;
            OC1R = hexr1;   //pwm output to osc 1
            __delay_ms(2);
        }
        else{
            OC2R = 0;
            OC1R = 0;
        }
        
        
        senseloadm = conversion * scalerm;
       
        // sends load data to LCD
        sprintf(s, "%-5.5lu", senseloadm);
        first_line();
        shift_cursor_right(6);
        write_to_LCD(s);
        write_to_LCD(" lbs");
        shift_cursor_left(9);
        
    }
    
    home();
    
}
