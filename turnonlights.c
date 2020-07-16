/*
 * turnonlights.c
 *
 *  Created on: Apr 13, 2020
 *      Author: nikhitagangla
 *
 *
 *  This file contains the function that implements the start animation start_seq_on() and end "winlose" winlose_seq() animation, the
 *  functions that turn individual LEDs on, and the function that turns all 4 LEDs off led_off().
 *
 *
 *
 *
 */

#include <msp430.h>
#include "random.h" // include random.h to access the function rand()

// initialize the variables that will store the values used to program the LEDs
char led1r = 0;
char led1g = 0;
char led1b = 0;

char led2r = 0;
char led2g = 0;
char led2b = 0;

char led3r = 0;
char led3g = 0;
char led3b = 0;

char led4r = 0;
char led4g = 0;
char led4b = 0;
int counts = 0;
int countend = 0;


// initialize start frame, end frame and led frame for the packet set
char start = 0b00000000;
char end = 0b11111111;
char global = 0b11100111;

// create table with RGB values for each of the 32 color possibilities an LED can have
char  kelvintable[4][3] = {
                             {255,0,0}, // Blue
                             {0,255,0}, // Green
                             {0,0,255}, // Red
                             {0,255,255}};


// this function turns all LEDs off.
int led_off(void){

    char led1r = 0;
    char led1g = 0;
    char led1b = 0;
    char led2r = 0;
    char led2g = 0;
    char led2b = 0;
    char led3r = 0;
    char led3g = 0;
    char led3b = 0;
    char led4r = 0;
    char led4g = 0;
    char led4b = 0;

                               UCA0TXBUF = start;
                               while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                               UCA0TXBUF = start;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = start;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = start;
                               while ((~IFG2 & 0x02)){}

                               UCA0TXBUF = global;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led1r;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led1g;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led1b;
                               while ((~IFG2 & 0x02)){}

                               UCA0TXBUF = global;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led2r;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led2g;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led2b;
                               while ((~IFG2 & 0x02)){}

                               UCA0TXBUF = global;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led3r;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led3g;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led3b;
                               while ((~IFG2 & 0x02)){}

                               UCA0TXBUF = global;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led4r;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led4g;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = led4b;
                               while ((~IFG2 & 0x02)){}


                               UCA0TXBUF = end;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = end;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = end;
                               while ((~IFG2 & 0x02)){}
                               UCA0TXBUF = end;
}


// this function plays the win animation or the lose animation, depending on its input won.
int winlose_seq(int won){
    int color;
    if (won == 1){
        color = 1;
    }
    else{
        color = 2;
    }

       countend = (countend + 1)% 4;

        if (countend == 0 || countend ==2){
            led1r = kelvintable[color][0];
            led1g = kelvintable[color][1];
            led1b = kelvintable[color][2];
            led3r = kelvintable[color][0];
            led3g = kelvintable[color][1];
            led3b = kelvintable[color][2];
            led2r = 0;
            led2g = 0;
            led2b = 0;
            led4r = 0;
            led4g = 0;
            led4b = 0;
        }
        else{
            led1r = 0;
            led1g = 0;
            led1b = 0;
            led3r = 0;
            led3g = 0;
            led3b = 0;
            led2r = kelvintable[color][0];
            led2g = kelvintable[color][1];
            led2b = kelvintable[color][2];
            led4r = kelvintable[color][0];
            led4g = kelvintable[color][1];
            led4b = kelvintable[color][2];

        }

             // to set an LED, broadcast a data stream where you have a startframe, the LED frame, then a global, and then 8 bits each for red, green and blue, and then an end frame
                            // load the series data structure into the buffer.
                           UCA0TXBUF = start;
                           while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                           UCA0TXBUF = start;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = start;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = start;
                           while ((~IFG2 & 0x02)){}

                           UCA0TXBUF = global;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led1r;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led1g;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led1b;
                           while ((~IFG2 & 0x02)){}

                           UCA0TXBUF = global;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led2r;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led2g;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led2b;
                           while ((~IFG2 & 0x02)){}

                           UCA0TXBUF = global;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led3r;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led3g;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led3b;
                           while ((~IFG2 & 0x02)){}

                           UCA0TXBUF = global;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led4r;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led4g;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = led4b;
                           while ((~IFG2 & 0x02)){}


                           UCA0TXBUF = end;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = end;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = end;
                           while ((~IFG2 & 0x02)){}
                           UCA0TXBUF = end;
                           while ((~IFG2 & 0x02)){}
                           // go into LPM3 after changing the LED color
                           __bis_SR_register(LPM3_bits + GIE);

                 return 0;
}

// this function plays the start animation.
int start_seq_on(void){

    led1r = kelvintable[counts][0];
    led1g = kelvintable[counts][1];
    led1b = kelvintable[counts][2];

            counts = (counts + 1) % 4;
               //each LED gets the color that the previos LED was, except for the first LED which gets the subsequent color in the rainbow
               led4r = led3r;
               led3r = led2r;
               led2r = led1r;

               led4g = led3g;
               led3g = led2g;
               led2g = led1g;

               led4b = led3b;
               led3b = led2b;
               led2b = led1b;

               led1r = kelvintable[counts][0];
               led1g = kelvintable[counts][1];
               led1b = kelvintable[counts][2];

                                      UCA0TXBUF = start;
                                      while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                                      UCA0TXBUF = start;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = start;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = start;
                                      while ((~IFG2 & 0x02)){}

                                      UCA0TXBUF = global;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led1r;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led1g;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led1b;
                                      while ((~IFG2 & 0x02)){}

                                      UCA0TXBUF = global;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led2r;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led2g;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led2b;
                                      while ((~IFG2 & 0x02)){}

                                      UCA0TXBUF = global;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led3r;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led3g;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led3b;
                                      while ((~IFG2 & 0x02)){}

                                      UCA0TXBUF = global;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led4r;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led4g;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = led4b;
                                      while ((~IFG2 & 0x02)){}


                                      UCA0TXBUF = end;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = end;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = end;
                                      while ((~IFG2 & 0x02)){}
                                      UCA0TXBUF = end;
                                      // go into LPM3 after changing the LED color
                         //__bis_SR_register(LPM3_bits + GIE);
return 0;
}

// this function will turn LED 4 on, in a yellow color
int led4_yellow_on(void){

    //  assign RGB values for RED led
    led1r = 0;
    led1g = 0;
    led1b = 0;

    led2r = 0;
    led2g = 0;
    led2b = 0;

    led3r = 0;
    led3g = 0;
    led3b = 0;

    led4r = kelvintable[3][0];
    led4g = kelvintable[3][1];
    led4b = kelvintable[3][2];

                         // to set an LED, broadcast a data stream where you have a startframe, the LED frame, then a global, and then 8 bits each for red, green and blue, and then an end frame
                         // load the series data structure into the buffer.
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4b;
                        while ((~IFG2 & 0x02)){}


                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        // go into LPM3 after changing the LED color
                        //__bis_SR_register(LPM3_bits + GIE);

              return 0;
}

// this function will turn LED 3 on, in a red color
int led3_red_on(void){

    //  assign RGB values for RED led
    led1r = 0;
    led1g = 0;
    led1b = 0;

    led2r = 0;
    led2g = 0;
    led2b = 0;

    led3r = kelvintable[2][0];
    led3g = kelvintable[2][1];
    led3b = kelvintable[2][2];

    led4r = 0;
    led4g = 0;
    led4b = 0;

                         // to set an LED, broadcast a data stream where you have a startframe, the LED frame, then a global, and then 8 bits each for red, green and blue, and then an end frame
                         // load the series data structure into the buffer.
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4b;
                        while ((~IFG2 & 0x02)){}


                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        // go into LPM3 after changing the LED color
                        //__bis_SR_register(LPM3_bits + GIE);

              return 0;
}






// this function will turn LED 2 on, in a green color
int led2_green_on(void){

    //  assign RGB values for RED led
    led1r = 0;
    led1g = 0;
    led1b = 0;

    led2r = kelvintable[1][0];
    led2g = kelvintable[1][1];
    led2b = kelvintable[1][2];

    led3r = 0;
    led3g = 0;
    led3b = 0;

    led4r = 0;
    led4g = 0;
    led4b = 0;

                         // to set an LED, broadcast a data stream where you have a startframe, the LED frame, then a global, and then 8 bits each for red, green and blue, and then an end frame
                         // load the series data structure into the buffer.
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4b;
                        while ((~IFG2 & 0x02)){}


                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        // go into LPM3 after changing the LED color
                       // __bis_SR_register(LPM3_bits + GIE);

              return 0;
}



// this function will turn LED 1 on, in a blue color
int led1_blue_on(void){

    //  assign RGB values for RED led
    led1r = kelvintable[0][0];
    led1g = kelvintable[0][1];
    led1b = kelvintable[0][2];

           // assign the variable of the other LEDs to zero
           led4r = 0;
           led3r = 0;
           led2r = 0;
           led4g = 0;
           led3g = 0;
           led2g = 0;
           led4b = 0;
           led3b = 0;
           led2b = 0;


                         // to set an LED, broadcast a data stream where you have a startframe, the LED frame, then a global, and then 8 bits each for red, green and blue, and then an end frame
                         // load the series data structure into the buffer.
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}  // wait until the buffer is ready to be filled again. its in the status register
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = start;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led1b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led2b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led3b;
                        while ((~IFG2 & 0x02)){}

                        UCA0TXBUF = global;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4r;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4g;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = led4b;
                        while ((~IFG2 & 0x02)){}


                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        while ((~IFG2 & 0x02)){}
                        UCA0TXBUF = end;
                        // go into LPM3 after changing the LED color

              return 0;
}






