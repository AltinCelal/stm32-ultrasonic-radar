# 🛰️ STM32 Ultrasonic Radar Project

This project is a mini radar system developed using an STM32 microcontroller, an ultrasonic distance sensor, and a servo motor. The system performs a 0–180° scan and transmits real-time angle and distance data to a PC via UART, where it is visualized using a Qt-based radar interface.

---

## 🔧 Hardware Components
- STM32 microcontroller
- SG90 servo motor
- HC-SR04 ultrasonic distance sensor
- USB-to-UART connection

---

## 💻 Software & Tools
- STM32CubeIDE (HAL libraries)
- Qt 6 (C++)
- QSerialPort for UART communication
- QPainter for custom radar visualization

---

## 📡 System Overview
1. The servo motor sweeps the ultrasonic sensor between 0° and 180°
2. Distance is measured using the HC-SR04 ultrasonic sensor
3. Angle and distance data are calculated on the STM32
4. Data is transmitted to the PC via UART
5. The Qt application displays the data in real time as a radar interface
6. Detected targets are logged with timestamps

---

## 🎯 Features
- Real-time radar visualization
- 0–180° ultrasonic scanning
- Distance filtering (0–30 cm)
- Red target detection
- Target fade-out effect
- UART-based serial communication
- Timestamped data logging

---

## 📁 Project Structure
STM32_Radar/
├── STM32/ # STM32 CubeIDE firmware
├── Qt/ # Qt radar visualization application
├── README.md
├── .gitignore

---

## 📄 Data Logging
Detected targets are logged to a text file with the following format:

HH:MM:SS.mmm | Angle: XX deg | Distance: YY cm

Example: 12:14:03.124 | Angle: 45 deg | Distance: 18 cm


---

## 🚀 How to Run

### STM32 Firmware
1. Open the `STM32` folder in STM32CubeIDE
2. Build and flash the firmware to the board
3. Connect the board to the PC via USB (UART)

### Qt Application
1. Open the `Qt` folder in Qt Creator
2. Set the correct COM port in the source code
3. Build and run the application

---



## 👤 Author
Celal ALTIN

---

## 📜 License
This project is released under the MIT License.

