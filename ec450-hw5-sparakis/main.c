#include  <msp430g2553.h>

// mask to select P1.1 as speaker pin


#define SPEAKER 0x02
#define rest 100000
#define space 100000
#define BUTTON1 0x08
#define BUTTON2 0x09
#define BUTTON3 0x0A
#define BUTTON4 0x0B
#define BUTTON5 0x0C


//Note Frequencies


#define Anote (1000000/(2*55))
#define Bnote (1000000/(2*62))
#define Cnote (1000000/(2*65))
#define Dnote (1000000/(2*73))
#define Enote (1000000/(2*82))
#define Fnote (1000000/(2*87))
#define Gnote (1000000/(2*98))
/*
int Anote2 =(1000000/(2*110))
int Bnote2 =(123
int Cnote2 =(131
int Dnote2 =(147
int Enote2 =(165
int Fnote2 =(175
int Gnote2 =(196
*/

/*
#define Anote 12
#define Bnote 100
#define Cnote 200
#define Dnote 400
#define Enote 600
#define Fnote 800
#define Gnote 1000
*/


//Note Timeing
#define Note1 1000
#define Note2 (Note1/2)
#define Note3 (Note2 + Note4)//dotted halfnote
#define Note4 (Note1/4)
#define Note5 (Note4+ (Note4/2))//Dotted quarterNote
#define Note8 (Note1/8)
#define Note16 (Note1/ 16)

int tempo =1;
int noteCount =0;

int musicSheet[45]  = {Cnote, Bnote, Anote, Gnote, rest, rest, rest, rest, Gnote,
		Anote, Anote, Bnote, Bnote, Cnote, Cnote, Cnote, Bnote, Anote, Gnote, rest, rest, Cnote,
		Cnote, Bnote, Anote, Gnote, rest, rest, Enote, Enote, Enote, Enote, Enote, Fnote, Gnote, rest, rest,
		rest, rest, Cnote, Anote, rest, rest, rest};

int timeSheet[45] = { Note2, Note5, Note8, Note3, Note2, Note1, Note2, Note4, Note4,
		Note3, Note4, Note3, Note4, Note3, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4,
		Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4, Note4, Note4, Note8, Note8, Note3, Note4, Note1,
		Note1, Note4, Note2, Note4, Note1, Note1, Note1};


int musicSheet2[44] = { rest, rest, rest, Fnote, Enote, Dnote, Cnote, rest,
		rest, rest, rest, rest, Gnote, Fnote, Enote, rest,
		rest, Gnote, Fnote, Enote, rest, rest, rest, rest, Fnote, Enote, Dnote, Dnote, Dnote, Dnote, Enote,
		Fnote, Enote, Dnote, Enote, rest, rest, Gnote, Fnote, Enote, Fnote, Enote, Dnote, Cnote};

int timeSheet2[44] =  { Note1, Note2, Note4, Note4, Note2, Note2, Note3, Note4,
		Note1, Note1, Note1, Note1, Note5, Note8, Note4, Note4,
		Note1, Note5, Note8, Note4, Note4, Note1, Note2, Note4, Note8, Note8, Note4, Note4, Note4, Note8, Note8,
		Note3, Note8, Note8, Note4, Note4, Note2, Note5, Note8, Note4, Note4, Note2, Note2, Note1};


int period = 500;
int playback =0;
int song =0;
int count =0;

volatile unsigned char last_button1;
volatile unsigned char last_button2;
volatile unsigned char last_button3;
volatile unsigned char last_button4;
volatile unsigned char last_button5;



void init_timer(void);

int main(void) {


 	BCSCTL1 = CALBC1_1MHZ;    // 1Mhz calibration for clock
	DCOCTL  = CALDCO_1MHZ;
	// setup the watchdog timer as an interval timer
	  WDTCTL =(WDTPW +	// (bits 15-8) password
	                        // bit 7=0 => watchdog timer on
	                        // bit 6=0 => NMI on rising edge (not used here)
	                        // bit 5=0 => RST/NMI pin does a reset (not used here)
	           WDTTMSEL +   // (bit 4) select interval timer mode
	           WDTCNTCL + 	// (bit 3) clear watchdog timer counter
	  		                // bit 2=0 => SMCLK is the source
	  		   1           // bits 1-0 = 10 => source/512 .
	  		   );
	  IE1 |= WDTIE;		// enable the WDT interrupt (in the system interrupt register IE1)

    noteCount = timeSheet[0];


    // initialize the I/O port.
   // in the CPU status register, set bits:
   //    GIE == CPU general interrupt enable
   //    LPM0_bits == bit to set for CPUOFF (which is low power mode 0)

    //INTIALIZE BUTTONS
    	  P1DIR &= ~BUTTON1;
    	  P1OUT |= BUTTON1;
     	  P1REN |= BUTTON1;

     	  P1DIR &= ~BUTTON2;
          P1OUT |= BUTTON2;
     	  P1REN |= BUTTON2;

    	  P1DIR &= ~BUTTON3;
    	  P1OUT |= BUTTON3;
     	  P1REN |= BUTTON3;

     	  P1DIR &= ~BUTTON4;
          P1OUT |= BUTTON4;
     	  P1REN |= BUTTON4;

    	  P1DIR &= ~BUTTON5;
    	  P1OUT |= BUTTON5;
     	  P1REN |= BUTTON5;


	init_timer();  // initialize timer
	TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)
    _bis_SR_register(GIE+LPM0_bits);// enable general interrupts and power down CPU
}


void init_timer(){              // initialization and start of timer
	TA0CTL |= TACLR;              // reset clock
		TA0CTL = TASSEL_2+ID_0+MC_1;  // clock source = SMCLK
		TA0CCTL0=0; // compare mode, output 0, no interrupt enabled
		TA0CCR0 = period-1; // in up mode TAR=0... TACCRO-1
		P1SEL|=SPEAKER; // connect timer output to pin
		P1DIR|=SPEAKER;
}

interrupt void WDT_interval_handler(){
	if(playback == 0)
	{
		if( --noteCount == 0)
		{
			if(song == 0)
			{
				count++;
				TA0CCR0 = musicSheet[count];
				noteCount = timeSheet[count]*tempo;
			}
			else if(song == 1)
			{
				count++;
				TA0CCR0 = musicSheet2[count];
				noteCount = timeSheet2[count]*tempo;
			}
		}
	}
	else
	{
		//Do nothing
	}


	//BUTTON STUFF


	//BUTTON 1 PLAY OR PAUSE
	unsigned char a;
	  	a= (P1IN & BUTTON1);  // read the BUTTON bit
	  	if (last_button1 && (a==0)){ // has the button bit gone from high to low
	  		//P1OUT ^= (RED+GREEN); // toggle both LED's
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
		  		//P1OUT ^= (RED+GREEN); // toggle both LED's
		  		tempo++;
		  	}
		  	last_button2=b;    // remember button reading for next time.

			//BUTTON 3 slow down
			unsigned char c;
			  	c= (P1IN & BUTTON3);  // read the BUTTON bit
			  	if (last_button3 && (c==0)){ // has the button bit gone from high to low
			  		//P1OUT ^= (RED+GREEN); // toggle both LED's
			  		tempo --;
			  	}
			  	last_button3=c;    // remember button reading for next time.


				//BUTTON 4 restart
				unsigned char d;
				  	d= (P1IN & BUTTON4);  // read the BUTTON bit
				  	if (last_button4 && (d==0)){ // has the button bit gone from high to low
				  		//P1OUT ^= (RED+GREEN); // toggle both LED's
				  		count =0;
				  		noteCount = timeSheet[0]*tempo;
				  	}
				  	last_button4=d;    // remember button reading for next time.



					//BUTTON 5 select song
					unsigned char e;
					  	e= (P1IN & BUTTON5);  // read the BUTTON bit
					  	if (last_button5 && (e==0)){ // has the button bit gone from high to low

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
}



// DECLARE WDT_interval_handler as handler for interrupt 10
ISR_VECTOR(WDT_interval_handler, ".int10")

