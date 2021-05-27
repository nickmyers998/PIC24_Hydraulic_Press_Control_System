/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ256GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"

/**
 Section: File specific functions
*/
void (*HOME_BUTTON_InterruptHandler)(void) = NULL;
void (*UP_BUTTON_InterruptHandler)(void) = NULL;
void (*DOWN_BUTTON_InterruptHandler)(void) = NULL;
void (*LEFT_BUTTON_InterruptHandler)(void) = NULL;
void (*BACK_BUTTON_InterruptHandler)(void) = NULL;
void (*RIGHT_BUTTON_InterruptHandler)(void) = NULL;
void (*E_STOP_BUTTON_InterruptHandler)(void) = NULL;
void (*BUTTON_InterruptHandler)(void) = NULL;
void (*SELECT_BUTTON_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x7F94;
    TRISB = 0xB7FB;
    TRISC = 0x0308;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPDC = 0x0000;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;
    IOCPUC = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x0008;
    ANSB = 0xB208;
    ANSC = 0x0008;
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR5bits.RP11R = 0x0003;    //RB11->UART1:U1TX
    RPOR7bits.RP14R = 0x000D;    //RB14->OC1:OC1
    RPOR1bits.RP2R = 0x000E;    //RB2->OC2:OC2
    RPINR18bits.U1RXR = 0x000A;    //RB10->UART1:U1RX

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    /****************************************************************************
     * Interrupt On Change: positive
     ***************************************************************************/
    IOCPAbits.IOCPA11 = 1;    //Pin : RA11
    IOCPAbits.IOCPA13 = 1;    //Pin : RA13
    IOCPAbits.IOCPA2 = 1;    //Pin : RA2
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    IOCNAbits.IOCNA10 = 1;    //Pin : RA10
    IOCNAbits.IOCNA12 = 1;    //Pin : RA12
    IOCNAbits.IOCNA4 = 1;    //Pin : RA4
    IOCNAbits.IOCNA7 = 1;    //Pin : RA7
    IOCNAbits.IOCNA8 = 1;    //Pin : RA8
    IOCNAbits.IOCNA9 = 1;    //Pin : RA9
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    IOCFAbits.IOCFA10 = 0;    //Pin : RA10
    IOCFAbits.IOCFA11 = 0;    //Pin : RA11
    IOCFAbits.IOCFA12 = 0;    //Pin : RA12
    IOCFAbits.IOCFA13 = 0;    //Pin : RA13
    IOCFAbits.IOCFA2 = 0;    //Pin : RA2
    IOCFAbits.IOCFA4 = 0;    //Pin : RA4
    IOCFAbits.IOCFA7 = 0;    //Pin : RA7
    IOCFAbits.IOCFA8 = 0;    //Pin : RA8
    IOCFAbits.IOCFA9 = 0;    //Pin : RA9
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    PADCONbits.IOCON = 1;    //Config for PORTA
    
    /* Initialize IOC Interrupt Handler*/
    HOME_BUTTON_SetInterruptHandler(&HOME_BUTTON_CallBack);
    UP_BUTTON_SetInterruptHandler(&UP_BUTTON_CallBack);
    DOWN_BUTTON_SetInterruptHandler(&DOWN_BUTTON_CallBack);
    LEFT_BUTTON_SetInterruptHandler(&LEFT_BUTTON_CallBack);
    BACK_BUTTON_SetInterruptHandler(&BACK_BUTTON_CallBack);
    RIGHT_BUTTON_SetInterruptHandler(&RIGHT_BUTTON_CallBack);
    E_STOP_BUTTON_SetInterruptHandler(&E_STOP_BUTTON_CallBack);
    BUTTON_SetInterruptHandler(&BUTTON_CallBack);
    SELECT_BUTTON_SetInterruptHandler(&SELECT_BUTTON_CallBack);
    
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS1bits.IOCIF = 0; //Clear IOCI interrupt flag
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void __attribute__ ((weak)) HOME_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) UP_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) DOWN_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) LEFT_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) BACK_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) RIGHT_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) E_STOP_BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) BUTTON_CallBack(void)
{

}

void __attribute__ ((weak)) SELECT_BUTTON_CallBack(void)
{

}

void HOME_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    HOME_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void HOME_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    HOME_BUTTON_SetInterruptHandler(handler);
}

void UP_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    UP_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void UP_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    UP_BUTTON_SetInterruptHandler(handler);
}

void DOWN_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    DOWN_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void DOWN_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    DOWN_BUTTON_SetInterruptHandler(handler);
}

void LEFT_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    LEFT_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void LEFT_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    LEFT_BUTTON_SetInterruptHandler(handler);
}

void BACK_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    BACK_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void BACK_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    BACK_BUTTON_SetInterruptHandler(handler);
}

void RIGHT_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    RIGHT_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void RIGHT_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    RIGHT_BUTTON_SetInterruptHandler(handler);
}

void E_STOP_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    E_STOP_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void E_STOP_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    E_STOP_BUTTON_SetInterruptHandler(handler);
}

void BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    BUTTON_SetInterruptHandler(handler);
}

void SELECT_BUTTON_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    SELECT_BUTTON_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void SELECT_BUTTON_SetIOCInterruptHandler(void *handler)
{ 
    SELECT_BUTTON_SetInterruptHandler(handler);
}

/* Interrupt service routine for the IOCI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
{
    if(IFS1bits.IOCIF == 1)
    {
        if(IOCFAbits.IOCFA2 == 1)
        {
            if(HOME_BUTTON_InterruptHandler) 
            { 
                HOME_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA2 = 0;  //Clear flag for Pin - RA2

        }
        
        if(IOCFAbits.IOCFA4 == 1)
        {
            if(UP_BUTTON_InterruptHandler) 
            { 
                UP_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA4 = 0;  //Clear flag for Pin - RA4

        }
        
        if(IOCFAbits.IOCFA7 == 1)
        {
            if(DOWN_BUTTON_InterruptHandler) 
            { 
                DOWN_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA7 = 0;  //Clear flag for Pin - RA7

        }
        
        if(IOCFAbits.IOCFA10 == 1)
        {
            if(LEFT_BUTTON_InterruptHandler) 
            { 
                LEFT_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA10 = 0;  //Clear flag for Pin - RA10

        }
        
        if(IOCFAbits.IOCFA9 == 1)
        {
            if(BACK_BUTTON_InterruptHandler) 
            { 
                BACK_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA9 = 0;  //Clear flag for Pin - RA9

        }
        
        if(IOCFAbits.IOCFA8 == 1)
        {
            if(RIGHT_BUTTON_InterruptHandler) 
            { 
                RIGHT_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA8 = 0;  //Clear flag for Pin - RA8

        }
        
        if(IOCFAbits.IOCFA13 == 1)
        {
            if(E_STOP_BUTTON_InterruptHandler) 
            { 
                E_STOP_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA13 = 0;  //Clear flag for Pin - RA13

        }
        
        if(IOCFAbits.IOCFA11 == 1)
        {
            if(BUTTON_InterruptHandler) 
            { 
                BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA11 = 0;  //Clear flag for Pin - RA11

        }
        
        if(IOCFAbits.IOCFA12 == 1)
        {
            if(SELECT_BUTTON_InterruptHandler) 
            { 
                SELECT_BUTTON_InterruptHandler(); 
            }
            
            IOCFAbits.IOCFA12 = 0;  //Clear flag for Pin - RA12

        }
        
        
        // Clear the flag
        IFS1bits.IOCIF = 0;
    }
}

