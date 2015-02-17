# ksparakis

##HW 1 

####What was your overall strategy?
  My overall strategy was to figure out first of all how to get everything to work. It was my first time working with the MSPBR430
  which caused a lot of overhead on set up. After setting everything up it took me a while to understand the example on blinking an led, but once I got that
  down I just used a global variable for unit which is 1 unit of time delay and I used for loops for the delay. I worked on getting the S down and once I had the code for that it was copy paste and edit the delay variables to get the SOS to work properly.
  
####What global variables did you use to keep track of the state of the system between WDT interrupts?
  The only global variables needed where P1DIR which would turn the light on or off depending on the xor of the regester value.I didnt realy need to keep track
of the rest as I just wrote my code in straight line type fashion. I also used a gloabl unit variable to have an easy way to decipher the time delays.

####What hardware did you use and how was it connected (for this HW the answer is probably to just way which on-board LED you used)
  I used the LED1 on the board to blink on and off to display the sos message.
