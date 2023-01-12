#include <xc.h>

void CCP1_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP1CON=0x0c;		// Compare mode, initialize CCP1 pin high, clear output on compare match
}
