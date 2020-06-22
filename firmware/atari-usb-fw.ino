
// Joystick pins:
//
// - Left: PD4 (pin 25)
// - Right: PD6 (pin 26, arduino 12)
// - Up: PC7 (pin 32, arduino 13)
// - Down: PB6 (pin 30, arduino 10)
// - Button: PC6 (pin 31, arduino 5)
//
// LED pins:
// - Left: PB5 (pin 29)
// - Right: PB4 (pin 28, arduino 9)
// - Up: PB0 (pin 8)
// - Down: PD7 (pin 27, arduino 6)
// - Button: PF4 (pin 39, arduino 21)

#include "Joystick.h"

typedef enum {
  JOY_UP = 0,
  JOY_DOWN,
  JOY_LEFT,
  JOY_RIGHT,
  JOY_BUTTON
} InputType;

InputType input_types[5] = {
  JOY_UP,
  JOY_DOWN,
  JOY_LEFT,
  JOY_RIGHT,
  JOY_BUTTON
};

bool joy_read(InputType type)
{
  switch (type) {
  case JOY_UP: return !(PINC & (1 << 7));
  case JOY_DOWN: return !(PINB & (1 << 6));
  case JOY_LEFT: return !(PIND & (1 << 6));
  case JOY_RIGHT: return !(PIND & (1 << 4));
  case JOY_BUTTON: return !(PINC & (1 << 6));
  default: return false;
  }
}

void led_set(InputType type, bool on)
{
  if (on) {
    switch (type) {
    case JOY_UP: PORTB &= ~(1 << 0); return;
    case JOY_DOWN: PORTD &= ~(1 << 7); return;
    case JOY_LEFT: PORTB &= ~(1 << 5); return;
    case JOY_RIGHT: PORTB &= ~(1 << 4); return;
    case JOY_BUTTON: PORTF &= ~(1 << 4); return;
    }
  } else {
    switch (type) {
    case JOY_UP: PORTB |= 1 << 0; return;
    case JOY_DOWN: PORTD |= 1 << 7; return;
    case JOY_LEFT: PORTB |= 1 << 5; return;
    case JOY_RIGHT: PORTB |= 1 << 4; return;
    case JOY_BUTTON: PORTF |= 1 << 4; return;
    }
  }
}

void setup() {
  // Port B: PB6 joystick, PB0+PB4+PB5 led
  DDRB = (1 << 0) | (1 << 4) | (1 << 5);
  PORTB = (1 << 6);

  // Port C: PC6+PC7 joystick
  DDRC = 0;
  PORTC = (1 << 6) | (1 << 7);
  
  // Port D: PD4+PD6 joystick, PD7 led
  DDRD = (1 << 7);
  PORTD = (1 << 4) | (1 << 6);

  // Port F: PF4 led
  DDRF = (1 << 4);
  PORTF = 0;

  Joystick.begin();
}

void loop() {
  bool left, right, up, down, button;
  uint8_t x;
  uint8_t y;

  left = joy_read(JOY_LEFT);
  right = joy_read(JOY_RIGHT);
  up = joy_read(JOY_UP);
  down = joy_read(JOY_DOWN);
  button = joy_read(JOY_BUTTON);

  led_set(JOY_UP, up);
  led_set(JOY_DOWN, down);
  led_set(JOY_LEFT, left);
  led_set(JOY_RIGHT, right);
  led_set(JOY_BUTTON, button);
  
  if (up && !down) {
    Joystick.setYAxis(-127);
  } else if (down && !up) {
    Joystick.setYAxis(127);
  } else {
    Joystick.setYAxis(0);
  }

  if (left && !right) {
    Joystick.setXAxis(-127);
  } else if (right && !left) {
    Joystick.setXAxis(127);
  } else {
    Joystick.setXAxis(0);
  }

  Joystick.setButton(0, button);
}

