# Lab 5: Remote Resistance Measurement via AWS IoT Core

## Overview
This lab demonstrates how to convert analog sensor readings from an ESP32 microcontroller into precise resistance measurements and securely transmit the telemetry data over MQTT to **AWS IoT Core**. 
The system uses a voltage divider configuration with a known reference resistor to calculate an unknown resistance dynamically, converting raw ADC values into engineering units.

---

## Hardware Setup & Wiring

---

### Components Required
* ESP32 Development Board
* Known Reference Resistor (10kOhm)
* Unknown Resistor or Component to be measured
* Breadboard & Jumper Wires

---

### Schematic Connection (Voltage Divider)
3.3V Pin
│
│
┌────┴────┐
│  R1     │ (10kΩ Reference Resistor)
└────┬────┘
├──────────────> ESP32 Analog Pin (GPIO 34)
┌────┴────┐
│  Rx     │ (Unknown Resistor to Measure)
└────┬────┘
│
GND

---

## Software & Cloud Configuration

### Libraries
Ensure the following libraries are installed in your Arduino IDE:
* WiFi.h 
* WiFiClientSecure.h 
* PubSubClient

---

### AWS IoT Setup
1. Thing Creation: Created an AWS IoT Thing (ESP32_Resistance_Device).
2. Security & Certificates: Generated X.509 certificates (Device Certificate, Private Key, and Amazon Root CA 1) and attached them to the Thing.
3. IoT Policy:
