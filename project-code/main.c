/* 
 * File:   main.c
 * Author: brennarabe
 */

#ifndef F_CPU
#define F_CPU 3333333
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "spi.h"
#include "fm_twi.h"
#include "oled.h"
#include "radio_control.h"
#include "gps.h"

ISR(PORTC_PORT_vect) {
    channel_pt += 2;
    if (channel_pt > 9) {
        channel_pt = 1;
    }
    _delay_ms(50);
    PORTC.INTFLAGS = PIN0_bm;
}
ISR (PORTF_PORT_vect) {
    if (muted) {
        muted = 0;
        set_volume(10);
    }
    else {
        muted = 1;
        set_volume(0);
    }
    _delay_ms(50);
    PORTF.INTFLAGS = PIN3_bm;
}

ISR(ADC0_RESRDY_vect) {
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    channel_whole = (ADC0.RES / (float)53.5);
    channel_whole += 88;
}

ISR(USART1_RXC_vect) {
    char c = USART1.RXDATAL;
    receive_char(c);
}

int main(void) {
    // initializations
    spiInit();
    displayInit();
    clearDisplay();
    displayUnits();
    usart_init();
    reciever_init();
    set_volume(10);
    init_buttons();
    clear_bufs();
    begin_reading();
    sei();
    
    // main loop
    uint8_t channel_change = 0;
    uint8_t dir_displaying = 0;
    
    while(1){
        if (speed_knots_int == 0 && dir_displaying) {
            changeBlock(DIR1, CLEAR);
            changeBlock(DIR2, CLEAR);
            changeBlock(DIR3, CLEAR);
            changeBlock(LETTER1, CLEAR);
            changeBlock(LETTER2, CLEAR);
            dir_displaying = 0;
        }
        if (dir_int != cur_dir_int && speed_knots_int != 0) {
            rewrite_dir(dir_int);
            cur_dir_int = dir_int;
            dir_displaying = 1;
        }
        if (speed_knots_int != cur_speed_knots) {
            rewrite_mph(speed_knots_int);
            cur_speed_knots = speed_knots_int;
        }
        if (channel_whole != previous_channel_whole) {
            previous_channel_whole = channel_whole;
            displayChannel(channel_whole, 'w');
            channel_change = 1;
        }
        if (channel_pt != previous_channel_pt) {
            previous_channel_pt = channel_pt;
            displayChannel(channel_pt, 'p');
            channel_change = 1;
        }
        if (channel_change) {
            channel_change = 0;
            change_channel((channel_whole * 10) + channel_pt);
        }
    }
    return (EXIT_SUCCESS);
}
