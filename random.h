/*
 * random.h
 *
 *  Created on: Apr 13, 2020
 *      Author: nikhitagangla
 */

#ifndef RANDOM_H_
#define RANDOM_H_

// constant which is the largest value that the randomly generated number can take on
#define RAND_MAX 31;

// this function generates a random number between 0 and 32, which will be used in order to choose colors for LEDs by the function lightson() in turnonlights.c
int rand(void);

// this function changes the "state" of the random number. Helper function for rand()
void srand(unsigned int seed);

// polls the ADC to retrieve a "seed" value for rand. Helper function for rand()
unsigned int get_ADC_val(void);

// initializes the MSP430. This function is called by main()
void init2(void);

// resets the MSP430 after the game has been played
void uninit(void);


#endif /* RANDOM_H_ */
