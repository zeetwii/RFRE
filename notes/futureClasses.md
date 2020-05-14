# Notes and suggestions for next time

## Software

### GNURadio

With Python 2.7 development finally finished as of Jan 1 2020, it is recommended to switch the class over to GNURadio 3.8 or later.  This will allow the students to develop on the latest version of Ubuntu without fear of breaking their system by updating.  

Thankfully installing GNURadio 3.8 is much easier on the latest version of ubuntu.  Rather than building from source, the ubuntu package has finally been updated, so a simple `apt install gnuradio-dev` will work.  

This does lead to a slight problem though, several of the projects are not yet compatible with 3.8, so it may be worthwhile developing an updated version internally.  The problem versions are: 

  * [gr-aistx](https://github.com/trendmicro/ais), which is the base for the AIS transmission part of project 4
  * [gr-paint](https://github.com/drmpeg/gr-paint), in the spectrum painter used for generating the images in P2
  * [gr-tempest](https://github.com/git-artes/gr-tempest), while not used in a project, all the tempest tool kits are written for GNURadio 3.7

### Receivers 

[SigDigger](https://github.com/BatchDrake/SigDigger) is a new analyzer that acts as a mix between GQRX, URH, and inspectrum.  It's a nice app to allow for both live streaming and detailed analysis of the signal.  

### Projects

It's recommended to replace project 3 with something else.  While the presentations were fun, the project overall felt like a bit of a wash.  

Some possible replacements: 

  * [IMSI Catcher](https://www.rtl-sdr.com/youtube-tutorial-building-a-passive-imsi-catcher-with-an-rtl-sdr/), this is a fun one that doesn't require any fancy antennas or complex setups.  It can easily be tied into a foxhunt like game where they must hunt down a specific IMSI address from a hidden phone.  
  * [Inmarsat-C](https://bitbucket.org/scytalec/scytalec/src/develop/), while a nicer antenna helps a lot its not 100% needed. The only downside is that its hard to know what you will hear over the radio.  Sometimes it will be super interesting, other times it will not.  
  * Hunting for an Jammer, while we did a lot of electronic warfare, we never really touched counter-ew activities.  This could be done by setting up a jammer and using an RFI tool like [CygnusRFI](https://github.com/0xCoto/CygnusRFI) to locate the jammer signal and location.