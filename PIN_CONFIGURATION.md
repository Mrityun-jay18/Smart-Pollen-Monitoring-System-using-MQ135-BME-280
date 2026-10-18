# 🔌 Pin Configuration & Hardware Connection Guide

Detailed pinout reference and hardware connection guide for the **Smart Pollen & Air Quality Monitoring System** using the **ESP32 Dev Module (30-pin / 38-pin)**.

---

## 📌 Complete Pinout Summary Table

| Peripheral / Sensor | Sensor Pin | ESP32 GPIO | Channel / Bus | Logic Level | Power Source | Function |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **MQ-135 Gas Sensor** | `AOUT` (Analog Out) | `GPIO 32` | ADC1_CH4 | 0 – 3.3V | `VIN` (5V) | Reads ambient pollution & gas levels |
| | `VCC` | `VIN` (5V) | Power | 5V DC | External / USB | Powers onboard heating coil (Rh) |
| | `GND` | `GND` | Ground | 0V | Common Ground | Reference ground |
| | `DOUT` (Digital Out) | *Not Connected* | — | — | — | Digital threshold output (unused) |
| **DHT11 Sensor** | `DATA` / `OUT` | `GPIO 2` | Digital I/O | 3.3V | `3V3` | Reads relative humidity (%) and temp (°C) |
| | `VCC` | `3V3` | Power | 3.3V DC | ESP32 3V3 rail | Power rail |
| | `GND` | `GND` | Ground | 0V | Common Ground | Reference ground |
| **16x2 LCD (I2C)** | `SDA` | `GPIO 21` | I2C Data | 3.3V / 5V | `VIN` (5V) | Serial data line (PCF8574 adapter) |
| | `SCL` | `GPIO 22` | I2C Clock | 3.3V / 5V | `VIN` (5V) | Serial clock line (PCF8574 adapter) |
| | `VCC` | `VIN` (5V) | Power | 5V DC | External / USB | Powers LCD driver & LED backlight |
| | `GND` | `GND` | Ground | 0V | Common Ground | Reference ground |

---

## ⚠️ Critical Hardware & Electrical Notes

### 1. MQ-135 Voltage Considerations
- **Heating Element:** The MQ-135 requires **5V (`VIN`)** for its internal heating element to operate properly and provide accurate chemical readings.
- **Analog Read Pin (`GPIO 32`):** `GPIO 32` belongs to **ADC1** on the ESP32. **Do not use ADC2 pins** (such as GPIO 0, 2, 4, 12–15, 25–27) for analog reads when Wi-Fi is active, because the Wi-Fi driver uses ADC2 internally. `GPIO 32` is on ADC1 and is 100% safe to use alongside Wi-Fi!

### 2. DHT11 Pull-up Resistor
- If you are using a **3-pin DHT11 module**, it already has an onboard 10kΩ pull-up resistor.
- If you are using a **bare 4-pin DHT11 sensor**, place a **4.7kΩ to 10kΩ resistor** between `DATA` (`GPIO 2`) and `3.3V`.

### 3. I2C LCD Address
- The default I2C address in the firmware is `0x27`:
  ```cpp
  LiquidCrystal_I2C lcd(0x27, 16, 2);
