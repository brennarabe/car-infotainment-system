#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 3333333
#endif

#define SPI_WAIT() while (!(SPI0.INTFLAGS & SPI_IF_bm))

void sendSpi(uint8_t msg) {
    SPI0.DATA = msg;
    SPI_WAIT();
    return;
}

void spiInit() {
    // Max clock frequency for display is 10MHz
    PORTD.DIR |= PIN1_bm;  // set RST pin direction to output
    PORTD.DIR |= PIN7_bm;  // set D/C pin direction to output
    PORTA.DIR |= PIN4_bm;  // set MOSI pin direction to output
    PORTA.DIR |= PIN6_bm;  // set SCK pin direction to output
    PORTA.DIR |= PIN7_bm;  // set SS pin direction to output
    SPI0.CTRLA = SPI_CLK2X_bm         // double speed
                | SPI_ENABLE_bm       // Enable
                | SPI_MASTER_bm       // Host mode
                | SPI_PRESC_DIV4_gc;   // System clock divided by 4
    return;
}
