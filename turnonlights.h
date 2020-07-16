/*
 * turnonlights.h
 *
 *  Created on: Apr 13, 2020
 *      Author: nikhitagangla
 */

#ifndef TURNONLIGHTS_H_
#define TURNONLIGHTS_H_



// this function turns the blue LED on
int led1_blue_on(void);

// this function turns the green LED on
int led2_green_on(void);

// this function turns the red LED on
int led3_red_on(void);


// this function turns the yellow LED on
int led4_yellow_on(void);

// this function plays the start sequence
int start_seq_on(void);

// this function plays the win sequence or the lose sequence, depending on the input
int winlose_seq(int won);

// this function turns the LEDs off
int led_off(void);


#endif /* TURNONLIGHTS_H_ */
