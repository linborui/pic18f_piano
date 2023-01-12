#include <xc.h>

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

