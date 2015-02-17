# ksparakis

##HW 1 

###What was your overall strategy?
  My overall strategy was to figure out first of all how to get everything to work. It was my first time working with the MSPBR430
  which caused a lot of overhead on set up. After setting everything up it took me a while to understand the example on blinking an led, but once I got that
  down I just used a global variable for unit which is 1 unit of time delay and I used for loops for the delay.
  
###What global variables did you use to keep track of the state of the system between WDT interrupts?
  The only global variables needed where P1DIR which would turn the light on or off depending on the xor of the regester value.I didnt realy need to keep track
of the rest as I just wrote my code in straight line type fashion.

###What hardware did you use and how was it connected (for this HW the answer is probably to just way which on-board LED you used)
  I used the LED1 on the board to blink on and off to display the sos message.
