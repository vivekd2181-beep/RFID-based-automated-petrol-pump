# RFID Based Automated Petrol Pump System

This project automates the fuel dispensing process using **RFID authentication** and **manual fuel amount selection**. 
Users must scan their RFID card for authorization and then select an amount (₹50 / ₹100 / ₹150 / ₹200) using push buttons. The system controls the pump based on the amount selected.

---

## 🎯 Objectives
- Automate fuel dispensing with RFID card authentication
- Allow manual fuel quantity selection
- Reduce manual errors & improve transaction transparency
- Demonstrate embedded systems automation

---

## 🛠 Hardware Components
- Arduino UNO
- RC522 RFID Reader
- RFID Card / Tag
- 16x2 LCD Display
- Relay Module
- DC Pump / Motor (fuel simulation)
- Buzzer
- Push Buttons (4x — ₹50/₹100/₹150/₹200 selection)
- Jumper wires & power supply

---

## ⚙ Working Principle
1. User selects a fuel amount using buttons
2. User taps RFID card for authorization
3. If authorized:
   - LCD displays amount selected
   - Relay activates the pump
   - Pump runs for a predefined time based on amount
4. Once time is completed, pump turns OFF and "Done" message is displayed

---

## ⛽ Fuel Time Mapping
| Amount | Pump Time (example) |
|--------|---------------------|
| ₹50 | 5 seconds |
| ₹100 | 10 seconds |
| ₹150 | 15 seconds |
| ₹200 | 20 seconds |

> Time can be adjusted based on pump speed calibration.

---

## 📌 Circuit Diagram
(Add your circuit image here)
