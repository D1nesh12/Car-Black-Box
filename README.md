# Car-Black-Box
This project implements a car black box system using the PIC18F4580 microcontroller. The black box is designed to record vehicle data such as speed, acceleration, and detect accidents or collisions.

## Features
Real-time data logging of vehicle parameters
Accident detection and event recording
Data storage on external EEPROM for retrieval and analysis
LED indicators for system status and alerts
User interface for configuration and data retrieval


## Hardware Requirements
PIC18F4580 microcontroller
External EEPROM (24LC256)
External RTC
CLCD
Matrix Keypad
Sensors (accelerometer)
LED indicators
Power supply unit


## Software Requirements
MPLAB IDE for PIC programming
XC8 Compiler
Serial communication terminal (e.g. PuTTY, TinyBootloader)

## Communication Protocols
The Car Black Box project utilizes the following communication protocols for data transmission:

- Serial Communication (UART): The PIC18F4580 microcontroller communicates with external peripherals and the user interface using the UART (Universal Asynchronous Receiver-Transmitter) protocol. UART is used for serial data transmission between the microcontroller and peripherals, such as GPS modules and serial communication terminals.

- I2C (Inter-Integrated Circuit): The project interfaces with external EEPROM memory using the I2C protocol. I2C is a two-wire serial communication protocol used for communication between integrated circuits and peripherals. It facilitates the storage and retrieval of data from the EEPROM memory module.

- Real-Time Clock (RTC): The project incorporates Real-Time Clock functionality for accurate timekeeping. The PIC18F4580 communicates with the RTC module using I2C protocol to retrieve real-time timestamp data, which is used for event logging and timestamping.

## Installation and Usage
Clone the repository to your local machine.
Open the project in MPLAB IDE.
Compile the source code using the XC8 compiler.
Program the PIC18F4580 microcontroller using a suitable programmer.
Connect the required sensors and peripherals to the microcontroller.
Power up the system and configure settings if necessary.
Monitor real-time data using a serial communication terminal.
Retrieve stored data from the EEPROM for analysis.

## Contributing
Contributions to this project are welcome. Feel free to fork the repository, make improvements, and submit pull requests.

## Acknowledgements
Thanks to the open-source community for the valuable resources and libraries used in this project.
