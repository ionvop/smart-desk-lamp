# smart-desk-lamp
An early-2025 Arduino school project.

## Components
- `camera/camera.ino` - ESP32-CAM server for camera input.
- `servo/servo.ino` - ESP-WROOM-32 server for servo control.
- `main.py` - Python script for image processing and object detection.

## Configuration
1. The OV2640 camera module should be connected to the ESP32-CAM.
2. The ESP32-CAM should be mounted on the ESP32-CAM-MB.
3. The horizontal and vertical servo motors should be connected to the ESP-WROOM-32 at pin 16 and 17 respectively.
4. The servo motors should be provided with its own 5V power supply.

## Setup
1. Clone the repository.
2. Run `pip install -r requirements.txt`.
3. Rename `config-template.json` to `config.json` and `config-template.h` to `config.h`.
4. Update the `config.h` with your WiFi credentials.
5. Compile and upload the respective sketches to the ESP32-CAM and ESP-WROOM-32.
6. Retrieve the IP address of the ESP32-CAM and ESP-WROOM-32 and update the `config.json`.
7. Run `python main.py`.