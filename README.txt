# Simon Game
This is my version of the classic [Simon game] which tests the player's memory skill.

*** GAMEPLAY ***
Upon turning on, the game will have a "start" animation consisting of 4 LED colors rotating through the LEDs, which will continue playing until the player presses any button. After a button is pressed, the game will immediately start. A sequence of length 3 will be played, and then when the player inputs the correct sequence using the buttons, the same sequence with one additional tone at the end will be played. This pattern will continue until the player either eventually correctly plays a sequence of (sequencelength), which can be changed easily at the beginning of the simonmain.c file, or if the player loses by playing an incorrect tone (the player will lose immediately an incorrect button is pressed). The player could also lose if they wait too long to press a button (around .5 s), in which case they lose by "timing out".

There will both be "win" animations and "lose" animations which will be the green and red LEDs flashing on the board respectively. When either the "win" or the the "lose" animation is playing, the player can exit the animation by pressing any button, which will bring them back to the "start" animation, repeating the cycle.


*** (a) CODE ARCHITECTURE ***
The code is separated into simonmain.c (which contains main), random.c, turnonlights.c, and playsound.c.

simonmain.c is the main driver function for the game. The main() function first calls init2(), which initializes the MSP430, and simon_logic() which actually implements the logic for the game by calling on other functions. These two functions are continuously called in a never-ending while loop, so the game will play forever. simon_main() initializes the buttons on the board by calling initgame(), which generates the sequence to be played for the specific game using the function generatesequence(). Next, starting from a slice of length 3, simonmain() generates the current slice to be played using generatecurrseuqnce() and then creates an array which can be played using convertnumstosound(), the which changes a global array variable which will be passed into playsound2() to play. After the buzzer plays the sequence for the user, the function soundlightonpress_and_check() is called, which not only registers the button sequence and makes the buttons that the user presses both have a corresponding light and sound, but also checks that the sequence pressed by the user is correct, and finally, implements the timeout so that if the user takes too long to press a button, the user loses the game. If the user has gotten the sequence correct, a longer slice starts playing, or if they already played the maximum sequence length, the flag won is changed to won, and the winlose_seq() function is called with won as the input, which plays the win animation. If the player loses by either playing the wrong sequence or waiting too long to press the button, the variable won is set to zero, and the winlose_seq() function is played with won as the input, this time resulting in the lose animation being played. The animation will play as long as there is no button press registered, upon when simon_logic()

A couple of points to note: in order to give the user time to respond between pressing the buttons and seeing the end sequences, the MSP430 is put into LPM, which gets kicked when a button is pressed. Button presses are mainly registered using interrupts. As such, there is a Port 2 ISR for the MSP430.

random.c contains the function rand(), which generates a random number. This random number is used by generatesequence() in main to generate a random sequence for the player.

playsound2.c contains the function playsound2(), which is called by simon_logic() whenever a sequence needs to be played, and is called by soundlightonpress_and_check() so that a tone is played whenever the user presses a button.

turnonlights.c contains the functions led1_blue_on(), led2_green_on(), led3_red_on(), led4_yellow_on(), which is called by soundlightonpress_and_check() so that a single LED is lit up when its corresponding button is pressed. The file also contains the function start_seq_on(), which plays the start animation, led_off(), which turns the LEDs off so that there are visual breaks and cues for the user while playing Simon, and winlose_seq(), which plays the win animation or lose animation, depending on the input to it. These 3 functions are all called by simon_logic().


*** (b) RUBRIC & SCORING ***
Based on the rubric below, I think that I should get a 130/130, and however much extra because, as bonus, I will be submitting a video on the project, and have also included an option to make the timeout shorter or longer in order to make the game harder or easier. I have documented this functionality in section (c) of this document.

1. MVP (minimally viable product): Does it actually run properly off of a battery and (occasionally) perform in a Simon-esque way? (30 pts)

Yes, the code does run properly off of a battery and performs in a Simon-esque way.

2. Gameplay - ButtonFeedback (when a button is pressed during play, does the light and sound asssociated with that button track the button press, i.e., start when it is depressed and stop when it is released?)

Yes, this does happen. Every button has a specific LED color and tone frequency that shows whenever pressed, or whenver the Simon game plays a sequence for the user to replicate.

3. Gameplay - Timeout (does it game over after a while if no button is pressed?)

Yes, the game times out after a while if no button is pressed. The default is 1.25 seconds, but this can be changed easily at the top of the simonmain.c file. Instructions and details are given below in section (c) of this file.

4. Gameplay - MisplayingError (does it game over if the wrong button is pressed?)

Yes, the game immediately games over when the wrong button is pressed (the player does not get to press anymore buttons after their first wrong button in the sequence)

5. Gameplay - Win (does it do something special when the player wins?)

Yes, the game shows a flashing green win animation when the player wins. 

6. Gameplay - Random (does it play a different sequence each time, including after power cycles?)

Yes, the game plays different sequences each time, including after power cycles.

7. Style - Animations (are there animations for start/win and lose/game-over?)

Yes, there is a start animation with 4 different LED colors rotating around the board. Depending on whether the player has won or lost, the LEDs will flash green or red respectively.

8. Style - SoundLight (do lights and sounds work well together and convey information?)

Yes, the lights and sounds work well together and convey information. During gameplay, lights and sounds only appear when a button is pressed or a tone is played for the user to replicate, and each LED has its own dedicated color, and its own dedicated tone frequency so as not to confuse the user. When the player has won, the LEDs flash green as a cue to let the player know that they have won. When the player has lost, the LEDs flash red as a cue to let the player know that they have lost. 

9. Is it easy to change the length of a sequence required to win?

Yes, it is easy to change the length of a sequence required to win. It is possible to do so by changing the variable sequencelength at the beginning of the simonmain.c file. Details and instructions are given in section (c) of this file. 

10. Well - commented, logical code architecture (does the architecture make sense? are there good comments?, functions (libraries?) used intelligently?) - 20 pts

Yes, I believe that the code architecture makes sense, and has been commented well.

*** (c) ACCESS & RUNNING OF SPECIAL FEATURES ***
The game can be made harder or easier by reducing or increasing the time the user has to press a button before the game times out. The default is set to 1.25 seconds, and the variable called timelimit has been set to 5. Increasing or reducing this variable by 1 would increase or decrease the variable by 0.25 seconds. 

The length of the sequence required to win can also be changed easily, by changing the variable sequencelength at the beginning of the simonmain.c file. The smallest length allowable for this variable is 3, and the largest length allowable for this variable is 30.







   [Simon game]: https://en.wikipedia.org/wiki/Simon_(game)
