# Arduino I2C Multi-Node Chat System

An I2C-based multi-device messaging framework for Arduino microcontrollers utilizing the standard `Wire` library. This project allows multiple Arduino boards connected to a shared I2C bus to send direct peer-to-peer messages or broadcast messages across all nodes via the Serial Monitor.

## Features

* **Multi-Node Architecture**: Operates over an I2C bus with dedicated Master and Slave node addresses.


* **Direct Addressing**: Send targeted messages to a specific node using `@<address> <message>` syntax.


* **Broadcast Capabilities**: Automatically transmit text to all connected nodes on the bus by entering plain text without an address tag.


* **Interactive CLI**: Simple text-based user interaction powered by the Arduino Serial Monitor running at 9600 baud.



## Hardware Connection Setup

* **SDA**: Connect SDA pins across all Arduino devices (Pin 20 on Arduino Mega).
* **SCL**: Connect SCL pins across all Arduino devices (Pin 21 on Arduino Mega).
* **GND**: Connect all board ground pins to establish a shared reference.
* **Pull-up Resistors**: Place $4.7\text{ k}\Omega$ pull-up resistors on both SDA and SCL lines connected to 5V.

## Project Structure

* `M1_Master.ino` — Code for Node 1 (Master device, I2C Address 1).


* `M2_Slave.ino` — Code for Node 2 (Slave device, I2C Address 2).


* `M3_Slave.ino` — Code for Node 3 (Slave device, I2C Address 3).



## Message Command Syntax

Set the Serial Monitor line ending to **Newline** at **9600 baud**.

* **Direct Message**: Enter `@2 Hello World` to send a message exclusively to Node 2.


* **Broadcast Message**: Enter `Hello everyone` to send the text to all other nodes on the network.
