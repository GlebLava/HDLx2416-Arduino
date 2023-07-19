#include "HDLx2416.h"

HDLx2416 led;
void setup() {

  Serial.begin(9000);

  uint8_t add[] = { A0, A1 };
  uint8_t data_pins[] = { 3, 4, 5, 6, 7, 8, 9 };

  led.set_pins(A4, A3, A5, 12, 13, A2, add, 11, data_pins);
}

int time_interval = 500;
void loop() {
  led.clear_display();

  led.start_write();

  led.write(3, 0x4, 0x8);
  led.write(2, 0x6, 0x5);
  led.write(1, 0x7, 0x9);
  led.write(0, 0x2, 0x1);


  led.end_write();
  led.show();
  delay(time_interval);

  led.set_intensity(60);
  delay(time_interval);
  led.set_intensity(40);
  delay(time_interval);
  led.set_intensity(27);
  delay(time_interval);
  led.set_intensity(17);
  delay(time_interval);
  led.set_intensity(10);
  delay(time_interval);
  led.set_intensity(7);
  delay(time_interval);
  led.set_intensity(3);

  delay(time_interval);
  led.turn_display_off();

  delay(time_interval);
  led.set_intensity(100);
  led.turn_display_on();

  delay(time_interval);
}
