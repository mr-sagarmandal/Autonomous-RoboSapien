=====//Kunal Mehan & Sagar Mandal//=====
----
===**Project Description**===
This project turns the Robosapien v1 into a Bluetooth controllable robot with an
autonomous mode functionality. Users can use the Adafruit Bluefruit app to make
the robot move forward, backward, turn etc. The app can also be used to initiate
the autonomouse mode. To interface with Robosapien's internal microcontroller,
an ARM Mbed was utilized. Mbed's internal code utilized three infra-red sensors
to read the robot's surroundings and a bluetooth module to receive signals from
user.

\\
===**Parts Used**===
* [[https://developer.mbed.org/platforms/mbed-LPC1768/#schematics-and-data-sheets|
 ARM Mbed Microcontroller]]
* [[https://developer.mbed.org/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/ | Adafruit Bluefruit BLE board]]
* [[https://www.pololu.com/product/136 | Sharp GP2Y0A21YK0F Analog Distance Sensor]]
* [[http://www.robots.nu/robosapien-v1-en-us/ | Robosapien v1 Robot]]
* [[https://www.radioshack.com/products/radioshack-470uf-35v-20-radial-lead-electrolytic-capacitor | 470uF Capacitor]]

\\
===**Pin Assignments**===

=====Adafruit BLE Board=====
|=Mbed |=Adafruit BLE|
| VU (+5V) | Vin |
| gnd | gnd |
| gnd | CTS |
| p27 (Serial RX) | TXO |
| p28 (Serial TX) | RXI |

=====Sharp Analog Distance Sensor=====
|=Mbed |= Sensors|
| VU (+5V) | Vin |
| gnd | gnd |
| p17, p18, p19 | Analog Inputs |

=====Robosapien=====
|=Mbed |= Robosapien|
| gnd | gnd |
| p20 | Analog Input |

**Note:** Since the components use a large amount of current, we connect
a 470uF capacitor between Ground Bus and VU Bus.

The pin assignments are visualized in Figure 2. below.
\\\\\\
{{/media/uploads/sagarmandal/robosapiens_pin_assignments.png}}
\\
**Figure 2.** The wiring diagram and pin assignments of all the components.
\\\\
===**Underlying Algorithm**===
====**Basic Movement Functionality**====
The basic movement functionality works by using the Adafruit BLE Board. The program
accepts command from the smartphone connected to the BLE board. After receiving the
command, it executes them
===**Issy
