// PIC16LF1933 / PIC16LF1938
// Common anode on RC1 (pin 12), colors on RC2 (pin 13) and RC3 (pin 14)
//
// Build:
//   xc8-cc --chip=16LF1933 -O2 --outdir=build main.c
// or:
//   xc8-cc --chip=16LF1938 -O2 --outdir=build main.c
/*
 * Sort this out later:
 * 
 * m@cker:~/Desktop/code/testing$ xc8-cc --chip=16LF1938 -O2 --outdir=build main.c
xc8-cc: command not found
m@cker:~/Desktop/code/testing$ source /etc/environment
m@cker:~/Desktop/code/testing$ xc8-cc --chip=16LF1938 -O2 --outdir=build main.c
(2042) no target device specified; use the -mcpu option to specify a target device
m@cker:~/Desktop/code/testing$ xc8-cc --chip=16LF1933 -O2 --outdir=build main.c
(2042) no target device specified; use the -mcpu option to specify a target device
m@cker:~/Desktop/code/testing$ xc8-cc -mcpu=16LF1938 -O2 --outdir=build main.c
::: warning: (2044) unrecognized option "--outdir=build"
::: error: (2103) no device-support files specified; use the -mdfp option
m@cker:~/Desktop/code/testing$ xc8-cc -mcpu=16LF1938 -O2 main.c
::: error: (2103) no device-support files specified; use the -mdfp option
m@cker:~/Desktop/code/testing$ ls -ld /opt/microchip/mplabx/packs ~/.mchp_packs ~/.microchip_packs 
m@cker:~/Desktop/code/testing$ find /opt/microchip ~/.mchp_packs ~/.microchip_packs -maxdepth 4 -type d -iname "*PIC16F1*DFP*" 

OK so this is saying in a really abstract way, you need MPLAB X becuase XC8 doesnt include the header files "" DRIVERS "" neccisary to use this 
chip with this compiler. .. which is the exact type of drain bread stuff you expect from microship.
* 
* ok I installed mplax X and its still stupid Ima go home
* 
 * 
 * */
#include <xc.h>
#include <stdint.h>

#pragma config FOSC     = INTOSC
#pragma config WDTE     = OFF
#pragma config PWRTE    = OFF
#pragma config MCLRE    = ON
#pragma config CP       = OFF
#pragma config CPD      = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = OFF

#pragma config WRT      = OFF
#pragma config PLLEN    = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF

#define _XTAL_FREQ 8000000UL

static void clock_init_8mhz(void)
{
    OSCCONbits.IRCF = 0b1110; // 8 MHz HFINTOSC
    OSCCONbits.SCS  = 0b10;   // internal oscillator
}

static void gpio_init(void)
{
    ANSELC = 0x00; // all port C digital

    // RC1 = common, RC2/RC3 = LED legs
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;

    // Common anode enabled (drive high)
    LATCbits.LATC1 = 1;

    // Start with both colors off (for common anode, off = high)
    LATCbits.LATC2 = 1;
    LATCbits.LATC3 = 1;
}

static void led_off_all(void)
{
    // common anode: off = HIGH
    LATCbits.LATC2 = 1;
    LATCbits.LATC3 = 1;
}

static void led_color_a_on(void)
{
    // on = LOW (sink)
    LATCbits.LATC2 = 0;
    LATCbits.LATC3 = 1;
}

static void led_color_b_on(void)
{
    LATCbits.LATC2 = 1;
    LATCbits.LATC3 = 0;
}

static void led_both_on(void)
{
    LATCbits.LATC2 = 0;
    LATCbits.LATC3 = 0;
}

int main(void)
{
    clock_init_8mhz();
    gpio_init();

    while (1) {
        led_color_a_on();
        __delay_ms(400);

        led_off_all();
        __delay_ms(100);

        led_color_b_on();
        __delay_ms(400);

        led_off_all();
        __delay_ms(100);

        led_both_on();
        __delay_ms(400);

        led_off_all();
        __delay_ms(400);
    }
}
