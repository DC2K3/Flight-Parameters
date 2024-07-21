# Flight-Parameters

## Capture and Transmit circuit
The transmitter PCB is mounted on a RC plane. Arduino nano is being used as the microcontroller for this circuit and these are the following sensors plugged into it:
- GPS (Neo7m)
- Accelerometer and Gyroscope (MPU6050)
- Altitude ,pressure and temperature sensor (BMP280)
  
**LoRa communication** is being used to transmit data to the receiver circuit on ground. SX1278 LoRa module is being used. It has a tested range of 1km.

## Receive and display circuit
The receiver PCB is on ground. ESP32 wroom is being used as the microcontroller. It has the receiver LoRa plugged into the PCB.
This circuit receives the data and sends it to processing software for visualisation on the device via bluetooth.

This repositry consists of the Final circuit codes , connections  , circuit diagrams and the PCB files.
