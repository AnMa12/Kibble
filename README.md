📸 ESP32-CAM + Stepper Motor Control Web Server
This project sets up an ESP32-CAM (AI Thinker model) as a Wi-Fi streaming server, plus a stepper motor controller with a web-based button!

Kibble - Smart Food Dispenser for Shelters

Kibble is a smart, donation-powered food dispenser, like a Tamagotchi, but you get to take care of a real pet! The idea is to make it easier for people to support shelters. They can donate, feed the animals remotely, and watch them live through a camera. This way, shelters get more support, and people can feel connected to the pets, even from a distance. Kibble uses 3D printing and Arduino to build an affordable, easy-to-use food dispenser with a live video feed. I created Kibble to help animal shelters in Romania and together with my friends we want to implement Kibble in shelters.

The project is named Kibble after the shape of cat and dog food, which inspired the design. Just like the small, convenient pieces of pet food, Kibble is designed to be simple, functional, and easy to use.

You will:
Stream the live camera feed from the ESP32.
Control a stepper motor remotely by pressing a button on the webpage.

📋 Wiring
ESP32-CAM Pin	Stepper Driver (ULN2003)	Description
GPIO 4	IN1	Stepper input 1
GPIO 6	IN3	Stepper input 3
GPIO 5	IN2	Stepper input 2
GPIO 7	IN4	Stepper input 4
GND	GND	Ground (shared)
5V	VCC	5V (external recommended for motor)
Important: Use common GND between ESP32-CAM and motor driver!

Libraries Needed
Stepper.h (built-in to Arduino IDE)

🌐 How It Works
ESP32 connects to your Wi-Fi network.
ESP32 runs a web server that:
Shows the camera stream (/stream endpoint).
Has a "Feed Mojo" button.
When you click the button, it triggers the motor to rotate one full revolution at 10 RPM.

Documentation: 
https://forum.arduino.cc/t/28byj-48-4-phase-stepper-motor/1162926
https://easyelectronicsproject.com/esp32-projects/program-esp32cam-arduino/
