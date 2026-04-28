/*
  Laser Lamp Controller

  Hardware:
  - Arduino Uno
  - 2x 74HC595 / 8-bit shift registers
  - 24x NMOSFET output channels
  - 24x LED / laser modules

  Output logic:
  - HIGH = channel ON
  - LOW  = channel OFF

  Important:
  Test with regular LEDs before connecting laser modules.
*/

#include <Arduino.h>

// =========================
// Shift register pins
// =========================

const byte DATA_PIN  = 11;  // SER / input
const byte CLOCK_PIN = 13;  // SRCLK / shift register clock
const byte LATCH_PIN = 10;  // RCLK / output register clock

// Number of shift registers
const byte NUM_SHIFT_REGISTERS = 2;
const byte NUM_SHIFT_OUTPUTS = NUM_SHIFT_REGISTERS * 8; // 16 outputs

// =========================
// Direct Arduino output pins
// =========================
// These are used for the extra 8 MOSFET channels.
// If your real wiring is different, change this list.

const byte DIRECT_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const byte NUM_DIRECT_OUTPUTS = sizeof(DIRECT_PINS) / sizeof(DIRECT_PINS[0]);

// =========================
// Total channels
// =========================

const byte TOTAL_CHANNELS = NUM_SHIFT_OUTPUTS + NUM_DIRECT_OUTPUTS; // 24

// Channel states
bool channelState[TOTAL_CHANNELS];

// Animation speed
int delayTime = 120;


// =========================
// Basic output functions
// =========================

void writeShiftRegisters() {
  byte firstRegister = 0;   // channels 0-7
  byte secondRegister = 0;  // channels 8-15

  for (byte i = 0; i < 8; i++) {
    if (channelState[i]) {
      firstRegister |= (1 << i);
    }
  }

  for (byte i = 0; i < 8; i++) {
    if (channelState[i + 8]) {
      secondRegister |= (1 << i);
    }
  }

  digitalWrite(LATCH_PIN, LOW);

  /*
    Because the shift registers are chained:
    send the second register first, then the first register.
  */
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, secondRegister);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, firstRegister);

  digitalWrite(LATCH_PIN, HIGH);
}


void writeDirectPins() {
  for (byte i = 0; i < NUM_DIRECT_OUTPUTS; i++) {
    byte channelIndex = NUM_SHIFT_OUTPUTS + i;
    digitalWrite(DIRECT_PINS[i], channelState[channelIndex] ? HIGH : LOW);
  }
}


void updateOutputs() {
  writeShiftRegisters();
  writeDirectPins();
}


void setChannel(byte channel, bool state) {
  if (channel >= TOTAL_CHANNELS) {
    return;
  }

  channelState[channel] = state;
  updateOutputs();
}


void allOff() {
  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    channelState[i] = false;
  }

  updateOutputs();
}


void allOn() {
  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    channelState[i] = true;
  }

  updateOutputs();
}


// =========================
// Test patterns
// =========================

void testEachChannel() {
  allOff();

  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    allOff();
    setChannel(i, true);

    Serial.print("Testing channel: ");
    Serial.println(i);

    delay(500);
  }

  allOff();
}


void chaseForward() {
  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    allOff();
    setChannel(i, true);
    delay(delayTime);
  }
}


void chaseBackward() {
  for (int i = TOTAL_CHANNELS - 1; i >= 0; i--) {
    allOff();
    setChannel(i, true);
    delay(delayTime);
  }
}


void bouncePattern() {
  chaseForward();
  chaseBackward();
}


void fillUpPattern() {
  allOff();

  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    setChannel(i, true);
    delay(delayTime);
  }

  delay(300);
  allOff();
}


void alternatingPattern() {
  allOff();

  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    channelState[i] = (i % 2 == 0);
  }

  updateOutputs();
  delay(500);

  for (byte i = 0; i < TOTAL_CHANNELS; i++) {
    channelState[i] = (i % 2 != 0);
  }

  updateOutputs();
  delay(500);
}


void sparklePattern() {
  allOff();

  for (byte i = 0; i < 40; i++) {
    byte randomChannel = random(0, TOTAL_CHANNELS);

    setChannel(randomChannel, true);
    delay(60);
    setChannel(randomChannel, false);
  }

  allOff();
}


// =========================
// Setup
// =========================

void setup() {
  Serial.begin(9600);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (byte i = 0; i < NUM_DIRECT_OUTPUTS; i++) {
    pinMode(DIRECT_PINS[i], OUTPUT);
  }

  randomSeed(analogRead(A0));

  allOff();

  Serial.println("Laser Lamp Controller Started");
  Serial.print("Total channels: ");
  Serial.println(TOTAL_CHANNELS);

  // Safe startup test: one channel at a time
  testEachChannel();
}


// =========================
// Main loop
// =========================

void loop() {
  bouncePattern();
  fillUpPattern();
  alternatingPattern();
  sparklePattern();
}
