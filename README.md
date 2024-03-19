The Automatic Guitar Tuner is a smart, motorized device that will aid musicians and music enthusiasts alike in tuning their guitars accurately and efficiently. The primary motivation behind this project was to expand our knowledge and experience with audio signal processing. A brief description of our system can be as follows: string vibrations are picked up by the auxiliary input connecting to the guitar, and the resulting analog signal is amplified by an opamp and filtered using an Autocorrelation algorithm. This is to eliminate low and high frequencies that lie outside the range of a guitar. The microprocessor will then execute a frequency detection procedure to extract the fundamental frequencies of the input signal, and a frequency differential  signal is used to inform the motor protocols to rotate the tuning peg.
The microcontroller was the main component that did the bulk of the work in our device - the ESP32 microcontroller contained a 240 MHz clock, 32-bit registers, and a 12 bit ADC which were ideal specifications for our device for frequency detection. Next, the voltage regulator LM7805 allowed us to regulate from 9V to 5V up to 1.5A peak current, allowing us to easily  run the 600mA max draw of our device.  The servo motor selected has 30 Kg of torque and is able to rotate continuously for more than 360 degrees, with fine increments of movement. The large torque value allowed us to undercut the peak limit, running at 500mA and peak current of 1.5A. The display is an OLED two color display with 4 wire I2C  input, and is powered off the ESP32 at 3.3V. The PCB printed for the project is designed in EasyEda, and is only 48mm x 66mm, allowing us room for the MCU, regulator, and Op-Amp. 
The software used was written in C and flashed onto the ESP32. The main algorithm that produced the device functionality was the autocorrelation algorithm, allowing us to extract the dominant frequency via the AUX cable from a noisy harmonic-filled signal. This incrementally samples the input signal and compares each signal with the sample previous to it. The motor control was assigned based on whether the string had to be tuned up, or tuned down as a result of the frequency delivered by the autocorrelation algorithm and compared to the string selected. The only interrupt we had to implement was the string selection button, which pauses the CPU’s current task and begins to shift to the desired selected string.
