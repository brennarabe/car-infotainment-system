/* 
 * File:   channel_select.h
 * Author: brennarabe
 */

#ifndef CHANNEL_SELECT_H
#define	CHANNEL_SELECT_H

extern volatile uint16_t reciever_regs[16];

extern volatile int channel_whole;
extern volatile int previous_channel_whole;
extern volatile int channel_pt;
extern int previous_channel_pt;
extern volatile uint8_t muted;

/* initialize potentiometer */
void begin_reading();

/* initialize buttons*/
void init_buttons();

/* change volume */
void set_volume(uint8_t vol);

/* power on radio and set initial settings */
void reciever_init();

/* change the channel */
void change_channel(int chan);



#endif	/* CHANNEL_SELECT_H */

