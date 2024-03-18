/*
 * Copyright 2023 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RPIGPIO2_LIB_LCD1602_H__
#define __RPIGPIO2_LIB_LCD1602_H__

#include "pcf8574/pcf8574.h"

namespace rpigpio2
{

class LCD1602
{
public:
  LCD1602() = default;
  virtual ~LCD1602() = default;

public:
  bool init(PCF8574 *pcf8574);
  void release(void);

public:
  bool initialized(void) { return _initalized; }

public:
  void clear();
  void home();
  void display(bool enable); // display on/off
  void cursor(bool enable);  // cursor on/off
  void blink(bool enable);   // blinking on/off
  void puts(const char *str);
  void putch(uint8_t ch);
  void move(uint8_t row, uint8_t col);

private:
  void function_set(uint8_t data);
  void cursor_set(uint8_t data);
  void display_set(void);
  void entrymode_set(uint8_t data);
  void putc(const char c);

private:
  void init_4bit(void);
  void send_4bits(uint8_t lcddata, uint32_t delay);
  void send_data(uint8_t data);
  void send_ctrl(uint8_t data);

private:
  void io_send_byte(uint8_t one);

private:
  PCF8574 *_pcf8574 = nullptr;
  bool _initalized = false;

  bool _back_light = false;
  bool _display = false;
  bool _cursor = false;
  bool _blink = false;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_LCD1602_H__
