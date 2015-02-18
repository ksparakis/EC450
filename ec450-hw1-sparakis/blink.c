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

#include <msp430.h>				

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

	volatile unsigned int unit = 20000;
	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		//FLASH S
		P1OUT ^= 0x01; //turn on
		for(i=0; i<unit; i++)
		{
						// SW Delay for
		}

		P1OUT ^= 0x01;

		for(i=0; i<unit; i++)
		{
										// SW Delay for
		}


			P1OUT ^= 0x01; //turn on
			for(i=0; i<unit; i++)
			{
							// SW Delay for
			}

			P1OUT ^= 0x01;

			for(i=0; i<unit; i++)
			{
											// SW Delay for
			}

				P1OUT ^= 0x01; //turn on
				for(i=0; i<unit; i++)
				{
								// SW Delay for
				}


				P1OUT ^= 0x01;
				//PAUSE FOR LETTER
				for(i=0; i<unit*3; i++)
				{
												// SW Delay for
				}



		//FLASH O
				P1OUT ^= 0x01; //turn on
						for(i=0; i<unit*3; i++)
						{
										// SW Delay for
						}

						P1OUT ^= 0x01;

						for(i=0; i<unit; i++)
						{
														// SW Delay for
						}


							P1OUT ^= 0x01; //turn on
							for(i=0; i<unit*3; i++)
							{
											// SW Delay for
							}

							P1OUT ^= 0x01;

							for(i=0; i<unit; i++)
							{
															// SW Delay for
							}

								P1OUT ^= 0x01; //turn on
								for(i=0; i<unit*3; i++)
								{
												// SW Delay for
								}


								P1OUT ^= 0x01;
								//PAUSE FOR LETTER
								for(i=0; i<unit*3; i++)
								{
																// SW Delay for
								}


		//FALSH S


								P1OUT ^= 0x01; //turn on
								for(i=0; i<unit; i++)
								{
												// SW Delay for
								}

								P1OUT ^= 0x01;

								for(i=0; i<unit; i++)
								{
																// SW Delay for
								}


									P1OUT ^= 0x01; //turn on
									for(i=0; i<unit; i++)
									{
													// SW Delay for
									}

									P1OUT ^= 0x01;

									for(i=0; i<unit; i++)
									{
																	// SW Delay for
									}

										P1OUT ^= 0x01; //turn on
										for(i=0; i<unit; i++)
										{
														// SW Delay for
										}


										P1OUT ^= 0x01;
										//PAUSE FOR WORD
										for(i=0; i<unit*7; i++)
										{
																		// SW Delay for
										}


	}
	
	return 0;
}
