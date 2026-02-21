# Secure In-Vehicle Communication on Infineon AURIX TC397

## 🚀 Overview

This project demonstrates secure and insecure automotive communication between two **Infineon AURIX TC397** boards.
It implements CAN, CAN FD, and Automotive Ethernet communication along with simulated cyber-attacks and a lightweight **SecOC (non-AUTOSAR)** security mechanism.

The goal is to study real-world ECU communication vulnerabilities and evaluate protection techniques in an embedded environment.

---

## 🎯 Key Features

### 🔹 Communication Protocols

* Classical CAN communication between two TC397 boards
* CAN FD high-speed communication
* Automotive Ethernet data exchange


### 🔹 Security Implementation

* Custom **SecOC-like authentication** (non-AUTOSAR)
* Message freshness concept
* Basic integrity protection
* Secure vs insecure communication comparison

### 🔹 Attack Simulation

The system demonstrates practical in-vehicle network attacks across two setups:

**Scenario 1 — External Attacker**

* STM32 ANCIT board injects malicious CAN traffic
* Two AURIX TC397 boards perform normal communication
* Attacks observed on the live CAN bus

**Scenario 2 — Compromised ECU**

* One AURIX TC397 acts as attacker ECU
* Second AURIX TC397 acts as victim ECU
* Demonstrates insider ECU threat model

Implemented attacks:

* **DoS Attack** – CAN bus flooding
* **Spoofing Attack** – Fake message injection
* **Fuzzing Attack** – Random payload testing

---

### 🔹 Hardware Used

* Infineon AURIX TC397 (2 boards)
* STM32 ANCIT board (attacker board)
* Vector CAN interface
* On-board LEDs for visualization

---

## 🏗️ Attack Scenarios

Two experimental setups were validated.

### Scenario 1 — External attacker node (STM32)

```text
STM32 ANCIT Attacker
        │
        │  Malicious CAN Frames
        ▼
     ┌─────────┐
     │ CAN Bus │◄──────── Legitimate Traffic ─────── AURIX ECU 1 (Normal)
     └────┬────┘
          │
          ├────────► AURIX ECU 2 (Receiver)
          │
          └────────► BUSMASTER / Vector (CAN Analysis)
```

---

### Scenario 2 — Compromised ECU attacker (AURIX)

```text
AURIX TC397 (Compromised ECU)
        │
        │  Spoofed / DoS / Fuzz Traffic
        ▼
     ┌─────────┐
     │ CAN Bus │
     └────┬────┘
          │
          ├────────► AURIX TC397 (Victim ECU)
          │
          └────────► BUSMASTER / Vector (CAN Analysis)
```

---

## 📂 Repository Structure

```text
aurix-secure-communication/
│
├── aurix_tc397/
│   ├─── can/
|   |      |──Basic Communication      
│   |      |          |──CAN_TX/
|   |      |          |──CAN_RX/
|   |      |
|   |      |──SecOC Implementation
|   |                 |───CAN_RX_SECOC/
|   |                 |───CAN_TX_SECOC/
|   |── can_fd/
|   |      |──Basic Communication      
│   |                 |──CAN_FD_Sender/
|   |                 |──CAN_FD_Receiver/
│   ├── ethernet/
│          |──Basic Communication      
│          |         |──Ethernet_TX/
|          |         |──Ethernet_RX/
|          |
|          |──SecOC Implementation
|                    |───Ethernet_Secured_Receiver/
|                    |───Ethernet_Secured_Sender/
│
├── Attacks Aurix/
│       ├── dos/
│       ├── spoofing/
│       └── fuzzing/
├
|
|── STM32/
│   ├── CAN (STM to Aurix)/
│   ├── CAN (STM to STM)/
│
|
|
├── Attacks STM/
│       ├── dos/
│       
└── README.md
```

---

## ⚙️ Development Environment

* Infineon AURIX Development Studio (ADS)
* DAS (Device Access Scanner)
* STM32CubeIDE
* Embedded C

---

## 🔍 CAN Bus Analysis Tools

* Vector CAN toolchain for bus interfacing
* BUSMASTER for raw CAN frame monitoring and analysis

---

## ▶️ How to Run

### AURIX Side

1. Import the project into **AURIX Development Studio**
2. Build the project
3. Flash to TC397 board
4. Connect CAN between boards
5. Observe LED on successful reception

### Attack Demonstration

1. Flash STM32 ANCIT attacker firmware
2. Connect attacker to CAN bus
3. Trigger attack scenarios
4. Observe ECU behavior

---

## 🧪 Demonstrated Scenarios

* Normal CAN communication
* CAN FD high-speed transfer
* Ethernet packet exchange
* DoS bus saturation
* Message spoofing
* Fuzz testing of ECU
* SecOC authentication success/failure

---

## 📌 Learning Outcomes

* Practical automotive bus implementation
* Real ECU attack surface understanding
* Embedded security fundamentals
* Secure communication design
* Multi-node CAN network debugging
* Automotive cybersecurity experimentation

---


## 👤 Author

**Abhijeet Naik**

---