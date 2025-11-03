/*
 * Copyright 2025 saehie.park@gmail.com
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

#ifndef __RPIGPIO2_LIB_LCDCO240_H__
#define __RPIGPIO2_LIB_LCDCO240_H__

#include "spi/spi.h"
#include "gpio/gpio2.h"

#include <gfxlib/gfxlib.h>

namespace rpigpio2
{

class LCDCO240 : public Gfx
{
public:
  LCDCO240();
  virtual ~LCDCO240() = default;

public:
  bool init(SPI *spi, GPIO *gpio, int dc);
  void release(void);

public:
  bool initialized(void) { return _initalized; }

public:
  void set_window(int16_t x0 = 0, int16_t y0 = 0, int16_t x1 = -1, int16_t y1 = -1);
  void clear_display(void);
  void flush_display(void);
  void draw_pixel(int16_t x, int16_t y, uint16_t color) override;

private:
  void send_cmd(uint8_t c);
  void send_data(uint8_t d);

private:
  SPI *_spi = nullptr;
  GPIO *_gpio = nullptr;
  int32_t _pin_dc = 0;
  bool _initalized = false;
  uint16_t *_gr_buffer = nullptr;

};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_LCDCO240_H__
