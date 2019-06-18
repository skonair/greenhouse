# My Greenhouse

The complete software solution for an automated greenhouse.

## What is it?

We would like to build a small and cheap greenhouse that is as self-contained as possible.
It should provide bountiful crops with minimal manual effort.

In the end we would like to
- measure the environment (temperature, humidity, lighting, gases),
- control the greenhouse (irrigation, cooling system, height sensors)
- and have a lot of fun in the process.

## Overview

### The Greenhouse

Some laths, screws, a few plastic buckets, some metal grids a plastic sheet and a some spare time.
This should be the frame for our greenhouse.

Structure (from bottom to top):
- compost pile
- 1st floor for planting
- 2nd floor for planting
- water tower and maintenance systems

### Sensors and Actuators

The sensors and actuators are controlled via an Arduino. 

#### TMP36 - temperature sensor

Lets start easy and measure the temperatur. We are interested in the temperature in the compost pile (lowest level of the greenhouse) and in the first and second planting floor. 

#### humidity sensors

At least every 

### Process Control

We will transmit the data to a Raspberry Pi that hosts the data storage and provides the ui interface.


