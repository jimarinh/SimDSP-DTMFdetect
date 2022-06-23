# SimDSP-DTMFdetect
## Test a DTMF detector on SimDSP for Android

This program implements a basic DTMF (Dual-tone multi-frequency) detector on SimDSP for Android. This program listens to the microphone and displays on the console the recognized character. 

To run this program, you need to install SimDSP for Android and the compiler toolchain as described in [SimDSP Project WebSite](https://github.com/parrado/SimDSP-Android). 

Once SimDSP is installed, download or clone this repository, and choose the option *Terminal > Run Build Task* on *VisualStudio Code*.  

To generate a DTMF sound for testing the application on Android, you can run the Matlab code (*gen_dtmf.m*) located in the *matlab* folder.

This DTMF detector is intended to show the SimDSP capabilites for signal processing and using the console mode. Console is updated approximately every second. It has been implemented using the Goertzel's algorithm. It does not include any signal detector to detect the beginning or ending of the tone. If you change the sampling frequency, please run the Matlab file "calc_dtmf.m" to compute the coefficients used by the Goertzel's algorithm.
