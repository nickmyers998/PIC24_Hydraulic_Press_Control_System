/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ256GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/* 
 * File:   main.c
 * Author: nmyers
 *
 * Created on February 11, 2021, 2:56 PM
 */
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
/*
 * 
 */

unsigned int lctestget(void);

int main(int argc, char** argv) {
    
    // Starts press initialization and user input
    
    SYSTEM_Initialize();
    
    welcome();
    int option = 0;
    
    while (1){
        resetm();
        unsigned int load__cell = lctestget();       
        __delay_ms(50);
        option = 10;
        option = menu();
        switch (option){
            case 0:
                manualmode(load__cell);
                break;
            case 1:
                routines();
                break;
            case 2:
                jog(load__cell);
                break;
            default:
                Nop();
        }
    }
}

