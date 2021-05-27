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

int counter = 0;
long totaltime = 0;
int exit0 = 0;
long senseload = 0;

void irr3(void){
    enddatasend();
    estop();
    exit0 = 1;
}

void irrcounter(void){
    counter++;
    totaltime++;
    datasend(senseload, totaltime);
}

void run(long target, long time, long interval, int loadcell){
    
    // This function handles the automated press routines from either routines()
    // or jog(). It contains built in timer interrupts for total time and time
    // interval control, as well as force regulation to ensure steady force output
    
    TMR3_SetInterruptHandler(irrcounter);
    E_STOP_BUTTON_SetInterruptHandler(irr3);
    INTERRUPT_GlobalEnable();
    int conversion = 0;     //HOLDS ADC CONVERSION VALUE RANGE(0, 4096))
    volatile unsigned int hexr1;     //CONVERTS conversion TO PWM RATIO FOR OSC1
    volatile unsigned int hexr2;     //CONVERTS conversion TO PWM RATIO FOR OSC2
    ADC1_Enable();          //ENABLE ANALOG TO DIGITAL CONVERTER
    ADC1_ChannelSelect( THROTTLE_POT );  //ADC1 CHANNEL SET TO RB3 PIN
    OC2_Start();            //ENABLE OSCILLATOR 2
    OC1_Start();            //ENABLE OSCILLATOR 1
    
    const unsigned long targetconst = target;
    
    
    float scaler = 0;
    float cutoff = 0;
    signed long difference = -1;
    float multiplier = 0;
    float start_force = 0;
    char s[4];
    char h[2];
    int min = 0;
    int hr = 0;
    float k = 0;
    bool flipper = true;
    long load = 0;
    
    long timer = 0;
    long total_time = 0;
    
    switch(loadcell){       //SET SCALER VALUES AND CUTOFF VALUES
        case 500:
            scaler = 0.122;
            cutoff = 475;
            k = 137;
            break;
        case 5000:
            scaler = 1.220;
            cutoff = 4750;
            k = 13.7;
            break;
        case 20000:
            scaler = 4.667;
            cutoff = 19000;
            k = 3.44;
            break;
        case 30000:
            scaler = 12.200;
            cutoff = 47500;
            k = 1.37;
            break;
        //default:
        //    return;
    }
    
    lcd_ini();
    clear_LCD();
    cursor_on();
    second_line();
    write_to_LCD("RunTime: 00h 00m");
    first_line();
    write_to_LCD("Load: ");
    shift_cursor_right(6); //what??
    TMR3_Start();
    
    
    while(1){               //MAIN LOOP
        
        
        if (exit0 == 1){
            return;
        }
        
        if (counter >= interval){   
            // reverses force value on timer interval interrupt
            flipper = !flipper;
            counter = 0;
        }
        
        if (totaltime >= time){
            // called when total time reaches user specified maximum
            enddatasend();
            endprocedure();
            return;
        }
       
        if (flipper){
            load = targetconst;
        }
        else if (!flipper){
            load = 0;
        }
        
        ADC1_SoftwareTriggerEnable();
        __delay_ms(1);
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(THROTTLE_POT));
        conversion = ADC1_ConversionResultGet(THROTTLE_POT);
        
        senseload = conversion * scaler;
        difference = load - senseload;
        multiplier = k * difference;
        if (multiplier > 0xffff){
            multiplier = 0xffff;
        }
        
        long test = senseload;
        
        
        if(difference >= 0){      //PWM PROPORTIONAL TO 
            hexr2 = multiplier;        
            OC2R = hexr2;
            OC1R = 0;
            __delay_ms(10);
        }
        else if(difference < 0){
            hexr1 = (multiplier * -1);        
            OC2R = 0;
            OC1R = hexr1;
            __delay_ms(10);
        }
        else{
            Nop();
        }
        
        // sends load data and elapsed time to lcd
        sprintf(s, "%-5.5lu", test);
        first_line();
        shift_cursor_right(6);
        write_to_LCD(s);
        write_to_LCD(" lbs");
        shift_cursor_left(9);
        if (totaltime % 60 == 0){
            min++;
            if (totaltime % 3600 == 0){
                hr++;
                min = 0;
                second_line();
                shift_cursor_right(9);
                sprintf(h, "%-2.2u", hr);
                write_to_LCD(h);
                shift_cursor_right(2);
                sprintf(h, "%-2.2u", min);
                write_to_LCD(h);
            }
            else{
                second_line();
                shift_cursor_right(13);
                sprintf(h, "%-2.2u", min);
                write_to_LCD(h);
            }   
        }
    }
    
    
    
}
