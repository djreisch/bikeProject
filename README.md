# Idea

My idea for the project was to have a simple button mounted on the outside of my CBR250R that would be able to open my garage door. After intial testing I figured I would work in frame mounted LEDs to the project as well.


# Implementation

The implementation for the project was simple. A single arduino would control two relays. One relay was connected to the button harness of the garage door remote, and the other connected to the Vin of the led lights on the bike.

The arduino was powered from a voltage regulator hooked into the 12v of the bike. While the arduino is rated for up to 12v, the bike's alternator will cause the battery to output up to 14v when running. For this reason the regulator was installed.

A RTC module (Real-Time-Clock) was also added into the installation to provide automatic switchoffs for the LEDs (or auto switch ons) at certain times.

# End Result

The end result was a small project box that mounted in the "trunk" of the bike. With a single press of the button (mounted indiscreetly on my bike frame) you could open the garage door. With two clicks of the button you would toggle the LEDs.

The RTC was setup to turn off the lights at 7AM and turn on the lights at 7PM although those values are subject to change as I continue to tinker.
