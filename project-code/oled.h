/* 
 * File:   oled.h
 * Author: brennarabe
 */

#ifndef OLED_H
#define	OLED_H

/* change displayed character at pos to val */
void changeBlock(uint8_t *pos, uint8_t *val);

/* change displayed character at pos to num*/
void changeBlockInt(uint8_t *pos, uint8_t num);

/* update displayed radio station */
void displayChannel(int c, char mode);

/* clear entire display */
void clearDisplay();

/* display a new speed (and convert from knots to mph) */
void rewrite_mph(int knot_speed);

/* rewrite the direction. Calculate N/E/S/W/NE/NW/SE/SW using degrees */
void rewrite_dir(int degrees);

/* initialize the display with starting characters */
void displayUnits();

/* initialize the display */
void displayInit();

extern uint8_t CLEAR[22];


// Character Positions
extern uint8_t MPH1[3];
extern uint8_t MPH2[3];
extern uint8_t DIR1[3]; 
extern uint8_t DIR2[3]; 
extern uint8_t DIR3[3];
extern uint8_t LETTER1[3];  
extern uint8_t LETTER2[3];  
extern uint8_t RADIO1[3];
extern uint8_t RADIO2[3];
extern uint8_t RADIO3[3];
extern uint8_t RADIO4[3];

#endif	/* OLED_H */

