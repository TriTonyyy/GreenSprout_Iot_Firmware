**Capstone IoT Firmware**

# Related Repositoty
- GreenSprout_IoT_Web: https://github.com/TriTonyyy/GreenSprout_IoT_Web
- GreenSprout_IoT_Mobile(Android): https://github.com/TriTonyyy/GreenSprout_IoT_Mobile

📌 **Overview**

  This repository houses the firmware developed for our Capstone IoT project.   
  The firmware is tailored for the ESP32 microcontroller, facilitating seamless integration with various sensors and modules to collect, process, and transmit environmental data.

🧰 **Features**

  Sensor Integration: Supports multiple sensors for comprehensive data collection.
  
  Data Processing: Implements efficient algorithms for real-time data analysis.
  
  Wireless Communication: Utilizes Wi-Fi and Bluetooth for data transmission.
  
  Modular Architecture: Structured codebase for easy maintenance and scalability.

📁 **Repository Structure**

  Capstone_Iot_Firmware/
  
                      ├── include/           # Header files
                      
                      ├── lib/               # External libraries
                      
                      ├── src/               # Main source code
                      
                      ├── test/              # Unit tests
                      
                      ├── .vscode/           # VSCode configurations
                      
                      ├── platformio.ini     # PlatformIO project configuration
                      
                      └── README.md          # Project documentation
  

🚀 **Getting Started**

**Prerequisites**

  PlatformIO installed in your development environment.
  
  ESP32 development board.

**Installation**

**Clone the Repository:**

          -In terminal run:

                      git clone https://github.com/HuynhTonyy/Capstone_Iot_Firmware.git
                      
                      cd Capstone_Iot_Firmware
**Build the Project:**

          -In terminal run:

                      platformio run
  
  **Upload to ESP32:**
  
          -In terminal run:
  
                      platformio run --target upload
  **Capstone IoT Firmware**
  
          -In terminal run:
  
                      platformio device monitor
🧪**Testing**

  To execute unit tests:
  
          -In terminal run:
                      platformio test
📄**License**

  This project is licensed under the MIT License. See the LICENSE file for details.

🙌 **Acknowledgments**

  We extend our gratitude to our mentors and peers who provided invaluable support throughout this project.
