#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 3333333
#endif

#define GPVTG "GPVTG"
#define GPRMC "GPRMC"
#define BUF_SIZE 6

volatile uint8_t find_type = 0;
volatile uint8_t find_speed = 0;
volatile uint8_t find_dir = 0;
volatile uint8_t ignore = 1;
volatile uint8_t msg_ptr = 0;
volatile uint8_t knot_ptr = 0;
volatile uint8_t dir_ptr = 0;
volatile uint8_t num_commas = 0;
volatile uint8_t rmc_message = 0;
volatile uint8_t vtg_message = 0;
volatile char msg_type[6];
volatile char knots[6];
volatile char dir[6];

volatile int speed_knots_int = 100;
volatile int cur_speed_knots = 100;
volatile int dir_int = 0;
volatile int cur_dir_int = 0;

void receive_char(char c) {
    // start of a new message. Has highest precedence.
    if (c == '$') {
        find_type = 1;
        num_commas = 0;
        ignore = 0;
        return;
    }
    // ignore spaces even if ignore = 0
    if (c == ' ' || ignore) {
        return;
    }
  
    if (c == ',') {
        num_commas++;
        if (vtg_message){
            if (num_commas == 1) {
                find_dir = 1;
            }
            else if (num_commas == 2) {
                find_dir = 0;
                dir_ptr = 0;
                dir_int = atoi((char *) dir);
                memset((char *)dir, '\0', BUF_SIZE);
            }
            else if (num_commas == 5) {
                find_speed = 1;
            }
            else if (num_commas == 6) {
                find_speed = 0;
                knot_ptr = 0;
                ignore = 1;
                uint8_t add = 0;
                for (uint8_t i = 0; i < 5; i++) {
                    if (knots[i] == '.') {
                        if (i < 4) {
                            if (knots[i + 1] == '5' || knots[i + 1] == '6' || knots[i + 1] == '7' || knots[i + 1] == '8' || knots[i + 1] == '9') {
                                add = 1;
                            }
                        }
                        knots[i] = '\0';
                        break;
                    }
                }
                speed_knots_int = atoi((char *) knots) + add;
                if (speed_knots_int > 99) {
                    speed_knots_int = 99;
                }
                memset((char *)knots, '\0', BUF_SIZE);
            }
        }
        if (rmc_message){
            if (num_commas == 9) {
                find_dir = 0;
                dir_ptr = 0;
                dir_int = atoi((char *) dir);
                memset((char *)dir, '\0', BUF_SIZE);
                ignore = 1;
            }
            else if (num_commas == 7) {
                find_speed = 1;
            }
            else if (num_commas == 8) {
                find_dir = 1;
                find_speed = 0;
                knot_ptr = 0;
                uint8_t add = 0;
                for (uint8_t i = 0; i < 5; i++) {
                    if (knots[i] == '.') {
                        if (i < 4) {
                            if (knots[i + 1] == '5' || knots[i + 1] == '6' || knots[i + 1] == '7' || knots[i + 1] == '8' || knots[i + 1] == '9') {
                                add = 1;
                            }
                        }
                        knots[i] = '\0';
                        break;
                    }
                }
                speed_knots_int = atoi((char *) knots) + add;
                if (speed_knots_int > 99) {
                    speed_knots_int = 99;
                }
                memset((char *)knots, '\0', BUF_SIZE);
            }
        }
    }
    // finding type of message
    else if (find_type) {
        // still reading message type
        if (msg_ptr < 4) {
            msg_type[msg_ptr] = c;
            msg_ptr++;
        }
        // message type 
        else {
            // put last character of type
            msg_type[msg_ptr] = c;
            // if our message is GPVTG type, we care about contents
            if (strcmp((char *) msg_type, GPVTG) == 0) {
                ignore = 0;
                vtg_message = 1;
                rmc_message = 0;
            }
            else if (strcmp ((char *) msg_type, GPRMC) == 0) {
                ignore = 0;
                rmc_message = 1;
                vtg_message = 0;
            }
            else {
                ignore = 1;
                rmc_message = 0;
                vtg_message = 0;
            }
            msg_ptr = 0;
            find_type = 0;
            memset((char *)msg_type, '\0', BUF_SIZE);
        }
    }
    // finding direction
    else if (find_dir) {
        if (c == '.' || dir_ptr > 4) {
            find_dir = 0;
            dir_ptr = 0; 
        }
        else {
            dir[dir_ptr] = c;
            dir_ptr++;
        }
    }
    // finding speed
    else if (find_speed) {
        if (knot_ptr < 5) {
            knots[knot_ptr] = c;
            knot_ptr++;
        }
        else {
            find_speed = 0;
            knot_ptr = 0;
        }
    }
}

void clear_bufs(){
    memset((char *)msg_type, '\0', BUF_SIZE);
    memset((char *)knots, '\0', BUF_SIZE);
    memset((char *)dir, '\0', BUF_SIZE);
}
