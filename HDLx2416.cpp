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
  delay(1); //delay of 40 ns is required, so we wait for 1ms

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
