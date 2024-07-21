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

## Assembly

These are instructions to assemble the circuit.

NOTE: Connect the antenna to the SX 1278 Module. (do this for both modules )

## CONNECTIONS FOR CAPTURE AND TRANSMIT CIRCUIT ( using Arduino Nano ):

### RA-02-SX1278 Module

| Arduino Nano Pin | RA-02-SX1278 Pin |
| ---------------- | ---------------- |
| D10              | NSS              |
| D11              | MOSI             |
| D12              | MISO             |
| D13              | SCK              |
| GnD              | GnD              |
| 3v3              | 3v3              |

### NEO7M GPS

| Arduino Nano Pin | NEO7M GPS Pin |
| ---------------- | ------------- |
| D6               | RX            |
| D5               | TX            |
| GnD              | GnD           |
| 5V               | VCC           |

### MPU6050

| Arduino Nano Pin | MPU6050 Pin |
| ---------------- | ----------- |
| A4               | SDA         |
| A5               | SCL         |
| GnD              | GnD         |
| 3V3              | 3V3         |

### BMP280

| Arduino Nano Pin | BMP280 Pin |
| ---------------- | ---------- |
| A4               | SDA        |
| A5               | SCL        |
| GnD              | GnD        |
| 3V3              | 3V3        |

### 18650 Power Supply

| Arduino Nano Pin | 18650 Power Supply Pin |
| ---------------- | ---------------------- |
| GnD              | -ve                    |
| Vin              | +ve                    |

## CONNECTIONS FOR RECEIVE AND DISPLAY CIRCUIT ( using ESP32 30pin ):

### RA-02-SX1278 Module

| ESP32 Pin | RA-02-SX1278 Pin |
| --------- | ---------------- |
| D5        | NSS              |
| D23       | MOSI             |
| D19       | MISO             |
| D18       | SCK              |
| GnD       | GnD              |
| 3v3       | 3v3              |

### OLED

| ESP32 Pin | OLED Pin |
| --------- | -------- |
| D21       | SDA      |
| D22       | SCL      |
| GnD       | GnD      |
| 3V3       | 3V3      |

### 18650 Power Supply

| ESP32 Pin | 18650 Power Supply Pin |
| --------- | ---------------------- |
| GnD       | -ve                    |
| Vin       | +ve                    |
