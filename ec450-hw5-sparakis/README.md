#HW 5

### Schematic
![alt text](https://github.com/Giles-ECE-BU/ksparakis/blob/master/ec450-hw5-sparakis/HW5-Schematic.png "Design Schematic")

#### Button Corralation 


Button 1: Play or Pause
<br />
Button 2: Speed up
<br />
Button 3: Speed down
<br />
Button 4: Reset
<br />
Button 5: Change song


#### Led States

| Green LED     | Red LED       | State |
| ------------- |:-------------| -----|
| off           | off           | Song 0 is playing at default Settings |
| off           | on            | Song 0 has been paused |
| off           | blinking slow | Song 0 is playing at a slower tempo than default |
| off           | blinking fast | Song 0 is playing at a faster tempo than default |
| on            | off           | Song 1 is playing at default Settings |
| on            | on            | Song 1 has been paused |
| on            | blinking slow | Song 1 is playing at a slower tempo than default |
| on            | blinking fast | Song 1 is playing at a faster tempo than default |






--------
### Design

The Design is straight forward given the tools we are given. I implemented the timer inorder to produce the pitches. Using a frequency chart and some simple math I was able to get the right pitch for each letter.

Since the timer produces time acording to half period. We know: 

####### Period = 1/ Frequency and therefore  Half Period = 2/Frequency

but this gives us a solution in seconds so we have to multiple it by 1,000,000 in order to get a proper result for each frequency to be played.

hence

####### Half period = (1,000,000 * (2/Frequency in HZ))

I created 2 arrays for each sheet of music, one array for the notes, and the other array for the note length.
This way using a simple counter I could index my way through each note and note length. The watchdog timer would look for button presses and at the same time count the beat, when the note length was met it would change the note and start a new count down decrementing the value of the note length.

The beat is origanlly set to 124 Beats per minute. I did this with some simple math with the knowledge that WDT runs ever 7.4ms.
A multiplier is added to this, so that if the beat needs to be simply speed up or slowed down you adjust the multiplier.

Using a simple global varibales for state I am also able to pause the song , reset the counter, start the song, and switch to my second song which is "Twinkle Twinkle Little Star."
