#include  <msp430g2553.h>

// mask to select P1.1 as speaker pin


#define rest 100000
#define space 100000

#define RED 0x01 //Red Led  1.0
#define GREEN 0x40 //Green LED 1.6

#define SPEAKER 0x02 //1.1
#define BUTTON1 0x04// 1.2
#define BUTTON2 0x10 // 1.4
#define BUTTON3 0x02 // 1.5
#define BUTTON4 0x80 // 1.7
#define BUTTON5 0x08 //1.3 aka on board button
//#define BUTTON5 BIT10

//Note Frequencies

#define silence 1000000

// Half period = 1000000*(2/frequcency)

/*
 #define Anote (1000000*(2/440))
 #define Bnote (1000000*(2/494))
 #define Cnote (1000000*(2/523))
 #define Dnote (1000000*(2/587))
 #define Enote (1000000*(2/659))
 #define Fnote (1000000*(2/698))
 #define Gnote (1000000*(2/784))
 */

#define Anote 9090
#define Bnote 8130
#define Cnote 7662
#define Dnote 6825
#define Enote 6079
#define Fnote 5730
#define Gnote 5102

#define Anote1 (1000000*(2/220))
#define Bnote1 (1000000*(2/246))
#define Cnote1 (1000000*(2/261))
#define Dnote1 (1000000*(2/293))
#define Enote1 (1000000*(2/329))
#define Fnote1 (1000000*(2/349))
#define Gnote1 (1000000*(2/392))

/*
 #define Anote1 (2/110)
 #define Bnote1 (2/122)
 #define Cnote1 (2/130)
 #define Dnote1 (2/146)
 #define Enote1 (2/164)
 #define Fnote1 (2/174)
 #define Gnote1 (2/196)

 #define Anote1 (2/55)
 #define Bnote1 (2/61)
 #define Cnote1 (2/65)
 #define Dnote1 (2/73)
 #define Enote1 (2/82)
 #define Fnote1 (2/87)
 #define Gnote1 (2/98)
 */



//Note Timeing
#define Note1 137   // This is the equivalent of 124bmp , derived from the fact that wdt runs at 7.3ms
#define Note2 (Note1/2)
#define Note3 (Note2 + Note4)//dotted halfnote
#define Note4 (Note1/4)
#define Note5 (Note4+ (Note4/2))//Dotted quarterNote
#define Note8 (Note1/8)
#define Note16 (Note1/ 16)
#define NoteSpace (Note1/64)

int musicSheet[57] = { Cnote, Bnote, Anote, Gnote1, Fnote, Enote, Dnote, Cnote,
		Gnote1,
		Anote, Anote, Bnote, Bnote, Cnote, Cnote, Cnote, Bnote, Anote, Gnote1,
		Gnote1, Fnote, Enote, Cnote,
		Cnote, Bnote, Anote, Gnote1, Gnote1, Fnote, Enote, Enote, Enote, Enote,
		Enote, Enote, Fnote, Gnote, Fnote, Enote, Dnote, Dnote, Dnote, Dnote,
		Enote,
		Fnote, Enote, Dnote, Enote, Cnote, Anote, Gnote1, Fnote, Enote, Fnote,
		Enote, Dnote, Cnote };

int timeSheet[57] = { Note2, Note5, Note8, Note3, Note4, Note2, Note2, Note3,
		Note4,
		Note3, Note4, Note3, Note4, Note3, Note4, Note4, Note4, Note4, Note4,
		Note5, Note8, Note4, Note4,
		Note4, Note4, Note4, Note4, Note3, Note8, Note4, Note4, Note4, Note4,
		Note4, Note8, Note8, Note3, Note8, Note8, Note4, Note4, Note4, Note8,
		Note8,
		Note3, Note8, Note8, Note4, Note2, Note4, Note5, Note8, Note4, Note4,
		Note2, Note2, Note1 };

int musicSheet2[42] = { Gnote1, Gnote1, Dnote, Dnote, Enote, Enote, Dnote,
		Cnote, Cnote, Bnote, Bnote, Anote, Anote, Gnote1,
		Dnote, Dnote, Cnote, Cnote, Bnote, Bnote, Anote, Dnote, Dnote, Cnote,
		Cnote, Bnote, Bnote, Anote,
		Gnote1, Gnote1, Dnote, Dnote, Enote, Enote, Dnote, Cnote, Cnote, Bnote,
		Bnote, Anote, Anote, Gnote };

int timeSheet2[42] = { Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4,
		Note4, Note4, Note4, Note4, Note4, Note2,
		Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4,
		Note4, Note4, Note4, Note2,
		Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4,
		Note4, Note4, Note4, Note2 };

volatile unsigned int period;
volatile unsigned int playback = 0;
volatile unsigned int song = 0;
volatile unsigned int count = 0;
volatile unsigned int tempo = 1;
volatile unsigned int noteCount = 0;
volatile unsigned char last_button1;
volatile unsigned char last_button2;
volatile unsigned char last_button3;
volatile unsigned char last_button4;
volatile unsigned char last_button5;
volatile int tempCount = 100;
void init_timer(void);

int main(void) {
	BCSCTL1 = CALBC1_1MHZ;    // 1Mhz calibration for clock
	DCOCTL = CALDCO_1MHZ;
	// setup the watchdog timer as an interval timer
	WDTCTL = (WDTPW +	// (bits 15-8) password
						// bit 7=0 => watchdog timer on
						// bit 6=0 => NMI on rising edge (not used here)
						// bit 5=0 => RST/NMI pin does a reset (not used here)
			WDTTMSEL +   // (bit 4) select interval timer mode
			WDTCNTCL + 	// (bit 3) clear watchdog timer counter
							// bit 2=0 => SMCLK is the source
			1           // bits 1-0 = 10 => source/512 .
	);
	IE1 |= WDTIE;// enable the WDT interrupt (in the system interrupt register IE1)

	noteCount = timeSheet[0];
	period = musicSheet[0];

	// initialize the I/O port.
	// in the CPU status register, set bits:
	//    GIE == CPU general interrupt enable
	//    LPM0_bits == bit to set for CPUOFF (which is low power mode 0)

	//INTIALIZE BUTTONS

	init_timer();  // initialize timer
	TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)

	P1DIR |= GREEN;
	P1OUT &= ~(GREEN + RED);	  //Turn off green light initialy
	P1DIR |= 0x01;
	/*
	P1DIR &= ~BUTTON5;						// make sure BUTTON bit is an input
	P1OUT |= BUTTON5;
	P1REN |= BUTTON5;
	*/
	P1DIR &= ~BUTTON1;   // Set button pin as an input pin
	P1OUT |= BUTTON1;    // Set pull up resistor on for button
	P1REN |= BUTTON1; // Enable pull up resistor for button to keep pin high until pressed
	P1IES |= BUTTON1; // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	P1IFG &= ~BUTTON1;   // Clear the interrupt flag for the button
	P1IE |= BUTTON1;     // Enable interrupts on port 1 for the button
	P1DIR &= ~BUTTON2;   // Set button pin as an input pin
	P1OUT |= BUTTON2;    // Set pull up resistor on for button
	P1REN |= BUTTON2; // Enable pull up resistor for button to keep pin high until pressed
	P1IES |= BUTTON2; // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	P1IFG &= ~BUTTON2;   // Clear the interrupt flag for the button
	P1IE |= BUTTON2;     // Enable interrupts on port 1 for the button
	P1DIR &= ~BUTTON3;   // Set button pin as an input pin
	P1OUT |= BUTTON3;    // Set pull up resistor on for button
	P1REN |= BUTTON3; // Enable pull up resistor for button to keep pin high until pressed
	P1IES |= BUTTON3; // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	P1IFG &= ~BUTTON3;   // Clear the interrupt flag for the button
	P1IE |= BUTTON3;     // Enable interrupts on port 1 for the button
	P1DIR &= ~BUTTON4;   // Set button pin as an input pin
	P1OUT |= BUTTON4;    // Set pull up resistor on for button
	P1REN |= BUTTON4; // Enable pull up resistor for button to keep pin high until pressed
	P1IES |= BUTTON4; // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	P1IFG &= ~BUTTON4;   // Clear the interrupt flag for the button
	P1IE |= BUTTON4;     // Enable interrupts on port 1 for the button
	P1DIR &= ~BUTTON5;   // Set button pin as an input pin
	P1OUT |= BUTTON5;    // Set pull up resistor on for button
	P1REN |= BUTTON5; // Enable pull up resistor for button to keep pin high until pressed
	P1IES |= BUTTON5; // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	P1IFG &= ~BUTTON5;   // Clear the interrupt flag for the button
	P1IE |= BUTTON5;     // Enable interrupts on port 1 for the button
	_bis_SR_register(GIE+LPM0_bits);// enable general interrupts and power down CPU
}

volatile unsigned int Mode;
volatile unsigned int Mode2;
volatile unsigned int Mode3;
volatile unsigned int Mode4;
volatile unsigned int Mode5;

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {

	unsigned char flags = P1IFG & P1IE;

	if (flags & BUTTON1)
	{
		P1IFG &= ~BUTTON1;
		if (Mode == 0) //Executes on button click
		{
			Mode = 1;
			P1IES &= ~BUTTON1; // Clear edge detect to rising edge
			//Button 1 stuff


			 if(playback == 0)
			 {
				 playback =1;
				 P1OUT ^= RED; // toggle both LED's

			 }
			 else if(playback == 1)
			 {
				 playback =0;
				 P1OUT &= ~(RED); // toggle both LED's
			 }
		}
		else //Executes on button press
		{
			Mode = 0;
			P1IES |= BUTTON1;
		}

	}

	if (flags & BUTTON2)
	{
		P1IFG &= ~BUTTON2;
		if (Mode2 == 0) //Executes on button click
		{
			Mode2 = 1;
			P1IES &= ~BUTTON2; // Clear edge detect to rising edge
			//Button 2 Stuff
			 tempo++;
		} else //Executes on button press
		{
			Mode2 = 0;
			P1IES |= BUTTON2;
		}
	}

	if (flags & BUTTON3)
	{
		P1IFG &= ~BUTTON3;
		if (Mode == 0) //Executes on button click
		{
			Mode3 = 1;
			P1IES &= ~BUTTON3; // Clear edge detect to rising edge

			//bUTTON 3 STUFF
			 tempo -= 1;
		} else //Executes on button press
		{
			Mode3 = 0;
			P1IES |= BUTTON3;
		}
	}

	if (flags & BUTTON4)
	{
		P1IFG &= ~BUTTON4;
		if (Mode4 == 0) //Executes on button click
		{
			Mode4 = 1;
			P1IES &= ~BUTTON4; // Clear edge detect to rising edge
			//BUTTON 4 STUFF
			 count =0;
			 noteCount = timeSheet[0]*tempo;
		} else //Executes on button press
		{
			Mode4 = 0;
			P1IES |= BUTTON4;
		}
	}

	if (flags & BUTTON5)
	{
		P1IFG &= ~BUTTON5;
		if (Mode == 0) //Executes on button click
		{
			Mode5 = 1;
			P1IES &= ~BUTTON5; // Clear edge detect to rising edge
			//Button 5 stuff
			if (song == 0) {
				song = 1;
				count =0;
				P1OUT |= (GREEN);
			} else if (song == 1) {
				song = 0;
				count =0;
				P1OUT &= ~(GREEN);
			}
		} else //Executes on button press
		{
			Mode5 = 0;
			P1IES |= BUTTON5;
		}
	}

}

void init_timer() {              // initialization and start of timer
TA0CTL |= TACLR;              // reset clock
TA0CTL = TASSEL_2 + ID_0 + MC_1;  // clock source = SMCLK
TA0CCTL0 = 0; // compare mode, output 0, no interrupt enabled
TA0CCR0 = period - 1; // in up mode TAR=0... TACCRO-1
P1SEL |= SPEAKER; // connect timer output to pin
P1DIR |= SPEAKER;
}


interrupt void WDT_interval_handler() {

if(tempo > 1)
{
	if(--tempCount == 0)
	{
		 P1OUT ^= RED;
		tempCount = 50;
	}
}
else if(tempo < 1)
{
	if(--tempCount == 0)
	{
		 P1OUT ^= RED;
		tempCount = 100;
	}
}
else if(tempo == 1 && playback == 1)
{
	 P1OUT &= ~RED;
}

if (playback == 0) {
	if (--noteCount == 0) {
		if (song == 0) {
			count++;
			if (count == 57) {
				count = 0;
			}

			TA0CCR0 = musicSheet[count];
			noteCount = timeSheet[count] * tempo;

		} else if (song == 1) {
			count++;
			if (count == 42) {
				count = 0;
			}
			TA0CCR0 = musicSheet2[count];
			noteCount = timeSheet2[count] * tempo;
		}
	}

} else {
	//Do nothing
}

//BUTTON STUFF

/*
 //BUTTON 1 PLAY OR PAUSE
 unsigned char a;
 a= (P1IN & BUTTON1);  // read the BUTTON bit
 if (last_button1 && (a==0)){ // has the button bit gone from high to low
 P1OUT ^= GREEN; // toggle both LED's

 if(playback ==0)
 {
 playback =1;
 }
 else if(playback =1)
 {
 playback =0;
 }
 }
 last_button1=a;    // remember button reading for next time.

 //BUTTON 2 Speed up
 unsigned char b;
 b= (P1IN & BUTTON2);  // read the BUTTON bit
 if (last_button2 && (b==0)){ // has the button bit gone from high to low
 P1OUT ^= GREEN;
 tempo++;
 }
 last_button2=b;    // remember button reading for next time.


 //BUTTON 3 slow down
 unsigned char c;
 c= (P1IN & BUTTON3);  // read the BUTTON bit
 if (last_button3 && (c==0)){ // has the button bit gone from high to low
 P1OUT ^= GREEN; // toggle both LED's
 tempo --;
 }
 last_button3=c;    // remember button reading for next time.


 //BUTTON 4 restart
 unsigned char d;
 d= (P1IN & BUTTON4);  // read the BUTTON bit
 if (last_button4 && (d==0)){ // has the button bit gone from high to low
 P1OUT ^= GREEN;
 count =0;
 noteCount = timeSheet[0]*tempo;
 }
 last_button4=d;    // remember button reading for next time.



 //BUTTON 5 select song
 unsigned char e;
 e= (P1IN & BUTTON5);  // read the BUTTON bit
 if (last_button5 && (e==0)){ // has the button bit gone from high to low
 P1OUT ^= (RED+GREEN);
 if(song ==0)
 {
 song =1;
 }
 else if(song ==1)
 {
 song =0;
 }
 }
 last_button5=e;    // remember button reading for next time.
 */

}

// DECLARE WDT_interval_handler as handler for interrupt 10
ISR_VECTOR(WDT_interval_handler, ".int10")

