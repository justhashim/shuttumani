# Wiring Guide

Disconnect USB and the battery before making or changing connections. Keep the motor's wheels raised during first tests. Use the labels and documentation for your exact ESP32 and L298N modules; board layouts and regulator circuits differ.

> [!WARNING]
> The image at [`images/wire-diagram.jpeg`](../images/wire-diagram.jpeg) is an illustrative reference, **not the wiring plan for this firmware**. It depicts two motors, additional GPIO pins, and a 7.4 V two-cell battery. This project controls one motor with GPIO 25, 26, and 27 and documents a single 3.7 V cell. Follow the pin connections and power guidance in this document instead of copying the image.

## Pin-to-pin wiring

| From | To |
|---|---|
| ESP32 GPIO 25 | L298N ENA (remove ENA jumper) |
| ESP32 GPIO 26 | L298N IN1 |
| ESP32 GPIO 27 | L298N IN2 |
| L298N OUT1 and OUT2 | The two DC motor terminals |
| ESP32 GPIO 32 | HC-SR04 TRIG |
| HC-SR04 ECHO | ESP32 GPIO 33 through the divider below |
| USB-powered ESP32 5V/VIN (if available) | HC-SR04 VCC |
| HC-SR04 GND | Common ground |
| ESP32 GND | Common ground |
| L298N GND | Common ground |
| Battery negative | Common ground |
| Battery positive | L298N motor supply input, only if suitable for the exact module and motor |

The motor may run in the opposite direction from the labels in the code depending on its wiring. With power disconnected, swap the two motor leads at OUT1/OUT2 if needed. Do not change direction wiring while powered.

## ENA jumper

GPIO 25 controls motor enable and speed in the firmware. Remove the L298N ENA jumper before connecting GPIO 25. If the jumper remains installed, ENA may be held enabled and the ESP32 may not be able to control the motor speed as intended. Keep IN1 and IN2 connected to GPIO 26 and GPIO 27.

## HC-SR04 ECHO voltage divider

The sensor's ECHO output can be about 5 V, which is unsafe for an ESP32 input. Place a resistor divider between ECHO and GPIO 33:

```text
HC-SR04 ECHO ── 1 kΩ resistor ──┬── ESP32 GPIO 33
                                |
                              2 kΩ resistor
                                |
                               GND
```

The 1 kΩ resistor is the upper resistor between ECHO and the GPIO junction; the 2 kΩ resistor runs from that junction to GND. This produces approximately 3.3 V from a 5 V ECHO signal. Verify the resistor arrangement and connections before powering the circuit. A suitable logic-level shifter is another option.

## Common ground

The ESP32, sensor, L298N, and motor supply need a shared ground reference for GPIO signals to be interpreted correctly. Join ESP32 GND, sensor GND, L298N GND, and battery negative. Common ground does not mean joining positive supply rails. Never connect the L298N 5V terminal to ESP32 3.3 V.

## Motor power

The battery positive lead goes to the L298N motor supply input only when the cell voltage, current capability, polarity, and module input range are appropriate. Battery negative joins common ground. The correct L298N power wiring depends on the exact module revision: regulator and 5V-enable jumper arrangements vary. Check the board markings and documentation; do not assume the 5V terminal is an output or connect it to another supply without verifying the circuit.

A single 3.7 V Li-ion cell may be inadequate. A cell is about 3.7 V nominal, while the L298N uses bipolar output stages and loses a significant part of the supply voltage. The voltage left for the motor can be too low to start or run reliably, especially under load. Do not compensate by wiring around the driver or exceeding the motor/module ratings. Choose a compatible motor supply and driver based on their specifications.

Use a protected cell and a suitable charger. Never short-circuit, overcharge, or leave a cell unattended while charging. Disconnect power before wiring changes.

## ESP32 power

During development, power the ESP32 from USB. Do not connect the 3.7 V battery directly to the ESP32 3.3 V pin. Do not connect USB and an external supply in a way that makes their outputs conflict. HC-SR04 VCC may use the ESP32 board's 5V/VIN pin only when that pin provides USB-derived 5 V on your specific board. Confirm the board documentation first. Keep sensor and driver grounds common even when their positive supplies are separate.
