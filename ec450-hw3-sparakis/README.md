Konstantino Sparakis, U47131572, Sparakis@bu.edu

##HW 3

####How did I implement the record?
   I had a global variable for the mode in which the code was in called mode. If mode was = 1 im in record mode if the mode = 0 the board is in playback mode. Within playback there are 2 modes, the global variable firstRun keeps track of this. When firstRun = 0 there is nothing to playback and the green light blinks continoulsy to show this. When firstRun = 1 the code runs through the message recorded and plays it back. Another global variabl I needed was glob wich is a counter , this counts how many times wdt has been executed and on button click I reset this in order to get the time length wich the button should be on or off. THe last global variable I used was countSinceLastButtonAction wich kept track of the time  bettween button actions and if this ever surpassed a certain time it changes the record mode to playbackmode.
   
   For recording the time I used the variable glob , when the button was clicked it would save the value of glob into an int array and reset. This way I got the exact WDT count for how long the button was clicked and then I could go back to it like a forloop using the WDT timmer running through it incrementing a variable count running through the array turining on and off the light.

#### Limitations
  At first I tried to implement an interupt for the button. I got the interrupt working and some functionality , but there would always be some issue. I assume this is because the watchdog interupt would interupt the code excecution of my button interupt and back and forth so this didnt work. I finaly moved onto using the example provided by Proffesor giles.
  A limitation within my code is that there can only be 20 button actions as this is the limit I hard coded into the int array size holding the time value for each button action.
