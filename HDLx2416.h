#ifndef HDLX2416_H
#define HDLX2416_H

#include <Arduino.h>
#include <inttypes.h>

class HDLx2416 {

public:
  // See chip documentation Pin Numbering and Location

  // Left side of chip top to bottom
  uint8_t m_CE1_;
  uint8_t m_CE2_;
  uint8_t m_CLR_;
  uint8_t m_CUE;
  uint8_t m_CU_;
  uint8_t m_WR_;
  uint8_t m_ADR[2];

  // Right side of chip top to bottom
  uint8_t m_BL_;
  uint8_t m_DATA[7];


  void set_pins(
    uint8_t CE1_,
    uint8_t CE2_,
    uint8_t CLR_,
    uint8_t CUE,
    uint8_t CU_,
    uint8_t WR_,
    uint8_t ADR[],
    uint8_t BL_,
    uint8_t DATA[]);

  void enter_no_change_mode();
  void clear_display();
  void start_write();
  void end_write();
  void show();
  void write(uint8_t segment, uint8_t data);
  void write(uint8_t segment, uint8_t row, uint8_t column);

  void set_intensity(int percent);
  void turn_display_off();
  void turn_display_on();


private:
  void choose_segment(uint8_t segment);
  void enter_function_mode();
};


#endif