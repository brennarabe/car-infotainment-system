#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 3333333
#endif
#define SAMPLES_PER_BIT 16
#define USART_BAUD_VALUE(BAUD_RATE) (uint16_t) ((F_CPU << 6) / (((float) SAMPLES_PER_BIT) * (BAUD_RATE)) + 0.5)

void usart_init(void) {
    //PORTC.DIR |= PIN0_bm;  // this would be the output (looks like it can be disabled for use with other devices)
    PORTC.DIR &= ~PIN1_bm;
    USART1.BAUD = (uint16_t)USART_BAUD_VALUE(9600);
    USART1.CTRLB |= USART_RXEN_bm | USART_RXMODE_NORMAL_gc;
    USART1.CTRLC |= USART_CHSIZE_8BIT_gc;
    USART1.CTRLD |= USART_DBGRUN_bm;
    USART1.CTRLA |= USART_RXCIE_bm;
}