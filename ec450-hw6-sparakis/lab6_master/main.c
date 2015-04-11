#include <msp430.h> 

/*
 * main.c
 */

//#define GREEN 0x40
#define RED 0x01
#define INPUT BIT4

// Define bit masks for ADC pin and channel used as P1.4
#define ADC_INPUT_BIT_MASK 0x10
#define ADC_INCH INCH_4
// bitrate = 1 bit every 4 microseconds
#define ACTION_INTERVAL 1
#define BIT_RATE_DIVISOR 32
//Bit positions in P1 for SPI
#define SPI_CLK 0x20 //1.5
#define SPI_SOMI 0x40 //1.6
#define SPI_SIMO 0x80 //1.7

// calculate the lo and hi bytes of the bit rate divisor
#define BRLO (BIT_RATE_DIVISOR &  0xFF)
#define BRHI (BIT_RATE_DIVISOR / 0x100)

 /* declarations of functions defined later */
 void init_spi(void);

// Global variables and parameters (all volatilel to maintain for debugger)
volatile unsigned int action_counter=ACTION_INTERVAL;
volatile unsigned int data_to_send = 0;        // current byte to transmit
volatile unsigned long tx_count = 0;            // total number of transmissions
volatile unsigned int data_received= 0;        // most recent byte received
volatile unsigned long rx_count=0;                      // total number received handler calls
volatile unsigned int photoDiodeValue;

int main(void) {
	   BCSCTL1 = CALBC1_8MHZ; // 8Mhz calibration for clock
	     DCOCTL = CALDCO_8MHZ;

	     // setup the watchdog timer as an interval timer
	     // INTERRUPT NOT YET ENABLED!


	     ADC10CTL1= ADC_INCH //input channel 4
	     +SHS_0 //use ADC10SC bit to trigger sampling
	     +ADC10DIV_4 // ADC10 clock/5
	     +ADC10SSEL_0 // Clock Source=ADC10OSC
	     +CONSEQ_0; // single channel, single conversion
	     ;
	     ADC10AE0=ADC_INPUT_BIT_MASK; // enable A4 analog input
	     ADC10CTL0= SREF_0 //reference voltages are Vss and Vcc
	     +ADC10SHT_3 //64 ADC10 Clocks for sample and hold time (slowest)
	     +ADC10ON //turn on ADC10
	     +ENC //enable (but not yet start) conversions
	     +ADC10IE //enable interrupts
	     ;
	     WDTCTL =(WDTPW + // (bits 15-8) password
	   	      // bit 7=0 => watchdog timer on
	   	      // bit 6=0 => NMI on rising edge (not used here)
	   	      // bit 5=0 => RST/NMI pin does a reset (not used here)
	   	      WDTTMSEL + // (bit 4) select interval timer mode
	   	      WDTCNTCL +
	   		  1// (bit 3) clear watchdog timer counter
	   	      // bit 2=0 => SMCLK is the source
	   	      // bits 1-0 = 00 => source/32K
	   	     );

	     IE1 |= WDTIE; // enable WDT interrupt so that conversions can start


	     init_spi();

	     P1DIR &= INPUT;
	     P1DIR |= (RED);
	    // P1OUT ^= (RED+GREEN);

	     //_bis_SR_register(GIE); //enable interrupts so we can print.
	      _bis_SR_register(GIE+LPM0_bits); //powerdown CPU
}

void interrupt spi_rx_handler()
{
	IFG2 &= ~UCB0RXIFG;
}
ISR_VECTOR(spi_rx_handler, ".int07");

void init_spi(){
        UCB0CTL1 = UCSSEL_2+UCSWRST;            // Reset state machine; SMCLK source;
        UCB0CTL0 = UCCKPH                                       // Data capture on rising edge
                                                                                // read data while clock high
                                                                                // lsb first, 8 bit mode,
                           +UCMST                                       // MASTER ONLY, DONT INCLUDE THIS IN THE SLAVE
                           +UCMODE_0                            // 3-pin SPI mode
                           +UCSYNC;                                     // sync mode (needed for SPI or I2C)
        UCB0BR0=BRLO;                                           // set divisor for bit rate
        UCB0BR1=BRHI;
        UCB0CTL1 &= ~UCSWRST;                           // enable UCB0 (must do this before setting
                                                                                //              interrupt enable and flags)
        IFG2 &= ~UCB0RXIFG;                                     // clear UCB0 RX flag
        IE2 |= UCB0RXIE;                                        // enable UCB0 RX interrupt
        // Connect I/O pins to UCB0 SPI
        P1SEL =SPI_CLK+SPI_SOMI+SPI_SIMO;
        P1SEL2 =SPI_CLK+SPI_SOMI+SPI_SIMO;
}


// ===== Watchdog Timer Interrupt Handler =====
interrupt void WDT_interval_handler()
{
	ADC10CTL0 |= ADC10SC; // trigger a conversion


	if(--action_counter ==0 )
	{
	               UCB0TXBUF= photoDiodeValue; // change this to output from ADC code
	                ++data_to_send; // increment byte to send for next time
	                ++tx_count;
	                action_counter=ACTION_INTERVAL;
	}


}
ISR_VECTOR(WDT_interval_handler, ".int10")


void interrupt adc_handler(){
	photoDiodeValue = ADC10MEM; // store the answer
	//UCB0TXBUF = photoDiodeValue >> 2;
	 if (photoDiodeValue < 10 )
	 {
		// P1OUT ^=  BIT3;   //  Set P1.0    HIGH
		 P1OUT ^= (RED);

     }
     else
     {
    	// P1OUT  &=  (BIT3);   //  Set P1.0    HIGH
    	 P1OUT &= ~(RED);    	// Turn off the entire Port 1, thus turning off the LED as well.
     }
}
ISR_VECTOR(adc_handler, ".int05")
