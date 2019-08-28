# Radio Frequency Signal Reverse Engineering (RFRE) class repo

## Course Description

The goal of this class is to teach students the basics of RF exploitation in relation to cyber security.  This means understanding both how to analyze a RF data link for possible cyber security vulnerabilities as well as how to utilize tools like Software Defined Radios (SDRs) in a penetration testing environment.  

## Course Lectures

Course Lectures will be Tuesday/Thursday at 4:45PM Central Time.  To attend lectures remotely, please request the hangout link from the instructor.  

## Course Topics

*  Introduction to RF signal analysis
    * Common tools of the trade
      * [qspectrumanalyzer](https://github.com/xmikos/qspectrumanalyzer)
      * [URH](https://github.com/jopohl/urh)
      * [GnuRadio](https://www.gnuradio.org/)
      * [HackRF](https://greatscottgadgets.com/hackrf/)
    * Basics of signal analysis
      * How to easy recognize common signals in the wild
      * How to use FCC ID and other government services
* RF vulnerability analysis
    * History of Electronic Warfare
    * What makes a RF data link vulnerable to attack
    * How to attack data links without getting caught
    * How to build custom attack scripts
* Walkthrough of common RF data links that are vulnerable to attacks
  * Car key fobs
  * Wireless M-Bus
  * IoT Alarm systems
  * Pager systems
  * GPS
  * AIS
  * Hotel Room Keys
  * And more

## Course Projects

There are six projects in the course that must be completed for a grade.  Each project is designed to only require a HackRF for hardware.  A class VM will be provided with software needed to complete each assignment.  Projects can be completed in groups, and are due two weeks after assigned.  

| Project | Description | Due Date |
|:---:|:---:|:---:|
| Electronic Warfare | Learn and practice attacking an RC tank with common EW techniques | TBD |
| Pirate Radio | Practice writing GnuRadio scripts by taking over a radio station hosted by the instructor | TBD |
| Signals in the wild | Pick an everyday RF device you interact with and do research on the signal as well as design a proof of concept attack | TBD |
| Cruise control | Design an attack on AIS to target a ships auto navigation system | TBD |
| Space Force | Take over a CubeSat simulator and practice targeting satellite based systems | TBD |
| Apocalypse Now | Using all of the data links and vulnerabilities discussed in class, design a presentation showing all the vulnerabilities you identified in a city assigned to you by your instructor.  | TBD |
  
  ## Course Books

  While not needed, a good intro to RF exploitation is [Inside Radio an Attack and Defense guide](https://www.amazon.com/Inside-Radio-Attack-Defense-Guide/dp/9811084467).  Many of the projects being used or discussed in the class can be found here, making it a good refresher or reference material.  