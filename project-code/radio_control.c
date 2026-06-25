#ifndef F_CPU
#define F_CPU 3333333
#endif

#include <avr/io.h>
#include "fm_twi.h"
#include "oled.h"
#include <util/delay.h>

volatile uint16_t reciever_regs[16];

volatile int channel_whole = 94;
volatile int previous_channel_whole = 0;
volatile int channel_pt = 5;
volatile int previous_channel_pt = 0;

volatile uint8_t muted = 0;

void begin_reading(){
   // disable digital input buffer and pull-up resistor
    PORTD.PIN6CTRL &= ~PORT_ISC_gm;
    PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
    // enable adc
    ADC0.CTRLC = ADC_PRESC_DIV256_gc | ADC_REFSEL_VDDREF_gc;
    ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
    ADC0.INTCTRL |= ADC_RESRDY_bm;
    ADC0.CTRLA |= ADC_FREERUN_bm | ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc;
    ADC0.COMMAND = ADC_STCONV_bm;
}

void init_buttons() {
    // blue button
    PORTC.DIR &= ~PIN0_bm;
    PORTC.PIN0CTRL |= PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
    // on board button
    PORTF.DIR &= ~PIN3_bm;
    PORTF.PIN3CTRL |= PORT_PULLUPEN_bm | PORT_ISC_RISING_gc;
}

void set_volume(uint8_t vol) {
    if (vol > 0xF) {
        vol = 0xF;
    }
    readRegs(reciever_regs);
    reciever_regs[5] &= 0xFFF0;
    reciever_regs[5] |= vol;
    writeControlRegs(reciever_regs);
    readRegs(reciever_regs);
}

void reciever_init() {
    
    PORTD.DIR |= PIN5_bm;   // reset pin
    PORTA.DIR |= PIN2_bm;   // SDA (SDIO)
    
    PORTD.OUTCLR = PIN5_bm;
    _delay_ms(100);
    PORTA.OUTCLR = PIN2_bm;
    _delay_ms(100);
    PORTD.OUTSET = PIN5_bm;
    _delay_ms(100);
    
    initTWI();
    
    readRegs(reciever_regs);
    reciever_regs[7] = 0x8100;
    writeControlRegs(reciever_regs);
    _delay_ms(500);
    
    readRegs(reciever_regs);
    reciever_regs[2] = 0x4001;
    writeControlRegs(reciever_regs);
    _delay_ms(110);
    
    // configs
    readRegs(reciever_regs);
    reciever_regs[7] &= 0xBFFF;
    reciever_regs[6] = 0x007F;
    reciever_regs[5] = 0x0000;
    reciever_regs[4] = 0x4000; 
    reciever_regs[2] = 0xC201;
    
    // write again
    writeControlRegs(reciever_regs);
}

uint8_t getSTC() {
    readRegs(reciever_regs);
    return (reciever_regs[10] & 0x4000);
}

void change_channel(int chan) {
    chan = (chan - 875) / 2;
    
    readRegs(reciever_regs);
    // always want 8 at front of hex
    reciever_regs[3] = 0x8000 + chan;
    writeControlRegs(reciever_regs);
    _delay_ms(60);
    while (getSTC()) {
        _delay_ms(40);
    }
    readRegs(reciever_regs);
    reciever_regs[3] &= 0x7FFF; 
    writeControlRegs(reciever_regs);
    _delay_ms(60);
    
    while (1) {
        readRegs(reciever_regs);
        if ((reciever_regs[10] & 0x4000) == 0) {
            break;
        }
        _delay_ms(40);
    }
    if ((reciever_regs[11] & 0x3FF) != ((channel_whole + channel_pt - 875) / 2)) {
        displayChannel(((chan * 2) + 875), 'f');
    }
}