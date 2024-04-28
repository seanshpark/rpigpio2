/*
 * Copyright 2024 saehie.park@gmail.com
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

#ifndef __RPIGPIO2_LIB_OLED128X64_H__
#define __RPIGPIO2_LIB_OLED128X64_H__

#include "i2c/i2c.h"

#include <gfxlib/gfxlib.h>

namespace rpigpio2
{

class OLED128x64 : public Gfx
{
public:
  OLED128x64();
  virtual ~OLED128x64() = default;

public:
  bool init(I2C *i2c);
  void release(void);

public:
  bool initialized(void) { return _initalized; }

public:
  void init_display(void);
  void clear_display(void);
  void flush_display(void);
  void draw_pixel(int16_t x, int16_t y, uint16_t color) override;

protected:
  void draw_fast_vline(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void draw_fast_hline(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void draw_fast_hline_internal(int16_t x, int16_t y, int16_t w, uint16_t color);
  void draw_fast_vline_internal(int16_t x, int16_t y, int16_t h, uint16_t color);

protected:
  uint8_t *_buffer = nullptr;

public:
  void test(void);

public:
  // low level SSD1306 commands
  bool cmd(uint8_t v);
  bool data(uint8_t v);
  bool data(uint8_t *b, size_t l);
  bool data(uint8_t *b, uint32_t s, uint32_t l);

  bool addrMode(uint8_t v);
  bool colAddr(uint8_t s, uint8_t e);
  bool pageAddr(uint8_t s, uint8_t e);

  void clear();

private:
  void init_ssd1306(void);
  void chk_resize_temp(size_t s);

private:
  I2C *_i2c = nullptr;
  uint8_t *_temp_buff = nullptr;
  uint32_t _temp_size = 0;
  bool _initalized = false;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_OLED128X64_H__
