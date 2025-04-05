# smart-desk-lamp
An early-2025 Arduino school project

## Components
- `camera/camera.ino` - ESP32-CAM camera server
- `servo/servo.ino` - ESP-WROOM-32 servo server
- `main.py` - Object detection and servo control

## Setup
1. `camera/camera.ino:6` - replace `<ssid>` with your WiFi SSID
2. `camera/camera.ino:7` - replace `<password>` with your WiFi password
3. `servo/servo.ino:4` - replace `<ssid>` with your WiFi SSID
4. `servo/servo.ino:5` - replace `<password>` with your WiFi password
5. Run both ESP32-CAM and ESP-WROOM-32 to start the servers and retrieve their IP addresses
6. `main.py:7` - replace `<camera_ip>` with your camera IP address
7. `main.py:24` - replace `<servo_url>` with your ESP32 IP address