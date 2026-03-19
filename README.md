🚀 Boost DC-DC Converter Control using Passivity-Based Control (PBC)

📌 Overview
This project presents the design and implementation of a Boost DC-DC Converter control system using the Passivity-Based Control (PBC) method.
The system integrates:
Embedded control (STM32 ARM Cortex-M4)
MATLAB/Simulink simulation
Hardware circuit design
PC interface for monitoring (C#)

🎯 Objectives
Design a Boost DC-DC converter (15V → 20V)
Apply Passivity-Based Control (PBC) for voltage regulation
Implement real-time control on STM32F407
Build a GUI for monitoring system parameters
Validate system performance through testing

🧠 Technologies & Tools
Microcontroller: STM32F407 (ARM Cortex-M4)
Programming: C (Embedded), C# (GUI)
Simulation: MATLAB / Simulink
Communication: UART (CP2102)
Circuit Design: Proteus / PCB
Version Control: Git

🏗️ System Architecture
MATLAB/Simulink → Control Algorithm (PBC)
                     ↓
               STM32F407
                     ↓
           Boost DC-DC Converter
                     ↓
                 Output Voltage
                     ↓
              UART Communication
                     ↓
               C# Monitoring GUI
               
📂 Project Structure
DOAN
│
├── bao_cao/              # Reports, documents
├── chuong_trinh/         # Embedded code + MATLAB
├── giao_dien/            # C# GUI application
├── so_do_nguyen_ly/      # Circuit design (Proteus)
└── README.md

⚙️ Key Features
Real-time voltage control using PBC algorithm
Stable output voltage regulation
UART communication between STM32 and PC
GUI for real-time monitoring and visualization
Simulation + Hardware implementation

🧪 Testing & Results
Verified system stability under varying load conditions
Tested PWM output and control response
Debugged UART communication and signal accuracy
Evaluated performance between simulation and real system

📸 Demo (Optional - Add your images here)
Circuit design screenshot
Simulink model
GUI interface
Hardware testing setup

🔗 Project Link
👉 https://github.com/dengkhoa/DOAN

👨‍💻 Author
Nguyen Dang Khoa
IoT & Applied AI Student
Industrial University of Ho Chi Minh City

📈 Future Improvements
Implement Adaptive Control or AI-based control
Add data logging and advanced visualization
Improve efficiency and response time
Develop automated testing system

⭐ Notes
This project was developed as a graduation thesis focusing on embedded control systems and power electronics.
