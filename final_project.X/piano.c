#include <xc.h>
#include <pic18f4520.h>
#include <stdio.h>
#include "setting.h"
#include "uart.h"

#define _XTAL_FREQ  1000000

#define c  261.63
#define c1 277.18
#define d  293.66
#define d1 311.13
#define e  329.63
#define f  349.23
#define f1 369.99
#define g  392.00
#define g1 415.30
#define a  440.00
#define a1 466.16
#define b  493.88

char str[10];

int TMR2PRESCALE = 4;
float times = 1;
int curr_value = 2;

void change_prescale(int prescale) {
    if (TMR2PRESCALE != prescale) {
        TMR2PRESCALE = prescale;
        switch(prescale) {
            case 1:
                T2CONbits.T2CKPS = 0b00;
                break;
            case 4:
                T2CONbits.T2CKPS = 0b01;
                break;
            case 16:
                T2CONbits.T2CKPS = 0b11;
                break;
        }
    }
}

void __interrupt() ADC_ISR (){
    int value = (unsigned char)(ADRESH * 5 / 256);
    switch(value) {
        case 0:
            change_prescale(16);
            times = 0.5;
            break;
        case 1:
            change_prescale(4);
            times = 1;
            break;
        case 2:
            change_prescale(4);
            times = 2;
            break;
        case 3:
            change_prescale(1);
            times = 4;
            break;
        case 4:
            change_prescale(1);
            times = 8;
            break;
    }
    PIR1bits.ADIF = 0;
    if (value != curr_value)
    {
        curr_value = value;
        sprintf(str, "c%d\n\r", value);
        UART_Write_Text(str);
    }
}

void play_reset()
{
    sprintf(str, "r\n\r");
    UART_Write_Text(str);
}

void play_tone(float freq, float times, int state)
{
    play_reset();
    PR2 = (unsigned char)(_XTAL_FREQ / (freq * times * 4 * TMR2PRESCALE) - 1);
    int duty = _XTAL_FREQ / (freq * times * TMR2PRESCALE) / 2;
    CCP1CONbits.CCP1X = duty & 0b10;
    CCP1CONbits.CCP1Y = duty & 0b01;
    CCPR1L = (unsigned char)(duty  >>  2);
    sprintf(str, "%d\n\r", state);
    UART_Write_Text(str);
}

void main(void)
{
    SYSTEM_Initialize();

    T2CONbits.TMR2ON = 0;
    T2CONbits.T2CKPS = 0b01; // prescaler = 4

    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0;
    
    PR2 = 0x9b;
    
    ADCON0bits.GO = 1;
    __delay_us(5);
    
    int state[2] = {0};
    while(1) {
        ADCON0bits.GO = 1;
        __delay_us(5);
        state[0] = 0;
        if (PORTAbits.RA1) {
            state[0] = 1;
        }
        if (PORTAbits.RA2) {
            state[0] = 2;
        }
        if (PORTAbits.RA3) {
            state[0] = 3;
        }
        if (PORTAbits.RA4) {
            state[0] = 4;
        }
        if (PORTAbits.RA5) {
            state[0] = 5;
        }
        if (PORTAbits.RA6) {
            state[0] = 6;
        }
        if (PORTAbits.RA7) {
            state[0] = 7;
        }
        if (PORTBbits.RB4) {
            state[0] = 8;
        }
        if (PORTBbits.RB3) {
            state[0] = 9;
        }
        if (PORTBbits.RB2) {
            state[0] = 10;
        }
        if (PORTBbits.RB1) {
            state[0] = 11;
        }
        if (PORTBbits.RB0) {
            state[0] = 12;
        }
        
        if (state[0] != state[1])
        {
            state[1] = state[0];
            switch(state[0])
            {
                case 1:
                    T2CONbits.TMR2ON = 0;
                    play_tone(c, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 2:
                    T2CONbits.TMR2ON = 0;
                    play_tone(c1, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 3:
                    T2CONbits.TMR2ON = 0;
                    play_tone(d, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 4:
                    T2CONbits.TMR2ON = 0;
                    play_tone(d1, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 5:
                    T2CONbits.TMR2ON = 0;
                    play_tone(e, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 6:
                    T2CONbits.TMR2ON = 0;
                    play_tone(f, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 7:
                    T2CONbits.TMR2ON = 0;
                    play_tone(f1, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 8:
                    T2CONbits.TMR2ON = 0;
                    play_tone(g, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 9:
                    T2CONbits.TMR2ON = 0;
                    play_tone(g1, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 10:
                    T2CONbits.TMR2ON = 0;
                    play_tone(a, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 11:
                    T2CONbits.TMR2ON = 0;
                    play_tone(a1, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                case 12:
                    T2CONbits.TMR2ON = 0;
                    play_tone(b, times, state[0]);
                    T2CONbits.TMR2ON = 1;
                    break;
                default:
                    T2CONbits.TMR2ON = 0;
                    play_reset();
                    break;
            }
        }
    }
    return;
}