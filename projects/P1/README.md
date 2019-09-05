# Project 1 - From Russia with Tanks

## Background

The dreaded [Russian RC battalion](https://www.amazon.com/Liberty-Imports-Fighting-Battle-Tanks/dp/B00GA97CUG) has joined the fight in Ukraine.  With their impenetrable poly-plastic armor and AAA engines they have shifted the war back in Russia's favor.  

Your mission, regardless of if you choose to accept it, is to reverse the signals, jam the controller, and rebroadcast commands to the tank.  

## Part 0 - Pick a command

For this project you must pick one of two commands to reverse and work on.  Your choices:
  * Start/Stop - You can halt the invasion by figuring out how to turn the tanks off at will.
  * Steering - You can also halt the invasion by taking control of the tanks steering and forcing them to reverse all the way back to Russia.  

## Part 1 - Signal Analysis

For this part, you will reverse and decode the command that you picked in part zero.  This means listing the following information in your report: 

 * What is the encoding used? ASK/FSK/PSK/ect?
 * What is the carrier frequency of the controller?
 * What does the decoded message look like both visually and in hex?
 * How often does the signal repeat? 
 * Is there any odd behaviors in the transmitter?

 ## Part 2 - Controller Jamming

 For this part of the assignment, you will need to create a Jammer script in GnuRadio for your controller.  

 You will include your GRC file in your report, and demo your jammer to the instructor or one of the TAs.  Please include who you demoed to in your report.  

 ## Part 3 - Signal Replay

 Using your analysis of the signal, find a way in GnuRadio to rebroadcast your command to force the tank to only do what you want.  Note, your going to want to clean up your capture file so that theres no downtime for the enemy to regain control.  

 Like before, include the GRC file in your report, as well as demo to the instructor or TAs.  


### Report format

For this report, in addition to answering the above questions, please include the name of your teammates, the number of the HackRF that you used, and the number of the tank that you used.  