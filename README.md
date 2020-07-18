# Simon Game
This is my version of the classic [Simon game] which tests the player's memory skill. The hardware for the game is laid out on a PCB designed using EAGLE, while the C code (stored on a MSP430 power microcontroller) runs the gameplay.
![IMG_2722](https://user-images.githubusercontent.com/62456147/87822116-7b2d1d00-c8a3-11ea-889e-653f60676ee5.jpg)

## Video Demo
Watch the [video demo] to see the homemade version of Simon in action.

## Technologies
* C
* Code Composer Studio
* EAGLE

## Hardware 
* TI MSP430 Power Microcontroller
* DotStar Addressable RGB LED

## Gameplay
Upon turning on, the game will have a "start" animation consisting of 4 LED colors rotating through the LEDs, which will continue playing until the player presses any button. After a button is pressed, the game will immediately start. A sequence of length 3 will be played, and then when the player inputs the correct sequence using the buttons, the same sequence with one additional tone at the end will be played. This pattern will continue until the player either eventually correctly plays a sequence of `sequencelength`, which can be changed easily at the beginning of the simonmain.c file, or if the player loses by playing an incorrect tone (the player will lose immediately an incorrect button is pressed). The player could also lose if they wait too long to press a button (more than 1.25s), in which case they lose by "timing out". The duration that results in a timeout can be increased or decreased easily in the beginning of the simonmain.c file, by changing the variable `timelimit`.

There will both be "win" animations and "lose" animations which will be the green and red LEDs flashing on the board respectively. When either the "win" or the the "lose" animation is playing, the player can exit the animation by pressing any button, which will bring them back to the "start" animation, repeating the cycle.

## Board Design
The board is designed on a single-layer PCB using EAGLE. The board contains 4 RGB LEDS with 4 corresponding buttons, and they are all connected to an MSP430 powercontroller, which controls their on/off state, and in the case of the LEDs, the data being sent to them. The MSP430 also has a piezobuzzer which can  be buzzed at different frequencies, and an ADC which allows us to control the brightness and color of the LEDS. Below is the button layout, with connection to the MSP430 ports. The button S1 is connected to P2.0 of the MSP 430, the button S2 is connected to P2.2, S3 is connected to P2.3 and S4 is connected to P2.4.

    /* BUTTON LAYOUT */
    /* (P2.2) S2 --------- S3 (P2.3) */
    /*         |            |        */
    /*         |            |        */
    /* (P2.0) S1 --------- S4 (P2.4) */

## Code Architecture
The code is separated into `simonmain.c` (which contains the function main()), `random.c`, `turnonlights.c`, and `playsound.c`.

### simonmain.c
simonmain.c is the main driver file for the game. The `main()` function first calls `init2()`, which initializes the MSP430, and `simon_logic()` which actually implements the logic for the game by calling on other functions. These two functions are continuously called in a never-ending while loop, so the game will play forever, until the board is powered off. 

`simon_main()` initializes the buttons on the board by calling `initgame()`, which generates the sequence to be played for the specific game using the function `generatesequence()`. Next, starting from a slice of length 3, `simonmain()` generates the current slice to be played using `generatecurrseuqnce()` and then creates an array which can be played using `convertnumstosound()`, which changes a global array variable which will be passed into `playsound2()` to play. After the buzzer plays the sequence for the user, the function `soundlightonpress_and_check()` is called, which not only registers the button sequence and makes the buttons that the user presses both have a corresponding light and sound, but also checks that the sequence pressed by the user is correct, and finally, implements the timeout so that if the user takes too long to press a button, the user loses the game. If the user has gotten the sequence correct, a longer slice starts playing, or if they already played the maximum sequence length, the flag won is changed to won, and the `winlose_seq()` function is called with won as the input, which plays the win animation. If the player loses by either playing the wrong sequence or waiting too long to press the button, the variable won is set to zero, and the `winlose_seq()` function is played with lose as the input, this time resulting in the lose animation being played. The animation will play as long as there is no button press registered, upon when `simon_logic()` will restart.

A couple of points to note: in order to give the user time to respond between pressing the buttons and seeing the end sequences, the MSP430 is put into LPM, which gets kicked when a button is pressed. Button presses are mainly registered using interrupts. As such, there is a Port 2 ISR for the MSP430.

### random.c
`random.c` contains the user-implemented function `rand()`, which generates a random number. This random number is used by `generatesequence()` in `main()` to generate a random sequence for the player.

### playsound2.c
`playsound2.c` contains the function `playsound2()`, which is called by `simon_logic()` whenever a sequence needs to be played, and is called by `soundlightonpress_and_check()` so that the corresponding tone is played and LED lights up whenever the user presses a button.

### turnonlights.c
`turnonlights.c` contains the functions `led1_blue_on()`, `led2_green_on()`, `led3_red_on()`, `led4_yellow_on()`, which is called by `soundlightonpress_and_check()` so that a single LED is lit up when its corresponding button is pressed. The file also contains the function `start_seq_on()`, which plays the start animation, `led_off()`, which turns the LEDs off so that there are visual breaks and cues for the user while playing Simon, and `winlose_seq()`, which plays the win animation or lose animation, depending on the input to it. These 3 functions are all called by `simon_logic()`.

## Notes
The game can be made harder or easier by reducing or increasing the time the user has to press a button before the game times out. The default is set to 1.25 seconds, and the variable called `timelimit` has been set to 5. Increasing or reducing this variable by 1 would increase or decrease the variable by 0.25 seconds. 

The length of the sequence required to win can be changed, by changing the variable `sequencelength` at the beginning of the simonmain.c file. The smallest length allowable for this variable is 3, and the largest length allowable for this variable is 30.


## References
* [MSP430G2553 Datasheet]
* [MSP430x2xx Family User's Guide]

   [Simon game]: https://en.wikipedia.org/wiki/Simon_(game)
   [video demo]: https://drive.google.com/file/d/1mtxt7HwAMOKZ6hkkJ4arVjRnBYrZxwgK/view
   
   [MSP430G2553 Datasheet]: http://elec327.github.io/assets/documents/msp430g2553.pdf
   [MSP430x2xx Family User's Guide]: http://elec327.github.io/assets/documents/slau144j_userguide.pdf
