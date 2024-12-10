# Ultra-Wideband (UWB) Distance Measurement Project

This project implements a **Two-Way Ranging (TWR)** protocol for Ultra-Wideband communication using **DW1000 modules**. It consists of three components:
1. Arduino-based **TWR Client** and **Server** programs.
2. Python-based **data logging** script for capturing measurements.

---

## **Project Overview**

The system alternates between two roles:
- **Client**: Initiates the ranging process and calculates distance using time-of-flight (ToF) measurements.
- **Server**: Responds to client requests and provides timestamps for ToF calculations.

Additionally, a Python script captures and logs data from the Arduino for analysis.

---

## **Contents**

### 1. **Arduino Programs**

#### **TWR Client**
The client:
- Initiates the ranging process by sending a `START` message.
- Computes the distance based on the round-trip time.
- Outputs both raw and skew-corrected distances to the serial monitor.

[View Client Code](#twr-client-code)

#### **TWR Server**
The server:
- Listens for `START` messages from the client.
- Responds with timestamps (`t2` and `t3`) for distance calculation.
- Ensures smooth communication by handling acknowledgments and errors.

[View Server Code](#twr-server-code)

#### **Ping-Pong Example**
An alternate implementation of the DW1000 module demonstrates a simple Ping-Pong communication system:
- Devices alternate between sending and receiving messages.
- Useful for testing basic communication functionality.

[View Ping-Pong Code](#ping-pong-code)

---

### 2. **Python Data Logger**

A Python script logs data received from the Arduino to a CSV file for analysis. It:
- Reads distance measurements in real-time over a serial connection.
- Logs `ToF`, `distance`, `ToF_sk`, and `distance_sk` to a CSV file.
- Safely handles interruptions and closes the serial connection.

[View Python Logger Code](#python-logger-code)

---

## **Installation and Setup**

### **Hardware Requirements**
1. **DW1000 Modules**:
   - Configured for UWB communication.
2. **Microcontroller**:
   - Arduino (e.g., Arduino Uno or DWM1001).
3. **Connections**:
   - SPI interface (MOSI, MISO, SCK, CS).
   - IRQ and RESET pins.

### **Software Requirements**
1. **Arduino IDE**:
   - Install the required **DW1000 library**.
2. **Python 3.x**:
   - Install `pyserial` library:
     ```bash
     pip install pyserial
     ```

---

## **Usage**

### **1. Upload Arduino Programs**
1. Connect the Arduino to your computer.
2. Upload the **TWR Client** sketch to one Arduino.
3. Upload the **TWR Server** sketch to another Arduino.
4. Ensure proper wiring for SPI and communication pins.

### **2. Run Python Logger**
1. Connect the Client Arduino to your computer.
2. Update the COM port in the Python script.
3. Run the logger script:
   ```bash
   python log_distance_data.py
