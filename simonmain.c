#include <msp430.h> 
#include "random.h"
#include "playsound2.h"
#include "turnonlights.h"

/**
 * This function contains all the logic for the Simon game to run, and the helper functions that implement and change the data that it continuously polls.
 * It contains the functions main, simon_logic, generatecurrsequence, soundlightonpress_and_check, initgame, generatesequence and convertnumstosound.
 *
 */

// change this to change the length of the sequence required to win. The max length allowable is 30. The smallest length allowable is 3.
static const int sequencelength = 6;

// change this to change the time limit before the game times out. 5 currently results in a time limit of 1.25 seconds, increasing or decreasing this by one will respectively
// increase or decrease the timelimit by .25 seconds.
int timelimit = 5;



int soundtable[4] = {262, 330, 392, 524}; // table that contains the Hz frequency values for the notes for the 4 distinct buttons

// to be passed into playsound to play each of the button tones
int button1[7] = {262,262,262,262,262,262,262};
int button2[7] = {330,330,330,330,330,330,330};
int button3[7] = {392,392,392,392,392,392,392};
int button4[7] = {524,524,524,524,524,524,524};


int prearray;                                   // used to generate random numbers by function generatesequence
unsigned int seqgenerated[sequencelength];      // contains the sequence of buttons to be played for the particular game
int songnotes[2*sequencelength];                // contains the notes of tones that system must play for the particular game
int TIME_OUT;                                   // indicates whether timeout has happened
int WAITING;                                    // flag indicating that the program is currently waiting for the player to press a button
int j = 0;
int BTTNON = 0;                                 // indicates whether button has been pressed
int won = 0;                                    // indicates whether game has been won
int correctguess;                               // tells us if guess is correct
int is_animation = 0;                           // indicates if we are in animation or no
int currsequence[sequencelength];               // contains the sequence slice that the player is currently being tested on
int game_end = 0;                               // indicates whether the game is over
int test = 0;
int check_array[sequencelength];                // contains all the buttons pressed by the user on their guess

void main(void)
{
    // run loop forever
    while (1){
    init2(); // initialize MSP430 for LEDs, adc and buzzer
    simon_logic(); // play the game

    }
}

// This function implements the logic for Simon.
void simon_logic(void){
    BTTNON = 0;
    unsigned int v;
        v = 3; // this is the smallest that the sequence that the player must guess can be

        initgame(); // initialize the game. sequencegenerated is now the global variable that contains the sequence for this round of the game

        while (P2IN == 0x1D){ // the start animation will keep playing until the user presses a button
            start_seq_on();
            __bis_SR_register(LPM3_bits + GIE); // go into low power mode
        }
            led_off(); // turn off the LEDs after start animation

            while (BTTNON != 0x0){ // sit in low power mode until the button is released, giving time for the user to respond
                __bis_SR_register(LPM3_bits + GIE);
            }

        while (v <= sequencelength){ // increment the slice length by 1 every time

            generatecurrsequence(v);   // generate a sequence that is the first v elements of the game sequence, stored in global variable currsequence.
            convertnumstosound(v); // generate the notes for the song that has the first v elements of the game sequence, stored in global variable songnotes

           playsound2(songnotes,2*sequencelength,15); //play currentsequence for the user


           // make sure that check_array sequence (what will store the guess that the user will make) is empty
           int c;
           for (c = 0; c < sequencelength; ++c){
                check_array[c] = 0;
           }

           //while (BTTNON == 0x0){ // the start animation will keep playing until the user presses a button
              // __bis_SR_register(LPM3_bits + GIE);
               //     }
           __delay_cycles(20000); // give time for user to respond

           // let the player guess, and check that the guess was correct
           if (soundlightonpress_and_check(v) == 0){ // if the player did it wrong, then player loses
               won = 0; // mark the end state as lost
               game_end = 1; // mark the game as ended
               break;
           }
           v++;

           // go into low power mode while the button is pressed
           while (BTTNON != 0x0){ // ensure button state for next loop
               __bis_SR_register(LPM3_bits + GIE);
           }

           /* ALTERNATIVE WAY THAN BELOW
           while (BTTNON != 0x0){ // the start animation will keep playing until the user presses a button
               __bis_SR_register(LPM3_bits + GIE);
           }
           for (j = 0; j < 2; ++j){ // same thing as delay cycles
               __bis_SR_register(LPM3_bits + GIE);
           }
*/
           __delay_cycles(1000000); // give time for user to respond
           //BTTNON = 0;
       }

      if (v > sequencelength){ // if the player passes all the rounds, then wins
          won = 1; // mark end state as 1
          game_end = 1; // mark game as over
      }

      // clear songnotes array that was used to play sequence to user
      int clc = 0;
      int clc2 = 0;
      while (clc < 2*sequencelength){
          songnotes[clc] = 0;
          clc += 1;
      }

      // clear currsquence and clear check array for next round
      while (clc2 < sequencelength){
          seqgenerated[clc2] = 0;
          currsequence[clc2] = 0;
          check_array[clc2] = 0;
          clc2 += 1;
      }

      // go into low power mode while the button is pressed
      while (BTTNON != 0x0){ // ensure button state for next loop
          __bis_SR_register(LPM3_bits + GIE);
      }

      // as long as no button is pressed, win/lose sequence will keep playing
      while (P2IN == 0x1D){ // the end animation will keep playing until the user presses a button, and then it will loop back to the start screen
          winlose_seq(won);
      }

      // ensure that no button is currently pressed before restarting
      while (P2IN != 0x1D){
      }

      BTTNON = 0;
}



// this function should take as input the sequence length, v and generate the sequence slice for that round of the game
void generatecurrsequence(int currlength){

    // make sure that curr_sequence is first entirely zeros
    int c = 0;
    while (c < sequencelength){
        currsequence[c] = 0;
        c += 1;
     }

    // fill currsequence with only the first v values of seqgenerated
   int cr = 0;
   while (cr < currlength){
       currsequence[cr] = seqgenerated[cr];
       cr += 1;
   }
}

// this function waits for the user to press buttons for a particular sequence, and checks that the buttons pressed are correct
int soundlightonpress_and_check(int currsequencelen){

    int difference = 0; // initialize flag
    int iter = 0;       // initialize the iteration

    // for the ath button press by the user, add the button id (1,2,3 or 4) into the ath positon of the array. else, have 0
    while(iter < currsequencelen){
        TIME_OUT = 0;                           // reset the time_out counter to be 0
        WAITING = 1;                            // set flag saying that program is waiting for user to press a button
        WDTCTL = WDT_ADLY_250;                  // clear the watchdog timer. (counter goes to 0 and resets it)
        while (BTTNON == 0 && TIME_OUT < timelimit){    // sit in LPM if no button is pressed, until we time out. the way the ISR is set, you just keep going in and out of LPM
            __bis_SR_register(LPM3_bits + GIE);
        }
        WAITING = 0;                            // after coming out of LPM by either timeout or button press, we are no longer waitng
        if (TIME_OUT >= timelimit){                     // if we timed out, immediately return 0, which states that user guess is wrong
            return 0;
        }

        // if a particular button is pressed, reset time_out, switch on the LED for that button, play the tone for that button, and add the press into the check_array
        while (BTTNON == 1){
            TIME_OUT = 0;
            led1_blue_on();
            playsound2(button1,1,1);
            check_array[iter] = 1;

        }
        while (BTTNON == 4){
            TIME_OUT = 0;
            led2_green_on();
            playsound2(button2,1,1);
            check_array[iter] = 2;

        }

        while (BTTNON == 8){
            TIME_OUT = 0;
            led3_red_on();
            playsound2(button3,1,1);
            check_array[iter] = 3;

        }

        while (BTTNON == 16){
            TIME_OUT = 0;
            led4_yellow_on();
            playsound2(button4,1,1);
            check_array[iter] = 4;

        }

        iter = iter + 1;
        // while check_array is being populated, do for loop and check if each element in the array is the same. if different, flag = 1.
        // this means that the function will be able to determine if the player is wrong immediately after the wrong button is pressed
        int iter2 = 0;
        while (iter2 < currsequencelen){
                if ((currsequence[iter2] != check_array[iter2]) && (check_array[iter2] != 0)){
                    difference = 1;
                    return (difference ^1);
                }
                iter2 += 1;
            }
}
       // if difference = 1, then return 0 (wrong guess). if difference is 0, return 1 (right guess).
       return (difference ^ 1);
}

// button ISR
#pragma vector=PORT2_VECTOR
__interrupt void button_press_isr(void){

    // if the game has ended and the button is pressed, don't bother toggling anything, just set the interrupt flag and game end to 0 and leave LPM
    if (game_end == 1){
        game_end = 0;
    }

    else { // if  a button is pressed, toggle the edge select, and toggle button on. what this does is it ensures that the buttonon is only high when button is held down
    // if button 1 is pressed
    if (P2IFG & BIT0){
        P2IES ^= BIT0;
        BTTNON ^= BIT0;
    }

    // if button 2 is pressed
    else if (P2IFG & BIT2){
        P2IES ^= BIT2;
        BTTNON ^= BIT2;

    }

    // if button 3 is pressed
    else if (P2IFG & BIT3){
        P2IES ^= BIT3;
        BTTNON ^= BIT3;
    }

    // if button 4 is pressed
    else if (P2IFG & BIT4){
        P2IES ^= BIT4;
        BTTNON ^= BIT4;
    }

    }

    // clear interrupt flag and leave LPM
    P2IFG  &= 0x0;
    __bic_SR_register_on_exit(LPM3_bits);
}


// this function will initialise the buttons for the game
void initgame(void){

    // Make P2.0, 2.2, 2.3, and 2.4 input pins
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4);

    // enable button interrupts
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4;
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4;

    // set interrupt edge to be high to loq
    P2IES |= BIT0 + BIT2 + BIT3 + BIT4;
    P2IE  |= BIT0 + BIT2 + BIT3 + BIT4;

    // generate the sequence to be played for the particular game.
    generatesequence(sequencelength);
    BTTNON = 0;

}

// this function will generate the sequence that the game will use.
void generatesequence(int sequencelength){
    int i = 0;
    int separate = 0;
    while (i < sequencelength){
       separate++;
       if ((separate % 150) ==0){ // this keeps the ADC polls relatively far apart so that the same numeber isnt picked again and again, resulting in a boring sequence
           prearray = rand(); // pick the random number
           int num = ((prearray & 0x03) + 1); // add the random number to the sequence, but make sure that it is between 1-4 inclusive
           seqgenerated[i] = num;
          i++;
       }
    }
}

// this convert all the button presses to the frequencies that the buzzer should play
void convertnumstosound(int seqlen){

    // clear songnotes array
    int clr = 0;
    while (clr < 2*seqlen){
        songnotes[clr] = 0;
        clr = clr += 1;
    }

    int i = 0; // go through the loop and convert the presses to the respective frequencies
    while (i<seqlen){
        int tone = currsequence[i];
        songnotes[2*i] = soundtable[tone-1];
        i = i + 1;
    }

}


// Watchdog Timer ISR
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
   if (WAITING == 1){ // if we are currently waiting for the user to press a button,
     TIME_OUT += 1; // every cycle, add 1 to time out
   }
    // Simply exit LPM3.
    __bic_SR_register_on_exit(LPM3_bits);
}

