/* 
 * File:   fm-twi.h
 * Author: brennarabe
 */

#ifndef FM_TWI_H
#define	FM_TWI_H

/* initialize TWI */
void initTWI();

/* read FM receiver registers */
int readRegs(volatile uint16_t* data);

/* write FM receiver registers */
int writeControlRegs(volatile uint16_t* data);

#endif	/* FM-TWI_H */

