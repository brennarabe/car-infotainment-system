#ifndef F_CPU
#define F_CPU 3333333
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

// for writing display
uint8_t MPH_HALF = 35;
uint8_t HALF = 11;

// display character bitmaps
uint8_t MPH[70] = {127, 63, 28, 14, 7, 3, 7, 14, 28, 63, 127, 0,
                         127, 127, 97, 97, 97, 97, 97, 115, 63, 30, 12, 0,
                         127, 127, 1, 1, 1, 1, 1, 1, 1, 127, 127,
                         254, 254, 0, 0, 0, 128, 0, 0, 0, 254, 254, 0,
                         254, 254, 128, 128, 128, 128, 128, 128, 0, 0, 0, 0,
                         254, 254, 128, 128, 128, 128, 128, 128, 128, 254, 254};

uint8_t N[22] = {127, 63, 30, 15, 7, 3, 1, 0, 0, 127, 127,
                       254, 254, 0, 0, 128, 192, 224, 240, 120, 254, 254};
uint8_t E[22] = {63, 127, 97, 97, 97, 97, 97, 97, 97, 97, 96,
                       252, 254, 134, 134, 134, 134, 134, 134, 134, 134, 6};
uint8_t S[22] = {12, 30, 62, 119, 99, 97, 97, 112, 56, 28, 12,
                       48, 56, 28, 14, 134, 134, 198, 238, 124, 120, 48};
uint8_t W[22] = {127, 127, 0, 0, 0, 1, 0, 0, 0, 127, 127,
                       254, 252, 56, 112, 224, 192, 224, 112, 56, 252, 254};

uint8_t ZERO[22] = {15, 63, 120, 112, 96, 96, 96, 112, 120, 63, 15,
                          240, 252, 30, 14, 6, 6, 6, 14, 30, 252, 240};
uint8_t ONE[22] = {6, 12, 24, 48, 127, 127, 63, 0, 0, 0, 0,
                         6, 6, 6, 6, 254, 254, 254, 6, 6, 6, 6};
uint8_t TWO[22] = {12, 28, 56, 112, 96, 96, 97, 115, 63, 30, 12,
                         6, 14, 30, 62, 118, 230, 198, 134, 6, 6, 6};
uint8_t THREE[22] = {24, 56, 112, 97, 97, 97, 97, 97, 115, 63, 30,
                           24, 28, 14, 134, 134, 134, 134, 134, 206, 252, 120};
uint8_t FOUR[22] = {127, 127, 1, 1, 1, 1, 1, 127, 127, 1, 1,
                          128, 128, 128, 128, 128, 128, 128, 254, 254, 128, 128};
uint8_t FIVE[22] = {127, 127, 99, 99, 99, 99, 99, 99, 97, 96, 96,
                          28, 14, 6, 6, 6, 6, 6, 134, 206, 252, 120};
uint8_t SIX[22] = {31, 63, 113, 97, 97, 97, 97, 97, 113, 56, 24,
                         248, 252, 142, 134, 134, 134, 134, 134, 206, 252, 120};
uint8_t SEVEN[22] = {96, 96, 96, 96, 96, 97, 99, 103, 126, 124, 120,
                           14, 30, 56, 112, 224, 192, 128, 0, 0, 0, 0};
uint8_t EIGHT[22] = {30, 63, 115, 97, 97, 97, 97, 97, 115, 63, 30,
                           120, 252, 206, 134, 134, 134, 134, 134, 206, 252, 120};
uint8_t NINE[22] = {30, 63, 115, 97, 97, 97, 97, 97, 113, 63, 31,
                          24, 28, 142, 134, 134, 134, 134, 134, 142, 252, 248};
uint8_t CLEAR[22] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t FM[22] = {0, 0, 0, 31, 20, 20, 20, 16, 0, 0, 0, 0, 0, 0, 120, 64, 120, 64, 120, 0, 0, 0};
uint8_t DEGREE[4] = {48, 72, 72, 48};
uint8_t PT[5] = {28, 62, 62, 62, 28};

uint8_t INIT_SEQ[23] = {0xAE, 0xD5, 0x80, 0xA8, 0x2F, 0xD3, 0x0, 0xB0, 0x8D, 
                            0x14, 0xA6, 0xA4, 0xA1, 0xC0, 0xDA, 0x12, 0x81, 0x8F,
                            0xD9, 0xF1, 0xDB, 0x40, 0xAF};

// Character Positions
uint8_t MPH1[3] = {0xB5, 0x12, 0x01};
uint8_t MPH2[3] = {0xB5, 0x12, 0x0D};
uint8_t DIR1[3] = {0xB3, 0x12, 0x00}; 
uint8_t DIR2[3] = {0xB3, 0x12, 0x0C}; 
uint8_t DIR3[3] = {0xB3, 0x13, 0x08}; 
uint8_t LETTER1[3] = {0xB3, 0x14, 0x09};  
uint8_t LETTER2[3] = {0xB3, 0x15, 0x05};  
uint8_t RADIO1[3] = {0xB1, 0x12, 0x0};
uint8_t RADIO2[3] = {0xB1, 0x12, 0x0C};
uint8_t RADIO3[3] = {0xB1, 0x13, 0x08};
uint8_t RADIO4[3] = {0xB1, 0x14, 0x0A};

void displayInit() {
    // power on sequence
    PORTD.OUTSET = PIN1_bm;
    _delay_ms(5);
    PORTD.OUTCLR = PIN1_bm;
    _delay_ms(10);
    PORTD.OUTSET = PIN1_bm;
    
    // turn display off
    PORTD.OUTCLR = PIN7_bm;  // make sure d/c is set to 0
    
     // select client
    PORTA.OUTCLR = PIN7_bm;
    
    for (int i = 0; i < 23; i++) {
        sendSpi(INIT_SEQ[i]);
    }
    return;
}

void clearDisplay() {
    for (uint8_t i = 0; i < 6; i++){
        PORTD.OUTCLR = PIN7_bm;  // make sure d/c is set to 0
        sendSpi(0xB0 + i);
        sendSpi(0x10);
        sendSpi(0x0);
        PORTD.OUTSET = PIN7_bm;
        for (int j = 0; j < 128; j++) {
            sendSpi(0);
        }
    }
    return;
}

void changeBlock(uint8_t *pos, uint8_t *val){
    // fill in new values
    PORTD.OUTCLR = PIN7_bm;
    for (int i = 0; i < 3; i++){
        sendSpi(pos[i]);
    }
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < HALF; i++) {
        sendSpi(val[i]);
    }
    PORTD.OUTCLR = PIN7_bm;
    for (int i = 0; i < 3; i++){
        if (i == 0) {
            sendSpi(pos[i] - 1);
        }
        else {
            sendSpi(pos[i]);
        }
    }
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < HALF; i++) {
        sendSpi(val[i + HALF]);
    }
}

void changeBlockInt(uint8_t *pos, uint8_t num){
    if (num == 0) {
        changeBlock(pos, ZERO);
    }
    else if (num == 1) {
        changeBlock(pos, ONE);
    }
    else if (num == 2) {
        changeBlock(pos, TWO);
    }
    else if (num == 3) {
        changeBlock(pos, THREE);
    }
    else if (num == 4) {
        changeBlock(pos, FOUR);
    }
    else if (num == 5) {
        changeBlock(pos, FIVE);
    }
    else if (num == 6) {
        changeBlock(pos, SIX);
    }
    else if (num == 7) {
        changeBlock(pos, SEVEN);
    }
    else if (num == 8) {
        changeBlock(pos, EIGHT);
    }
    else if (num == 9) {
        changeBlock(pos, NINE);
    }
}

void displayChannel(int c, char mode) {
    // full mode updates entire display
    // whole updates only whole number
    // part updates only after decimal
    if (mode == 'w' || mode == 'f') {    
        if (mode == 'w') {
            c *= 10;
        }
        if ((c / 1000) != 0) {
            changeBlock(RADIO1, ONE);
            c -= 1000;
        }
        else {
            changeBlock(RADIO1, CLEAR);
        }
        int tmp = c / 100;
        changeBlockInt(RADIO2, tmp);
        c = c - (tmp * 100);
        tmp = c / 10;
        changeBlockInt(RADIO3, tmp);
        c = c - (tmp * 10);
    }
    if (mode == 'p' || mode == 'f') {
        changeBlockInt(RADIO4, c);
    }
}

void rewrite_mph(int knot_speed) {
    // perform conversion to MPH first while
    uint8_t mph = 0;
    if (knot_speed > 86) {  // this is about 99 mph
        mph = 99;
        changeBlock(MPH1, NINE);
        changeBlock(MPH2, NINE);
    }
    else {
        uint8_t tens = knot_speed / 10;
        if (tens != 0) {
            mph += (tens * 11) + 1;
            mph += ((tens - 1) / 2);
        }
        uint8_t ones = knot_speed - (tens * 10);
        mph += ones;
        if (ones >= 4) {
            mph += 1;
        }
        if ((tens & 1) && (ones >= 4) && (ones <= 6)) {
            mph -= 1;
        }
        
        // write new mph value
        if ((mph / 10) != 0){
            changeBlockInt(MPH1, (mph / 10));
        }
        else {
            changeBlock(MPH1, CLEAR);
        }
        mph = mph - ((mph / 10) * 10);
        changeBlockInt(MPH2, mph);
    }
}

void rewrite_dir(int degrees){
    // change number
    uint8_t hundreds = degrees / 100;
    uint8_t tens = (degrees - (100 * hundreds)) / 10;
    uint8_t ones = degrees - (100 * hundreds) - (10 * tens);
    if (hundreds != 0){
        changeBlockInt(DIR1, hundreds);
        changeBlockInt(DIR2, tens);
    }
    else {
        changeBlock(DIR1, CLEAR);
        if (tens != 0) {
            changeBlockInt(DIR2, tens);
        }
        else {
            changeBlock(DIR2, CLEAR);
        }
    }
    changeBlockInt(DIR3, ones);
    
    // change letters
    if (degrees <= 67 || degrees >= 293) {
        changeBlock(LETTER1, N);
        if (degrees >= 23 && degrees <= 67){
            changeBlock(LETTER2, E);
        }
        else if (degrees >= 293 && degrees <= 337) {
            changeBlock(LETTER2, W);
        }
        else {
            changeBlock(LETTER2, CLEAR);
        }
    }
    else if (degrees >= 113 && degrees <= 247) {
        changeBlock(LETTER1, S);
        if (degrees >= 203) {
            changeBlock(LETTER2, W);
        }
        else if (degrees <= 157) {
            changeBlock(LETTER2, E);
        }
        else {
            changeBlock(LETTER2, CLEAR);
        }
    }
    else if (degrees >= 248) {
        changeBlock(LETTER1, W);
        changeBlock(LETTER2, CLEAR);
    }
    else if (degrees <= 112){
        changeBlock(LETTER1, E);
        changeBlock(LETTER2, CLEAR);
    }
    else {
        changeBlock(LETTER1, CLEAR);
        changeBlock(LETTER2, CLEAR);
    }
}

void displayUnits() {
    // MPH (pages 5 & 4, start @ column 27)
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB5);
    sendSpi(0x13);
    sendSpi(0x0C);
    PORTD.OUTSET = PIN7_bm;
    for(int i = 0; i < MPH_HALF; i++) {
        sendSpi(MPH[i]);
    }
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB4);
    sendSpi(0x13);
    sendSpi(0x0C);
    PORTD.OUTSET = PIN7_bm;
    for(int i = 0; i < MPH_HALF; i++) {
        sendSpi(MPH[MPH_HALF + i]);
    }
    
    // DEGREE (start column 36)
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB3);
    sendSpi(0x14);
    sendSpi(0x04);
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < 4; i++) {
       sendSpi(DEGREE[i]);
    }
    
    // FM marker
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB1);
    // 53rd column
    sendSpi(0x15);
    sendSpi(0x05);
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < HALF; i++){
        sendSpi(FM[i]);
    }
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB0);
    // 53rd column
    sendSpi(0x15);
    sendSpi(0x05);
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < HALF; i++) {
        sendSpi(FM[i + HALF]);
    }
    // PERIOD
    PORTD.OUTCLR = PIN7_bm;
    sendSpi(0xB0);
    // 36th column
    sendSpi(0x14);
    sendSpi(0x04);
    PORTD.OUTSET = PIN7_bm;
    for (int i = 0; i < 5; i++) {
        sendSpi(PT[i]);
    }
    
    return;
}
