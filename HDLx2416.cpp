#include "HDLx2416.h"

void HDLx2416::set_pins(
  uint8_t CE1_,
  uint8_t CE2_,
  uint8_t CLR_,
  uint8_t CUE,
  uint8_t CU_,
  uint8_t WR_,
  uint8_t ADR[],
  uint8_t BL_,
  uint8_t DATA[]) {
  m_CE1_ = CE1_;
  m_CE2_ = CE2_;
  m_CLR_ = CLR_;
  m_CUE = CUE;
  m_CU_ = CU_;
  m_WR_ = WR_;
  for (int i = 0; i < 2; i++) {
    m_ADR[i] = ADR[i];
  }

  m_BL_ = BL_;
  for (int i = 0; i < 7; i++) {
    m_DATA[i] = DATA[i];
  }

  pinMode(m_CE1_, OUTPUT);
  pinMode(m_CE2_, OUTPUT);
  pinMode(m_CLR_, OUTPUT);
  pinMode(m_CUE, OUTPUT);
  pinMode(m_CU_, OUTPUT);
  pinMode(m_WR_, OUTPUT);

  for (uint8_t adr : m_ADR) {
    pinMode(adr, OUTPUT);
  }

  pinMode(m_BL_, OUTPUT);
  for (uint8_t data : m_DATA) {
    pinMode(data, OUTPUT);
  }

  clear_display();
}


void HDLx2416::enter_no_change_mode() {
  digitalWrite(m_CLR_, HIGH);
  digitalWrite(m_WR_, HIGH);
}

void HDLx2416::clear_display() {
  digitalWrite(m_CLR_, LOW);
  // clear needs to be held for at least 40 microseconds
  // we dont have access to microsecond, so we delay for one ms (which is probalby not necessary)
  delay(1);
  enter_no_change_mode();
}

void HDLx2416::start_write() {
  digitalWrite(m_CLR_, HIGH);
  digitalWrite(m_CE1_, LOW);
  digitalWrite(m_CE2_, LOW);
  digitalWrite(m_WR_, LOW);
  digitalWrite(m_CU_, HIGH);
}

void HDLx2416::end_write() {
  // write needs to be held for at least 40 nanoseconds
  // we dont have access to nanoseconds, so we delay for one ms
  delay(1);
  enter_no_change_mode();
}

void HDLx2416::show() {
  digitalWrite(m_CUE, LOW);
  digitalWrite(m_BL_, HIGH);
  digitalWrite(m_CLR_, HIGH);
}

void HDLx2416::choose_segment(uint8_t segment) {

  // we dont want to overwrite any other segments with our data that is currently lying on the pins
  // so we need to set write to high (write being low means writing is enabled), while changing the segment
  digitalWrite(m_WR_, HIGH);
  delay(1);  //delay of 40 ns is required, so we wait for 1ms

  switch (segment) {
    case 0:
      digitalWrite(m_ADR[0], LOW);
      digitalWrite(m_ADR[1], LOW);
      break;
    case 1:
      digitalWrite(m_ADR[0], HIGH);
      digitalWrite(m_ADR[1], LOW);
      break;
    case 2:
      digitalWrite(m_ADR[0], LOW);
      digitalWrite(m_ADR[1], HIGH);
      break;
    case 3:
      digitalWrite(m_ADR[0], HIGH);
      digitalWrite(m_ADR[1], HIGH);
      break;
  }

  digitalWrite(m_WR_, LOW);
}

void HDLx2416::write(uint8_t segment, uint8_t data) {

  choose_segment(segment);

  for (int i = 0; i < 7; i++) {
    bool bit = (data >> i) & 1;
    // 'bit' will contain the value of the i-th bit (0 or 1)
    digitalWrite(m_DATA[i], bit);
  }
}

void HDLx2416::write(uint8_t segment, uint8_t row, uint8_t column) {

  choose_segment(segment);

  for (int i = 0; i < 4; i++) {
    bool bit = (column >> i) & 1;
    digitalWrite(m_DATA[i], bit);
  }

  for (int i = 0; i < 3; i++) {
    bool bit = (row >> i) & 1;
    digitalWrite(m_DATA[4 + i], bit);
  }
}


void HDLx2416::enter_function_mode() {
  digitalWrite(m_CLR_, HIGH);
  digitalWrite(m_CE1_, LOW);
  digitalWrite(m_CE2_, LOW);
  digitalWrite(m_WR_, LOW);
}

void HDLx2416::set_intensity(int percent) {

  enter_function_mode();
  digitalWrite(m_CU_, LOW);
  digitalWrite(m_ADR[0], LOW);
  digitalWrite(m_ADR[1], LOW);
  digitalWrite(m_WR_, LOW);
  digitalWrite(m_DATA[6], LOW);

  // only a percentage mapped within 3 bits is allowed
  // while 111 represents 3% and 000 represents 100%
  // the intensity control values are not linear,
  // because of that a if cascade is the solution

  uint8_t intensity_value = 0;

  if (percent > 60) {
    intensity_value = 0;
  } else if (percent > 40) {
    intensity_value = 1;
  } else if (percent > 27) {
    intensity_value = 2;
  } else if (percent > 17) {
    intensity_value = 3;
  } else if (percent > 10) {
    intensity_value = 4;
  } else if (percent > 7) {
    intensity_value = 5;
  } else if (percent > 3) {
    intensity_value = 6;
  } else {
    intensity_value = 7;
  }

  uint8_t d5_d3[] = { m_DATA[3], m_DATA[4], m_DATA[5] };

  for (int i = 0; i < 3; i++) {
    bool bit = (intensity_value >> i) & 1;
    digitalWrite(d5_d3[i], bit);
  }

  delay(1);
  enter_no_change_mode();
}

void HDLx2416::turn_display_off() {
  enter_function_mode();
  digitalWrite(m_DATA[6], LOW);
  digitalWrite(m_DATA[2], HIGH);
  enter_no_change_mode();
}

void HDLx2416::turn_display_on() {
  enter_function_mode();
  digitalWrite(m_DATA[6], LOW);
  digitalWrite(m_DATA[2], LOW);
  enter_no_change_mode();
}