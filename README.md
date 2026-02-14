# Sporebox - Arduino powered


Sporebox is a microcontroller-based environmental controller for small-scale hobby setups. It automatically manages humidity, fans, and grow lights using a DHT11 sensor and relays with an Arduino.

---

## Features

- **Humidity Control**
  - Humidifier turns ON below 80% relative humidity (RH)
  - Turns OFF above 85% RH
  - Hysteresis prevents relay chatter

- **Fan Control**
  - Two 120mm PC fans cycle automatically
  - 5 minutes ON / 55 minutes OFF
  - Provides airflow and helps prevent overheating

- **Light Control**
  - Blue/yellow grow light cycle: 9 hours ON / 15 hours OFF
  - Starts ON at boot
  - Prevents heat buildup while providing a plant-friendly light spectrum

- **Non-blocking Timing**
  - Uses `millis()` for all timers
  - Keeps the system responsive for future expansion

- **Button Placeholders**
  - Wired buttons ready for manual adjustments
  - Debounced and safe for future use

---

## Hardware

| Component                  | Pin | Notes                                  |
|----------------------------|-----|----------------------------------------|
| DHT11 Humidity Sensor      | 8   | Reads humidity every 2 seconds         |
| Humidifier Relay           | 2   | Active LOW                             |
| Grow Light Relay (Blue/Yel | 3   | Active LOW, prevents overheating       |
| Button UP                  | 4   | Input pull-up, placeholder             |
| Button DOWN                | 5   | Input pull-up, placeholder             |
| Fan 1 Relay (120mm PC)     | 6   | Active LOW                             |
| Fan 2 Relay (120mm PC)     | 7   | Active LOW                             |

---

## Software

- Written for Arduino IDE
- Compatible with Arduino Uno, Mega, and similar boards
- Non-blocking timing allows continuous operation
- Hysteresis ensures stable humidifier and light control

---

## Notes

- Relays are active LOW (LOW = ON, HIGH = OFF)
- Keep the DHT11 sensor away from condensation
- Blue/yellow lights provide plant-friendly spectrum while minimizing heat
- Ready for future ESP32 upgrades, cloud monitoring, and display integration



