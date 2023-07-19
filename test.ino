#include "HDLx2416.h"

HDLx2416 led;
void setup() {

  uint8_t add[] = { A0, A1 };
  uint8_t data_pins[] = { 0, 1, 2, 3, 4, 5, 6};

  led.set_pins(A4, A3, A5, 12, 13, A2, add, 11, data_pins);
}


void loop() {
  // put your main code here, to run repeatedly:
  led.clear_display();

  led.start_write();

  led.write(3, 0x4, 0x8);
  led.write(2, 0x6, 0x5);
  led.write(1, 0x7, 0x9);
  led.write(0, 0x2, 0x1);


  led.end_write();
  led.show();

  delay(5000);
}
