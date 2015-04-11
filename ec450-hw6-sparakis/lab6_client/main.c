#include <msp430.h> 

/*
 * main.c
 */

//#define GREEN 0x40
#define RED 0x01
#define INPUT BIT4
#define SPEAKER 0x02
//Bit positions in P1 for SPI
#define SPI_CLK 0x20
#define SPI_SOMI 0x40
#define SPI_SIMO 0x80
#define ACTION_INTERVAL 1
#define BIT_RATE_DIVISOR 32
// calculate the lo and hi bytes of the bit rate divisor
#define BRLO (BIT_RATE_DIVISOR &  0xFF)
#define BRHI (BIT_RATE_DIVISOR / 0x100)


volatile unsigned char data_to_send = 0;	// current byte to transmit
volatile unsigned long tx_count = 0;		// total number of transmissions
volatile unsigned int data_received= 0; 	// most recent byte received
volatile unsigned long rx_count=0;			// total number received handler calls
volatile unsigned int period= 100;

void init_timer(void);
int main(void) {

	BCSCTL1 = CALBC1_8MHZ;    // 1Mhz calibration for clock
	DCOCTL  = CALDCO_8MHZ;

	//Intialize SPI
	  UCB0CTL1 = UCSSEL_2+UCSWRST;  		// Reset state machine; SMCLK source;
	  	UCB0CTL0 = UCCKPH					// Data capture on rising edge
	  			   							// read data while clock high
	  										// lsb first, 8 bit mode,
	  			   //+UCMST					// MASTER ONLY, DONT INCLUDE THIS IN THE SLAVE
	  			   +UCMODE_0				// 3-pin SPI mode
	  			   +UCSYNC;					// sync mode (needed for SPI or I2C)
	  	UCB0BR0=BRLO;						// set divisor for bit rate
	  	UCB0BR1=BRHI;
	  	UCB0CTL1 &= ~UCSWRST;				// enable UCB0 (must do this before setting
	  										//              interrupt enable and flags)
	  	IFG2 &= ~UCB0RXIFG;					// clear UCB0 RX flag
	  	IE2 |= UCB0RXIE;					// enable UCB0 RX interrupt
	  	// Connect I/O pins to UCB0 SPI
	  	P1SEL |=SPI_CLK+SPI_SOMI+SPI_SIMO;
	  	P1SEL2=SPI_CLK+SPI_SOMI+SPI_SIMO;


	//Intialize watchdog
	  	/*
	WDTCTL =(WDTPW +		// (bits 15-8) password
	     	                   	// bit 7=0 => watchdog timer on
	       	                 	// bit 6=0 => NMI on rising edge (not used here)
	                        	// bit 5=0 => RST/NMI pin does a reset (not used here)
	           	 WDTTMSEL +     // (bit 4) select interval timer mode
	  		     WDTCNTCL  		// (bit 3) clear watchdog timer counter
	  		                	// bit 2=0 => SMCLK is the source
	  		                	// bits 1-0 = 10=> source/512
	 			 );
	  	IE1 |= WDTIE; // enable WDT interrupt
		*/
	  	//init timer
		init_timer();  // initialize timer
		TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)

		/*
		  P1DIR &= INPUT;
		  P1DIR |= (GREEN);
		  P1OUT ^= (GREEN);
	*/


   _bis_SR_register(GIE+LPM0_bits);


	return 0;
}

void init_timer(){              // initialization and start of timer
	    TA0CTL |= TACLR;              // reset clock
		TA0CTL = TASSEL_2+ID_0+MC_1;  // clock source = SMCLK
		TA0CCTL0=0; // compare mode, output 0, no interrupt enabled
		TA0CCR0 = period-1; // in up mode TAR=0... TACCRO-1
		P1SEL|=SPEAKER; // connect timer output to pin
		P1DIR|=SPEAKER;
}


void interrupt spi_rx_handler(){
	data_received=UCB0RXBUF; // copy data to global variable
	TA0CCR0 = data_received*10;
	++rx_count;				 // increment the counter
	IFG2 &= ~UCB0RXIFG;		 // clear UCB0 RX flag
}
ISR_VECTOR(spi_rx_handler, ".int07")

/*
interrupt void WDT_interval_handler(){
  if ((P1IN & INPUT) !=0)
	{
	  P1OUT ^= (GREEN);
	}
	else
	{
		P1OUT &= ~(GREEN);
	}
}


// DECLARE WDT_interval_handler as handler for interrupt 10
ISR_VECTOR(WDT_interval_handler, ".int10")
*/


