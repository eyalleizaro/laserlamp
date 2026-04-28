# Build Instructions

This guide explains how to build the Laser Lamp electronics and assemble the first working version of the project.

The project uses an Arduino Uno, two 8-bit shift registers, NMOSFET transistors, and multiple LED/laser modules. The shift registers allow the Arduino to control many outputs using only a few control pins.

> Important: Test the circuit with regular LEDs first. Connect the laser modules only after the electronics work correctly.

---

## 1. Parts Used

Main parts:

- 1 × Arduino Uno R3
- 2 × 8-bit shift registers, such as 74HC595
- 1 × 5V power supply
- 24 × LED or laser modules
- 24 × NMOS transistors / MOSFETs
- 24 × 100kΩ resistors
- 16 × 100Ω resistors
- Jumper wires / soldering wires
- Breadboard, perfboard, or custom PCB
- Lamp body / enclosure

---

## 2. Project Overview

The circuit has three main parts:

1. **Arduino Uno**  
   Controls the lamp patterns and sends data to the shift registers.

2. **Shift registers**  
   Expand the number of digital outputs. Each shift register gives 8 outputs. Two shift registers provide 16 controlled outputs.

3. **NMOSFET output channels**  
   Each LED/laser module is controlled by an NMOSFET.  
   The MOSFET works like an electronic switch: the Arduino or shift register sends a signal to the MOSFET gate, and the MOSFET turns the LED/laser channel on or off.

---

## 3. Safety Before Building

Before powering the circuit:

- Do not look directly into any laser.
- Do not point lasers at people, animals, vehicles, windows, or reflective surfaces.
- Use low-power modules only.
- Test the circuit with LEDs before connecting lasers.
- Disconnect power before changing wires.
- Check polarity before powering the circuit.
- Make sure the power supply can provide enough current.
- Use an enclosure or diffuser when possible.

---

## 4. Build Step 1: Prepare the Power Rails

Create two main power rails:

- `POWER_SUPPLY+` = positive 5V rail
- `POWER_SUPPLY-` = ground / negative rail

Connect:

- Power supply `+` to the positive rail
- Power supply `-` to the ground rail
- Arduino `GND` to the same ground rail

The Arduino and the external power supply must share a common ground.

Do not connect the LED/laser modules before checking the voltage with a multimeter.

---

## 5. Build Step 2: Connect the Arduino

Connect the Arduino Uno to the circuit.

The Arduino is used for:

- Sending data to the shift registers
- Sending clock signals to the shift registers
- Controlling several MOSFET channels directly

In the schematic, Arduino digital pins are labeled as:

- `U1_D2`
- `U1_D5`
- `U1_D6`
- `U1_D7`
- `U1_D8`
- `U1_D9`
- `U1_D10`
- `U1_D11`
- `U1_D13`

Pins `D10`, `D11`, and `D13` are used for communication with the shift registers.

A common setup is:

| Arduino Pin | Function |
|---|---|
| D11 | Data / SER |
| D13 | Shift clock / SRCLK |
| D10 | Latch clock / RCLK |

---

## 6. Build Step 3: Connect the First Shift Register

Connect the first 8-bit shift register.

For a 74HC595-style shift register:

| Shift Register Pin Name | Connect To |
|---|---|
| VCC / Power | 5V |
| GND / Ground | Ground |
| SER / Input | Arduino D11 |
| SRCLK / Shift register clock | Arduino D13 |
| RCLK / Output register clock / Latch | Arduino D10 |
| OE / Output Enable | Ground |
| SRCLR / Shift Register Clear | 5V |
| Q0-Q7 / Outputs | MOSFET gates |
| Q7S / Serial output | Input of second shift register |

In Tinkercad, the same pins may appear with names like:

| Tinkercad Name | Meaning |
|---|---|
| power | VCC |
| ground | GND |
| input | SER / Data |
| shift register clock | SRCLK |
| output register clock | RCLK |
| output enable | OE |
| shift register clear | SRCLR |
| output 1-8 | Q0-Q7 |
| inverted output 8 | Q7S / serial output |

---

## 7. Build Step 4: Connect the Second Shift Register

Connect the second shift register the same way as the first one.

The important difference:

- The data input of the second shift register should receive data from the serial output of the first shift register.
- This allows both chips to work together as one longer 16-output controller.

Connect:

| Second Shift Register Pin | Connect To |
|---|---|
| VCC / Power | 5V |
| GND / Ground | Ground |
| SER / Input | Q7S of first shift register |
| SRCLK | Arduino D13 |
| RCLK | Arduino D10 |
| OE | Ground |
| SRCLR | 5V |
| Q0-Q7 | MOSFET gates |

---

## 8. Build Step 5: Build One MOSFET Output Channel

Before building all 24 channels, build and test only one channel.

Each channel includes:

- 1 NMOSFET
- 1 LED or laser module
- 1 100kΩ resistor
- Optional/current-limiting resistor, depending on the LED/laser module

Basic connection:

| MOSFET Pin | Connect To |
|---|---|
| Gate | Arduino pin or shift-register output |
| Source | Ground |
| Drain | Negative side of LED/laser module |

The positive side of the LED/laser module connects to `POWER_SUPPLY+`.

The 100kΩ resistor is used as a pull-down resistor:

- One side connects to the MOSFET gate
- The other side connects to ground

This keeps the MOSFET turned off when there is no control signal.

---

## 9. Build Step 6: Test One Output

Upload a simple Arduino test program that turns one output on and off.

Check that:

- The LED turns on when the control pin is HIGH
- The LED turns off when the control pin is LOW
- The MOSFET does not get hot
- The resistor does not get hot
- The power supply voltage stays stable

Do this test with a normal LED first, not a laser module.

---

## 10. Build Step 7: Build the Remaining Channels

After one channel works, repeat the same MOSFET channel design for all outputs.

Each LED/laser output should have:

- One NMOSFET
- One 100kΩ pull-down resistor on the gate
- A connection to the positive power rail
- A switched connection through the MOSFET to ground

The schematic uses labels like:

- `T1_G`, `T2_G`, `T3_G`, etc. for MOSFET gates
- `D1`, `D2`, `D3`, etc. for LED/laser outputs
- `POWER_SUPPLY+` for positive voltage
- `POWER_SUPPLY-` for ground

Build the outputs gradually:

1. Build 1 channel
2. Test it
3. Build 4 channels
4. Test them
5. Build 8 channels
6. Test them
7. Continue until all channels are connected

---

## 11. Build Step 8: Check Current Usage

Before turning on many lights at once, check the current requirement of each LED/laser module.

Example:

If each module uses 20mA:

```text
24 modules × 20mA = 480mA
