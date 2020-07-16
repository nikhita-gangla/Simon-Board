#include <msp430.h>


/**
 * random.c
 *
 * This file contains the functions rand(), srand(), get_ADC_val(), and init2(). The function rand() will be called by generatesequence() in simonmain.c when randomly choosing colors
 * for the LEDs to glow.
 *
 *
 */

#define RAND_MAX 31; // this is the globally defined variable for how large the ran

unsigned int is_init = 0; // variable that states if MSP430 is initialized before random number is generated
unsigned int is_seeded = 0; // variable that states if there is a seed for rand() to use to generate a random number

unsigned int STATE = 0; // Variable which has module "scope" showing the "state" of the seed value
int counter = 0;
int n = 3; // variable which can be changed which adjusts the "filter" in get_ADC_value. This can be a integer between 1 and 5. 5 will result in the largest entropy and 1 will result in the least entropy


// this function uses an LFSR to generate a random number from the seed value, which gets stored as STATE by srand()
// this function is called by lightson() in turnonlights.c to randomly choose colors for the LEDs
int rand(void){
    unsigned int tap0, tap3; // generate variables to store value for "taps" in LFSR

    // make sure that the MSP430 has been initialized, if not, do it
    if (is_init == 0){
        init2();
    }



    //if (counter == 0){
    // srand will reseed STATE every STATE 5 times to reduce periodicity. STATE is 5 bits long
    srand(get_ADC_val());
    //}

    // tap0 is the least significant bit of STATE, and tap3 is the fourth least significant bit of STATE.
    tap0 = (STATE & 0x01); // get the 0th bit
    tap3 = ((STATE & 0x08) >> 3); // get the third bit

    //this implements the LFSR and outputs the randomly generated number .
    // XOR the taps to generate the new bit called new,, right shift STATE, add the new bit to new
    unsigned int new = tap3 ^ tap0;
    unsigned int outp  = ( (STATE << 1) | ( (STATE >> 4) ^ (STATE >> 1) & 0x00000001) ) & 0x0000001F;


    counter = (counter + 1) % 10;

    return outp;

}

// this function seeds STATE with the given seed. Used as a helper function by rand().
void srand(unsigned int seed){

    unsigned int inter;
    inter = seed;
    inter &= RAND_MAX; //bitmask the value with 31 so that the max number obtained is 31, or 11111
    STATE = inter;  // seed the current state
    is_seeded = 1; // tell the system that we have been seeded
}



// this function polls the ADC multiple times to generate a random seed.
unsigned int get_ADC_val(void){
    if (is_init){
           ADC10CTL0 |= ENC + ADC10SC; // enable and start conversion


          /*int i = 0;
           unsigned int outputadc; // variable in which the final seed value will be outputted
           while (i < n){ // poll the ADC n times. n can be increased or decreased to in turn increase or decrease the entropy of the final seed generated
               unsigned int tempint = ADC10MEM & 0x01;
               outputadc += (tempint << i); // the LSB of the ADC value is taken and concatenated. these n bits will form the  n MSBs of the final 5-bit seed value. the rest will be 0s.
               i += 1;
           }
           return outputadc;*/

           int outputadc = ADC10MEM;
           return outputadc;


       }
}


// this function initializes the MSP430. it is called in main().
void init2(void) {


      WDTCTL = WDT_ADLY_250;   // WDT 16ms, ACLK, interval timer
      IE1 |= WDTIE;           // enable watchdog timer interrupt
      BCSCTL3 |= LFXT1S_2;    // set clock to source from VLO

      ADC10CTL1 |= INCH_1 + ADC10SSEL0; // ADC sources from temperature sensor, and ACLK


      // configure P1.2 (spi clock pin)
      P1SEL |= BIT2;
      P1SEL2 |= BIT2;
      ADC10AE0 &= ~BIT2;
      CAPD &= ~BIT2;


      // configure P1.4 (data pin)
      P1SEL |= BIT4;
      P1SEL2 |= BIT4;
      ADC10AE0 &= ~BIT4;
      CAPD &= ~BIT4;

      // set up transmission for SPI serial
      UCA0CTL1 = UCSWRST;                // allow editing to occur
      UCA0CTL0 = UCSYNC + UCMST + UCCKPL + UCMSB + UCCKPH;
      UCA0CTL1 |= UCSSEL_2;
      UCA0CTL1 &= ~UCSWRST;


    // SETUP FOR PIEZOBUZZER
      // Pins 2.1 and 2.2 are connected to piezo buzzer: choose timer A module
      P2DIR |= (BIT5 + BIT1);
      P2OUT &= ~(BIT1 + BIT5);
      P2SEL |= (BIT5 + BIT1);
      P2SEL2 &= ~(BIT5 + BIT1);


      TA1CCTL1 |= OUTMOD_7; //Set/Reset Mode For Timer A register 1
      TA1CCTL2 |= OUTMOD_7; //Set/Reset Mode For Timer A register 2


      TA0CTL |= TASSEL_1 + MC_1; // Timer A0 sources from ACLK and uses CCR0 interrupt
      TA1CTL |= TASSEL_1 + MC_1; // Timer A1 sources from ACLK and uses CCR0 interrupt

// END SETUP FOR PIEZO BUZZER

    ADC10CTL0 |= SREF_1 + ADC10ON + REF2_5V + REFON; // enable interrupts, turn on the adc, enable the voltage reference generator at 2.5V
    ADC10AE0 |= BIT0; // set analog enable bit for P1.0, so we can sample using it
    //CAPD &= ~BIT0; // ensure that the port disable register is not set for P1.0
    //P1DIR &= ~BIT0;     // P1.0 is input (ADC)

    // Make sure to source from P1.0 to which ADC is connected, and use auxiliary clock for sample rate.
    ADC10CTL1 |= INCH_0 + ADC10SSEL0;



    is_init = 1; // tell system that initialization of MSP430 has occurred
}


void uninit(void){
    WDTCTL = 1;

    /*
    //WDTCTL &= ~(WDT_ADLY_250);   // WDT 16ms, ACLK, interval timer

    IE1 &= ~(WDTIE);           // enable watchdog timer interrupt
    BCSCTL3 &= ~(LFXT1S_2);    // set clock to source from VLO

    ADC10CTL1 &= ~(INCH_1 + ADC10SSEL0); // ADC sources from temperature sensor, and ACLK


    // configure P1.2 (spi clock pin)
    P1SEL &= ~(BIT2);
    P1SEL2 &= ~(BIT2);
    ADC10AE0 |= BIT2;
    CAPD |= BIT2;


    // configure P1.4 (data pin)
    P1SEL &= ~BIT4;
    P1SEL2 &= ~BIT4;
    ADC10AE0 |= BIT4;
    CAPD |= BIT4;

    // set up transmission for SPI serial
    //UCA0CTL1 = UCSWRST;                // allow editing to occur
    UCA0CTL0 = ~(UCSYNC + UCMST + UCCKPL + UCMSB + UCCKPH);
    UCA0CTL1 &= ~(UCSSEL_2);
    UCA0CTL1 |=UCSWRST;


  // SETUP FOR PIEZOBUZZER
    // Pins 2.1 and 2.2 are connected to piezo buzzer: choose timer A module
    P2DIR &= ~(BIT5 + BIT1);
    P2OUT |= (BIT1 + BIT5);
    P2SEL &= ~(BIT5 + BIT1);
    P2SEL2 |= (BIT5 + BIT1);


    TA1CCTL1 &= ~OUTMOD_7; //Set/Reset Mode For Timer A register 1
    TA1CCTL2 &= ~OUTMOD_7; //Set/Reset Mode For Timer A register 2


    TA0CTL &= ~(TASSEL_1 + MC_1); // Timer A0 sources from ACLK and uses CCR0 interrupt
    TA1CTL &= ~(TASSEL_1 + MC_1); // Timer A1 sources from ACLK and uses CCR0 interrupt

// END SETUP FOR PIEZO BUZZER

  ADC10CTL0 &= ~(SREF_1 + ADC10ON + REF2_5V + REFON); // enable interrupts, turn on the adc, enable the voltage reference generator at 2.5V
  ADC10AE0 &= ~(BIT0); // set analog enable bit for P1.0, so we can sample using it


  // Make sure to source from P1.0 to which ADC is connected, and use auxiliary clock for sample rate.
  ADC10CTL1 &= ~(INCH_0 + ADC10SSEL0);

*/

}

