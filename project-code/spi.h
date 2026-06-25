/* 
 * File:   spi.h
 * Author: brennarabe
 */

#ifndef SPI_H
#define	SPI_H

/* send a character over SPI */
void sendSpi(uint8_t msg);

/* initialize SPI */
void spiInit();

#endif	/* SPI_H */

