# 🌸 Smart Pollen & Air Quality Monitoring System

[![ESP32](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B%20%2F%20Arduino-00599C.svg)]()
[![IoT Platform](https://img.shields.io/badge/IoT-Blynk-24B47E.svg)](https://blynk.io/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

An IoT-enabled environmental telemetry station built on the **ESP32** microcontroller. Fuses atmospheric conditions (temperature & relative humidity) with air pollution metrics (MQ-135 gas sensor) to predict high pollen concentration risks and hazardous ambient air conditions in real time. 

Telemetry is visualized locally via an **I2C 16x2 LCD** and streamed wirelessly to the **Blynk IoT Cloud** for mobile dashboard tracking and automated push notification alerts.

---

## 📌 Features

- **Real-Time Atmospheric Telemetry:** Continuous measurement of ambient temperature and relative humidity.
- **Air Quality & Gas Detection:** MQ-135 sensor evaluates hazardous gases (CO₂, NH₃, smoke, and volatile organic compounds).
- **Dual Output Interface:**
  - **Local Display:** 16x2 I2C LCD alternating between thermal metrics and air quality status (*"Fresh Air"* vs. *"Bad Air"*).
  - **Cloud Telemetry:** Wireless synchronization with Blynk IoT mobile app every 30 seconds.
- **Automated Hazard Alerting:** Dispatches instant `pollution_alert` notifications to mobile devices when pollution levels cross safety thresholds (> 400).
- **Non-Blocking Architecture:** Driven by `BlynkTimer` event scheduler to prevent loop locking and ensure stable Wi-Fi connectivity.

---

## 🏗️ System Architecture

```mermaid
graph TD
    A[MQ-135 Gas Sensor] -->|Analog Signal - GPIO 32| ESP[ESP32 Microcontroller]
    B[DHT11 Sensor] -->|Digital Signal - GPIO 2| ESP
    
    ESP -->|I2C SDA:21, SCL:22| LCD[16x2 I2C LCD Display]
    ESP -->|Wi-Fi 2.4 GHz| Cloud[Blynk IoT Cloud]
    
    Cloud --> App[Blynk Mobile App Dashboard]
    Cloud --> Alert[Push Notification Event]
