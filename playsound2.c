/*.
 * playsound2.c
 *
 *  Created on: Apr 8, 2020
 * Author: nikhitagangla
 *
 * This file contains the playsound2 function, which takes as input a string of notes, the length of the string, and the length of the tone duration, and plays the sequence.
 *
 */



#include <msp430.h>
#include "turnonlights.h"

static short tobeplayed[75]; // Array of TA1CCR0 values for set/reset. This array is populated later after values are calculated -1 is the end of the array
static int isplaying = 0; // are we playing music
static int leavelpm3 = 0; // does the ISR need to leave LPM3
unsigned int ACLK_FREQ = 12000; // Auxiliary clock is driven at 12KHz
unsigned int global_len;
unsigned int t = 0;


// this function plays the notes in sound_string when the button is pressed, and plays the sequence for the user to replicate.
void playsound2(int* sound_string, int string_length, int tone_duration) {

    global_len = string_length;
    __bis_SR_register(GIE);

    unsigned int i;

    // if the song is playing, leave low power mode
    //while (isplaying == 1) {
        //leavelpm3 = 1;
        // __bis_SR_register(LPM3_bits);
    //}
    isplaying = 1;  // set the song to be playing


    for (i = 0; i < string_length; i++) {

        if (sound_string[i] == 0){      // if there is a 0 in the string, there is a pause. make sure this is reflected
            tobeplayed[i] = 0;
        }
        else {
        tobeplayed[i] = (unsigned short)(ACLK_FREQ)/sound_string[i]; // divide the frequency by the ACLK frequency
            }
    }

    tobeplayed[i] = -1; // the last value -1 denotes the end of the song
    /*
     * Set the second timer module to fire when we expect to change notes.
     * Since the timer is driven from ACLK (12KHz) we must normalize the
     * values by multiplying by 150
     */

    TA0CCR0 = (tone_duration * 150);     //Multiply by 150 in order to get the values as quarter seconds long

    TA1CCR0 = tobeplayed[0]; // move on to the next note in the string
    TA1CCR1 = 0;
    TA1CCR2 = (tobeplayed[0] >> 1);

    // have an LED light up for the first note, so that we do not miss it
    if (tobeplayed[0] == 45){
                led1_blue_on();
            }
            else if(tobeplayed[0] == 36){
                led2_green_on();
            }
            else if(tobeplayed[0] == 30){
                led3_red_on();
            }
            else if(tobeplayed[0] == 22){
                led4_yellow_on();
             }
    t = 1;  // right now, count up to the timer value before playing the first note

    TA0CCTL0 |= CCIE;    // Enable CCR0 interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer0_isr(void) {

    if (tobeplayed[t] == -1) {      // check if we have reached the end of the song

        //led_off();
        // switch off the buzzer
        TA1CCR1 = 0;
        TA1CCR2 = 0;

        // Disable the ISR.
        TA0CCTL0 &= ~CCIE;
        TA0CCR0 = 0;

        t = 0;
        isplaying = 0; // we are no longer playing the song
    }

    else {

        // if the note to be played is certain value, the note belongs to a specific LED. turn it on as well
        if (tobeplayed[t] == 45){
            led1_blue_on();
        }
        else if(tobeplayed[t] == 36){
            led2_green_on();
        }
        else if(tobeplayed[t] == 30){
            led3_red_on();
        }
        else if(tobeplayed[t] == 22){
            led4_yellow_on();
         }

        // move on to the next note in the string
        TA1CCR0 = tobeplayed[t];
        TA1CCR1 = 0;
        TA1CCR2 = (tobeplayed[t] >> 1);

        t++;
    }
}

