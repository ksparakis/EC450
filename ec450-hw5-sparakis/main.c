#include  <msp430g2553.h>

// mask to select P1.1 as speaker pin


#define SPEAKER 0x02
#define rest 100000
#define space 100000
#define RED 0x01
#define BUTTON1 BIT4
#define BUTTON2 BIT5
#define BUTTON3 BIT7
#define BUTTON4 BIT9
#define BUTTON5 0x08
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

#define GREEN 0x40

//Note Timeing
#define Note1 137   // This is the equivalent of 124bmp , derived from the fact that wdt runs at 7.3ms
#define Note2 (Note1/2)
#define Note3 (Note2 + Note4)//dotted halfnote
#define Note4 (Note1/4)
#define Note5 (Note4+ (Note4/2))//Dotted quarterNote
#define Note8 (Note1/8)
#define Note16 (Note1/ 16)
#define NoteSpace (Note1/64)



int musicSheet[57]  = {Cnote ,Bnote, Anote, Gnote1, Fnote, Enote, Dnote, Cnote, Gnote1,
		Anote, Anote, Bnote, Bnote, Cnote, Cnote, Cnote, Bnote, Anote, Gnote1, Gnote1, Fnote, Enote, Cnote,
		Cnote, Bnote, Anote, Gnote1, Gnote1, Fnote, Enote, Enote, Enote, Enote, Enote, Enote, Fnote, Gnote, Fnote, Enote, Dnote, Dnote, Dnote, Dnote, Enote,
		Fnote, Enote, Dnote, Enote, Cnote, Anote, Gnote1, Fnote, Enote, Fnote, Enote, Dnote, Cnote};

int timeSheet[57] = { Note2, Note5, Note8, Note3, Note4, Note2, Note2, Note3, Note4,
		Note3, Note4, Note3, Note4, Note3, Note4, Note4, Note4, Note4, Note4, Note5, Note8, Note4, Note4,
		Note4, Note4, Note4, Note4, Note3, Note8, Note4, Note4, Note4, Note4, Note4, Note8, Note8, Note3, Note8, Note8, Note4, Note4, Note4, Note8, Note8,
		Note3, Note8, Note8, Note4, Note2, Note4, Note5, Note8, Note4, Note4, Note2, Note2, Note1 };


int musicSheet2[42] = { Gnote1, Gnote1, Dnote, Dnote, Enote, Enote, Dnote, Cnote, Cnote, Bnote, Bnote, Anote, Anote, Gnote1,
		Dnote, Dnote, Cnote, Cnote, Bnote, Bnote, Anote, Dnote, Dnote, Cnote, Cnote, Bnote, Bnote, Anote,
		Gnote1, Gnote1, Dnote, Dnote, Enote, Enote, Dnote, Cnote, Cnote, Bnote, Bnote, Anote, Anote, Gnote};

int timeSheet2[42] =  { Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4, Note4, Note4, Note4, Note2,
		Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4, Note4, Note4, Note4, Note2,
		Note4, Note4, Note4, Note4, Note4, Note4, Note2, Note4, Note4, Note4, Note4, Note4, Note4, Note2};

int period;
int playback =0;
int song =0;
int count =0;
int tempo =1;
int noteCount =0;
int spaceC = 0;
int spaceCount = NoteSpace;
volatile unsigned char last_button1;
volatile unsigned char last_button2;
volatile unsigned char last_button3;
volatile unsigned char last_button4;
volatile unsigned char last_button5;



void init_timer(void);

int main(void)
{


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
    period = musicSheet[0];


    // initialize the I/O port.
   // in the CPU status register, set bits:
   //    GIE == CPU general interrupt enable
   //    LPM0_bits == bit to set for CPUOFF (which is low power mode 0)

    //INTIALIZE BUTTONS

	init_timer();  // initialize timer
	TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)

	 P1DIR |= GREEN;
	 P1OUT &= ~(GREEN+RED);	  //Turn off green light initialy
	 P1DIR|= 0x01;
	 P1DIR &= ~BUTTON5;						// make sure BUTTON bit is an input
     P1OUT |= BUTTON5;
     P1REN |= BUTTON5;

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
spaceC =0;
	if(playback == 0)
	{

		if(spaceC ==1)
		{
			noteCount = 1;
		}

		if( --noteCount == 0)
		{
			if(spaceC ==1)
			{
				TA0CCR0 = 0;
				if(--spaceCount == 0)
				{
					spaceC =0;
					spaceCount = NoteSpace;

				}
			}
			else
			{
				if(song == 0)
				{
					count++;
					if(count == 57)
					{
						count =0;
					}

					TA0CCR0 = musicSheet[count];
					noteCount = timeSheet[count]*tempo;
					spaceC =1;
				}
				else if(song == 1)
				{
					count++;
					if(count == 42)
					{
						count =0;
					}
					TA0CCR0 = musicSheet2[count];
					noteCount = timeSheet2[count]*tempo;
					spaceC =1;
				}
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

}



// DECLARE WDT_interval_handler as handler for interrupt 10
ISR_VECTOR(WDT_interval_handler, ".int10")

