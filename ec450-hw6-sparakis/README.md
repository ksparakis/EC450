#EC450 HW6
###Team Members- Konstantino Sparakis & Zach Sarki

####Schematics
![schematic 1](https://raw.githubusercontent.com/Giles-ECE-BU/ksparakis/master/ec450-hw6-sparakis/schematic/Slide2.png?token=AEOE93v8ZOeNTFSf1AbGzeGaWsKVQP-wks5VMcxawA%3D%3D)
![schematic 2](https://raw.githubusercontent.com/Giles-ECE-BU/ksparakis/master/ec450-hw6-sparakis/schematic/Slide1.png?token=AEOE9zyszCjW8XkJvAgqjVHkYsxD6TZNks5VMcv6wA%3D%3D)

####Design

Master: The master reads in the photodiodes value using adc conversion and then using the spi communicates this value to the slave. When the Photo Diode is covered a red led on the Master lights up.

Slave: The slave recieves this value through the spi and using the timer and a speaker plays this value as a frequency creating a diffrent noise for when you cover the photo diode or shine a bright light on it.

There is a limitation of the speed of communication of the spi and also the adc value out is 10 bits, and the spi can only send 8 bits.
