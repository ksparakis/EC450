#HW 5

### Schematic

--------

### Design

The Design is prety straight forward given the tools we are given. I implemented the timer inorder produce the pitches. Using a frequency chart and some simple math i was able to get the right pitch for each letter.

Since the timer produces time acording to half period. we know 

Period = 1/ Frequency and therefore  Half Period = 2/Frequency

but this gives us a solution in seconds so we have to multiple it by 1,000,000 in order to get a proper result for each frequency to be played.

hence

Half period = (1,000,000 * (2/Frequency in HZ))
