# Project 4 - Now you see me

## Background

With the success in South America, Rebel Holdings has gained several new contracts in the middle east that require your skills.  

Increased tensions in the region have led to the UN cracking down on smugglers and embargo violators in the region.  They have reached out with Rebel to find a way to break through the blockade.

Rebel intel has identified that the UN is using two common data links to search for smugglers, [AIS](https://en.wikipedia.org/wiki/Automatic_identification_system) for ships and [ADS-B](https://en.wikipedia.org/wiki/Automatic_dependent_surveillance_%E2%80%93_broadcast) for aircraft.  Both of these data links are vulnerable to spoofing which gives you two options: flood the system with too many targets to track, or spoof valid tracks so that no one can tell the difference.  

This means that you have four possible ways to complete your assignment: target either AIS or ADS-B, and then build a tool to either flood the region with targets or spoof a valid moving target.  

## Part 0 - Pick an action

Unlike P3, this time your actions will control the entire assignment and how you are graded.  With four possible outcomes, the path forward will be quite chaotic.  

* Choose AIS to focus on maritime travel and attempt to free the seas for perfectly illegal activities once again.
  * By building a AIS spammer, you will flood the UN systems with too many targets, causing them to crash and leaving UN patrol vessels blind.
    * Without the ability to use modern scanners, UN patrols will be limited to visual and verbal identification.  The fact that the worlds largest governing body was defeated by a bunch of smugglers and pirates will be humiliating, and cause many nations to reconsider future cooperation.
  * By building an AIS spoofer, you will cause UN patrols to doubt their own data and increase the likelihood of an accidental seizure or hostile action taken against an allied vessel.  
    * If UN forces were to accidentally attack an allied vessel, it would jeopardize the whole mission and possible force the UN to retreat in humiliation.
* Choose ADS-B to focus on air travel and return the skies to the glorious pre-1980 accident rates.
  * By building an ADS-B spammer, you will flood a given area with ADS-B tracks and cause Air Traffic Control (ATC) systems to crash, preventing the UN from monitoring the skies.
    * The UN won't be the only one blind though, due to your flooding attack, major airports in the region will not be able to operate, causing massive finical losses to the host countries and turning them against the UN.
  * Building an ADS-B spoofer will allow smuggling planes to pretend to be previously cleared aircraft.  This will cause UN officers to become paranoid and confused as to which airplane is which on their monitoring station.  
    * If the UN were to forcefully divert or worse attack a commercial aircraft, the resulting fallout would force them out of the region.

## Part 1 - The actual assignment

After picking which data link you want to target and how you will exploit it, you must design a program to carry out your attack.  Your program has several constraints:

* The backend of the program must use GNURadio to pass data to and control your SDR.
* The GNURadio portion of the program should use a GNURadio block that your team wrote on their own.
* The program can have multiple components and does not have to be written all in the same language.
* The program can be a combination of multiple subprograms all executing individually but talking to each other.

If making a flooding/spamming technique, your program must be able to:

* Generate a given number of targets without requiring to be recompiled.
* Generate those targets within a given lat lon range.
  * The team can choose to either evenly disperse the targets over the area or generate all of them in one location.

If making a spoofer technique, your program must be able to:

* Generate a target with a given lat, lon, id, and heading
* Have the target be able to travel between two points at a reasonable speed
