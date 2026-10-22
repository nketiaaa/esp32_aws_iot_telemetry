Lab 1: Basic Telemetry Data Collection with ESP32 and AWS IoT Core
Overview
This lab establishes the foundational connection between an ESP32 microcontroller and AWS IoT Core. It demonstrates how to securely connect to Wi-Fi, using X.509 certificates via mutual TLS and publish basic JSON telemetry data to the AWS cloud.

---

Hardware Requirements
Microcontroller: ESP32 Development Board
Connectivity: Wi-Fi Network (or Smartphone Hotspot)
Development Environment: Arduino IDE with ESP32 board support

---

Software & Libraries

Ensure the following libraries are installed in your Arduino IDE:
WiFi.h (ESP32 core library)
WiFiClientSecure.h (ESP32 core library for secure TLS connections)
PubSubClient` (MQTT client library by Nick O'Leary)

---

AWS IoT Core Configuration

1. IoT Thing: Created a registered Thing in AWS IoT Core representing the ESP32 device.
2. X.509 Certificates: Generated and downloaded the Device Certificate, Private Key, and Amazon Root CA 1.
3. IoT Policy
