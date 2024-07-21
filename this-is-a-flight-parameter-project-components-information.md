---
name: This is a Flight Parameter Project
slug: this-is-a-flight-parameter-project
reference_id:
overview:
thumbnail:
type: "Project"
mode: Self
status: Active
access: 1
tags: []
locale: en-IN
first_published: Thu Sep 22 2022 06:59:46 GMT+0530 (India Standard Time)
last_updated: Thu Sep 22 2022 06:59:46 GMT+0530 (India Standard Time)
author: STEMVentor
attribution:
license:
complexity: Simple
level:
minutes:
credits:
draft: false
canonical_url:
---

NOTE: The typical current is usage is <= half the max current required for all components.

## Components for Capture and Transmit Circuit

| Component Name | Purpose                                       | Communication type | Max current req. | Volatge req. |
| -------------- | --------------------------------------------- | ------------------ | ---------------- | ------------ |
| Arduino Nano   | This will be the TX microcontroller           | -                  | 500mA            | 3.7v-12v     |
| RA-02SX1278    | This will be the LoRa module we will be using | SPI                | 120mA            | 1.8v-3.7v    |
| BMP280         | This will be the altitude sensor we will use  | I2C                | 1.12mA           | 1.7v-3.6v    |
| MPU6050        | This will be the Gyro sensor we will use      | I2C                | 3.2mA            | 3v-5v        |
| NEO7M          | This will be the GPS sensor we will use       | UART               | 40mA             | 3.3v-5v      |
| RA-02 Antenna  | This will be the antenna we wil be using      | -                  | -                | -            |

## Components for Receive and Display Circuit

| Component Name | Purpose                                       | Communication type | Max current req. | Volatge req. |
| -------------- | --------------------------------------------- | ------------------ | ---------------- | ------------ |
| ESP32 Wroom    | This will be the RX microcontroller           | -                  | 500mA            | 3.3v-3.7v    |
| RA-02SX1278    | This will be the LoRa module we will be using | SPI                | 120mA            | 1.8v-3.7v    |
| OLED (128x64)  | This will be the display we will use          | I2C                | 20mA             | 3.3v-5v      |
| RA-02 Antenna  | This will be the antenna we wil be using      | -                  | -                | -            |
