# Troubleshooting

Disconnect power before inspecting or changing wiring. Keep the wheels raised while diagnosing motor behavior. Test the ESP32, sensor, and motor independently before testing the complete system.

| Problem | Likely causes | Safe diagnostic steps |
|---|---|---|
| ESP32 does not appear in Arduino IDE | Charge-only USB cable, missing USB-to-serial driver, bad port, or board not powered | Try a known data USB cable and another USB port. Check whether the board power LED lights and install the USB-to-serial driver specified for the board. Select the newly appearing port. |
| Sketch compilation errors mention duplicate `setup()` or `loop()` | A second sketch/tab or pasted code defines another entry point | Keep one `setup()` and one `loop()` definition in the sketch. Check other open tabs and remove accidental duplicate code. |
| Motor does not spin | No motor supply, ENA jumper still installed, incorrect IN wiring, loose motor leads, low battery, or motor damage | Disconnect power and verify OUT1/OUT2, GPIO 25/26/27, and common ground. Remove the ENA jumper for GPIO speed control. Check battery/module ratings and polarity. Test the motor separately with a suitable current-limited supply; never drive it from an ESP32 GPIO. |
| L298N power LED remains off | Battery absent/flat, reversed polarity, loose connection, or module fault | Disconnect power, confirm polarity and the module's motor-supply input, then verify the battery with a meter. Do not infer the 5V terminal's role from the LED; check the exact module documentation. |
| Sensor returns no echo | TRIG/ECHO reversed or disconnected, sensor lacks 5 V, missing common ground, target outside range, or ECHO divider wired incorrectly | Check GPIO 32 to TRIG, ECHO through the divider to GPIO 33, sensor supply, and common ground. Test with a large flat target a short distance away. Verify the divider with power off. |
| Distance readings are implausible | ECHO connected directly to GPIO, poor ground, loose wires, noisy motor supply, angled/soft target, or sensor cross-talk | Confirm the divider uses 1 kΩ from ECHO to the junction and 2 kΩ from junction to GND. Check grounds and wiring. Test the sensor with the motor disconnected and a flat target. |
| Motor runs continuously | ENA jumper bypasses GPIO control, motor wiring does not match the intended channel, or firmware is not the expected sketch | Power down and remove the ENA jumper when GPIO 25 controls ENA. Confirm the uploaded sketch and the IN1/IN2 connections. Keep the wheels raised while retesting. |
| Motor stops unexpectedly | Reading timed out, reading is below 2 cm or above 100 cm, poor sensor wiring, or obstacle is beyond configured range | Read the Serial Monitor at 115200 baud. Inspect target distance, ECHO divider, and common ground. The firmware intentionally stops for invalid readings; it does not treat them as clear space. |
| ESP32 resets when motor starts | Motor startup current causes supply sag or electrical noise; grounds or wiring are poor | Power down. Keep ESP32 on USB during development and power the motor through a suitable separate supply and driver, with common ground. Check connections and motor current requirements. Do not connect conflicting power outputs. |
| Motor is weak with a single 3.7 V battery | Cell voltage/current capability and L298N voltage drop leave too little voltage for the motor | Check the motor and driver specifications and measure the supply safely under load. A single cell may be insufficient. Use a compatible supply/driver combination; do not exceed ratings or connect the battery to ESP32 3.3 V. |

## Isolate the subsystems

1. Upload the sketch with motor power disconnected; verify the ESP32 uploads and the Serial Monitor opens.
2. Test the HC-SR04 separately with the motor disconnected. Always keep the ECHO divider in place and confirm sensible readings.
3. Test the motor and L298N separately using an appropriate motor supply and raised wheels. Check ENA jumper configuration and common ground.
4. Connect all subsystems only after each works independently, then run the full test with wheels raised and the battery supervised.
