#include <io.h>
#include <stdint.h>
#include "msp430f5529.h" //use your msp430 configuration!

//#include <msp430.h>

int oldmain(void) {
    // Disable watchdog timer
    WDTCTL = WDTPW + WDTHOLD;

    /* --- Set Oscillator Settings ----------------------
    BCSCTL1 = XT2OFF;           // Turn off XT2 oscillator, ACLK = low freq mode/1
    BCSCTL1 = CALBC1_1MHZ;      // Set DCO to callibrated 1Mhz
    DCOCTL  = CALDCO_1MHZ;      // Set DCO to callibrated 1Mhz
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3; // MCLK = DCO/1, SMCLK = MCLK/8
    BCSCTL3 = LFXT1S_0 | XCAP_2;        // LFXT1 = 32.768khz, 10pF termination
    IE1 &= ~OFIE;               // Disable oscillator fault interrupt
    /* -------------------------------------------------- */

    /* --- Set Timer Settings ---------------------------
    // Timer A clock source = ACLK/1, continuous mode, interrupt enabled
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR | TAIE;
    TACCTL0 = CCIE;     // Enable timer A comparator 0 interrupt
    TACCR0  = 32768;    // Set initial comparator value
    /* -------------------------------------------------- */

    /* --- Set Port Settings ----------------------------
    // Set port direction to output
    P1DIR = BIT0 | BIT6;
    // Set initial port values
    P1OUT = ~(BIT0 | BIT6);
    /* -------------------------------------------------- */

    // Go into low power mode with interrupts enabled
   // _BIS_SR(LPM1_bits + GIE);

    while(1);
}
/*
void Timer0_A0(void) __attribute__((interrupt(TIMER0_A0_VECTOR)));
void Timer0_A0(void) {
    // This vector gets called on timer A comparator 0 interrupt
    P1OUT ^= BIT0;      // Toggle LED
    TACCR0 += 32768;    // Add offset to CCR0
    TACCTL0 &= ~CCIFG;  // Reset the interrupt flag
}

void Timer0_A1(void) __attribute__((interrupt(TIMER0_A1_VECTOR)));
void Timer0_A1(void) {
    // This vector gets called on timer A overflow and comparators 1 and 2
    P1OUT ^= BIT6;      // Toggle LED
    TACTL &= ~TAIFG;    // Reset the interrupt flag
}






/*int main() {                    // The main function
    volatile uint8_t i = 0;
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR |= 0x0F;
    // do forever:
    while (1) {
        i++;
        if (i == 0) {
            //toggle red led in gina
            P2OUT ^= 0x08;
        }
    }
}*/
