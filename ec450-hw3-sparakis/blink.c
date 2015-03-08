//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430g2553.h>

// Bit masks for port 1
#define RED 0x01
#define GREEN 0x40
#define BUTTON 0x08

volatile unsigned int blink_interval;  // number of WDT interrupts per blink of LED
volatile unsigned int blink_counter;   // down counter for interrupt handler
volatile unsigned int mode; // =0 for playback =1 for record
volatile unsigned int globalCount;
volatile unsigned int buttonC[25];
volatile unsigned int buttonClickCounter;
volatile unsigned int countSinceLastButtonAction;
volatile unsigned int firstRun;
volatile unsigned int Mode;
volatile unsigned int countCount;
volatile unsigned int glob;
volatile unsigned char last_button;

int main(void) {
	  // setup the watchdog timer as an interval timer
	  WDTCTL =(WDTPW + // (bits 15-8) password
	                   // bit 7=0 => watchdog timer on
	                   // bit 6=0 => NMI on rising edge (not used here)
	                   // bit 5=0 => RST/NMI pin does a reset (not used here)
	           WDTTMSEL + // (bit 4) select interval timer mode
	           WDTCNTCL +  // (bit 3) clear watchdog timer counter
	  		          0 // bit 2=0 => SMCLK is the source
	  		          +1 // bits 1-0 = 01 => source/8K
	  		   );
	  IE1 |= WDTIE;		// enable the WDT interrupt (in the system interrupt register IE1)

	  mode =0; //playback mode initialy
	  firstRun = 0; //set 0 intialy
	  buttonClickCounter =0;
	  P1DIR |= RED;
	  P1DIR |= GREEN;


	  P1OUT &= ~(GREEN);	  //Turn off green light initialy
	  /*
	   * BUtton interupt didnt work how i intended it to.
	  P1DIR &= ~BUTTON;   // Set button pin as an input pin
	  P1OUT |= BUTTON;    // Set pull up resistor on for button
	  P1REN |= BUTTON;    // Enable pull up resistor for button to keep pin high until pressed
	  P1IES |= BUTTON;    // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	  P1IFG &= ~BUTTON;   // Clear the interrupt flag for the button
	  P1IE |= BUTTON;     // Enable interrupts on port 1 for the button
	  */
	  P1DIR &= ~BUTTON;						// make sure BUTTON bit is an input


	 	  P1OUT |= BUTTON;
	 	  P1REN |= BUTTON;
	  // initialize the state variables
	  blink_interval=67;                // the number of WDT interrupts per toggle of P1.0
	  blink_counter=blink_interval;     // initialize the counter

	  _bis_SR_register(GIE+LPM0_bits);  // enable interrupts and also turn the CPU off!
}



//This interupt is for the button presss
/* Port 1 interrupt to service the button press */
/*
 *
 * I TRIED USING an intereupt for the button, but there was to much conflict with the interrupt messing up the code
 *
 *
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  P1IFG &= ~BUTTON;                 // Clear the interrupt flag for the button

	  if(mode == 0)// Playback mode
	  {
		  	  if (Mode == 0) //Executes on button click
	 		  {
		  		 // Set the LED P1.0 to on

		  	  	  Mode  = 1;
		  	  	  P1IES &= ~BIT3; // Clear edge detect to rising edge
	 		  }
	 		  else //Executes on button press
	 		  {
	 			  glob =0;
	 			  P1OUT &= ~(GREEN);
	 			  mode = 1; //Changes to Record mode
				  Mode  = 0;
				  P1IES |= BUTTON;
	 		  }
	  }
	  else if(mode == 1) // Record mode
	  {

		  if (Mode == 0) //Executes on button press
		  {
			    buttonC[buttonClickCounter] = glob;
			    buttonClickCounter++;
			    countSinceLastButtonAction = 0;
			    glob =0;
				P1OUT ^= GREEN;              // Set the LED P1.0 to on
				Mode  = 1;
				P1IES &= ~BIT3;
		  }
		  else //Executes on button release
		  {
			buttonC[buttonClickCounter] = glob;
		    buttonClickCounter++;
			glob =0;
			countSinceLastButtonAction = 0;
			P1OUT ^= GREEN;              // Set the LED P1.0 to on
			Mode  = 0;
			P1IES |= BUTTON;
		  }
	  }

}
*/

// ===== Watchdog Timer Interrupt Handler =====
// This event handler is called to handle the watchdog timer interrupt,
//    which is occurring regularly at intervals of about 8K/1.1MHz ~= 7.4ms.

interrupt void WDT_interval_handler(){

	glob++; //Counts the number of WDT intervals

	if(mode == 0) // Playback mode
	{



		if(firstRun == 0) // in this mode the playback is waiting for user to move into record
		{
			//do nothing since there is nothing
			if (--blink_counter==0){          // decrement the counter and act only if it has reached 0
							P1OUT ^= RED;
							P1OUT ^= GREEN;// Toggle P1.0 using exclusive-OR
							blink_counter=blink_interval; // reset the down counter
				}

			unsigned char b;
			  	b= (P1IN & BUTTON);  // read the BUTTON bit
			  	if (last_button && (b==0)){ // has the button bit gone from high to low
			  		//P1OUT ^= (RED+GREEN); // toggle both LED's
			  		mode =1;
			  	}
			  	last_button=b;    // remember button reading for next time.
		}
		else // Plays back pattern
		{
			P1OUT ^= RED;
			//PLAY BACK CODE
				if(glob == buttonC[countCount])
				{
						P1OUT ^= GREEN;

					if(countCount+1 == buttonClickCounter)
					{
						countCount =0;
						buttonClickCounter =0;
						firstRun =0; 			// Comment this line if you want playback to repeat
						P1OUT &= ~(GREEN);
					}
					else
					{
						countCount++;
						globalCount =0;
						glob =0;
					}
				}
				globalCount++;

		}

	}
	else if(mode == 1)  // Record
	{
			//This blink counter is to get the red led to blink to show that it is in record mode
			if (--blink_counter==0){          // decrement the counter and act only if it has reached 0
				P1OUT ^= RED;				// Toggle P1.0 using exclusive-OR
				blink_counter=blink_interval; // reset the down counter
			}


			//GO Back to playback mode if inactive for 500 or button clicks reaches 20
			 if(countSinceLastButtonAction >= 800 || buttonClickCounter >= 20) // If these conditions are met move to playback mode
			 {
					 mode = 0;
					 glob =0;
					 firstRun = 1;
					 globalCount = 0;
					 countCount =0;
					 P1OUT &= ~(GREEN);
			 }
			 countSinceLastButtonAction++;
			 	 	 	 	 	 	 	//Detects button click and acts accordingly
			 	 	 	 	 	 	 unsigned char b;
									  	b= (P1IN & BUTTON);  // read the BUTTON bit
									  	if (last_button && (b==0)){ // has the button bit gone from high to low

									  	  buttonC[buttonClickCounter] = glob; // saves time
									  				    buttonClickCounter++; // increments array length
									  				    countSinceLastButtonAction =  0; //resets count since last click
									  				    glob =0; //resets the count
									  					P1OUT ^= GREEN; //turns on or off green button
									  	}
									  	last_button=b;    // remember button reading for next time.

	 }



}



// DECLARE function WDT_interval_handler as handler for interrupt 10
// using a macro defined in the msp430g2553.h include file
ISR_VECTOR(WDT_interval_handler, ".int10")
