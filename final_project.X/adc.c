#include <xc.h>

void ADC_Initialize(void) {
    TRISA = 0xff;		// Set as input port
    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
    ADCON1bits.VCFG = 0b00;//VREF+ = VDD VREF- = VSS
    ADCON1bits.PCFG = 0b1110;//AN0(RA0)=A
    ADCON0bits.CHS = 0b0000;//analog input = RA0
    ADCON2bits.ADCS = 0b000;//Fosc = 1MHz, Tad = 2*Tosc = 2 µs
    ADCON2bits.ACQT = 0b001;//Tad = 2 µs, AQtime 2*tad = 4 > 2.4
    ADCON2bits.ADFM = 0;//8-bits, left justified
    ADCON0bits.ADON = 1;//open A/D convert bit
    PIE1bits.ADIE = 1;//enable AD convert interrupt
    PIR1bits.ADIF = 0;//AD convert interrupt flag bit is clear at first
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS =  0x07; // Select Channe7
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}