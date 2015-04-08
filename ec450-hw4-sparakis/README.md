#HW 4 Changing Clock to 4MHZ

I used a usb powered oscilloscope to measure the output frequency from pin 1.1 in the code example provided to us. This along with some trial error, reading the documentation and understanding how the BCSTL1 register and DCO Register effect the clock frequency was what eventualy led me to an answer. <br /> <br />At the 1Mhz setting I got the following frequency. I used the defualt provieded variable for this for seting DCO and BCSTL1 registers.
<br />
<br />
<img src="https://raw.githubusercontent.com/Giles-ECE-BU/ksparakis/master/ec450-hw4-sparakis/1Mhz.jpg?token=AEOE9_65A6ColMIrsEuGr7kJjfbAP0cJks5VLfCxwA%3D%3D" alt="Drawing"width="500" height="500"/>
<br />
<br />
As you can see the frequency I get is in hz and almost at 1khz at around 990hz, So with this I able to start building a pattern. I tried this as well using the 8MHZ default variables for the DCO a
and BCSCTL1 variables. and I got the following result:
<br />
<br />
<img src="https://raw.githubusercontent.com/Giles-ECE-BU/ksparakis/master/ec450-hw4-sparakis/8Mhz.jpg?token=AEOE98kcTUOyRIfbBp2XVN36uRHuVxZBks5VLfF0wA%3D%3D" alt="Drawing" width="500" height="500"/>
<br />
<br />
At 7.7Khz it became obvious that there was a pattern here so I turned over to the register and using the pause and edit method I played around with the BCSTL1 last
few bits as these are the bits that have the drastic effect on the internal clock and came to the solution of using the hex 0x8A which brought me the closest I could get to 4Mhz.

This was my final result which I believe was probably a little faster than 4Mhz because I was reading 3.9khz and looking at the fact that at 8mhz it decreases to about 7.7mhz UI can assume being at 3.9khz means I was a tad faster than 4mhz and probably at lik 4.1mhz, but this was close enough.
<br />
<br />

<img src="https://raw.githubusercontent.com/Giles-ECE-BU/ksparakis/master/ec450-hw4-sparakis/4mhz.jpg?token=AEOE9wp4KcUWWGqRSlrEDsw7mHRwyQy6ks5VLfJLwA%3D%3D" alt="Drawing"width="500" height="500"/>
