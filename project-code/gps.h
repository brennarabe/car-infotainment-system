/* 
 * File:   gps.h
 * Author: brennarabe
 */

#ifndef GPS_H
#define	GPS_H

/* sort through UART messages */
void receive_char(char c);

/* initialize buffers */
void clear_bufs();

extern volatile int speed_knots_int;
extern volatile int cur_speed_knots;
extern volatile int dir_int;
extern volatile int cur_dir_int;

#endif	/* GPS_H */

