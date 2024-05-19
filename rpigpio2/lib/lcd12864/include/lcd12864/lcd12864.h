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

#ifndef __RPIGPIO2_LIB_LCD12864_H__
#define __RPIGPIO2_LIB_LCD12864_H__

#include "mcp23017/mcp23017.h"

#include <gfxlib/gfxlib.h>

namespace rpigpio2
{

class LCD12864 : public Gfx
{
public:
  LCD12864();
  virtual ~LCD12864() = default;

public:
  bool init(MCP23017 *mcp23017);
  void release(void);

public:
  bool initialized(void) { return _initalized; }

public:
  void clear();
  void home();
  void display(bool enable);  // display on/off
  void cursor(bool enable);  // cursor on/off
  void blink(bool enable);   // blinking on/off
  void move(uint8_t row, uint8_t col);
  void puts(const char *str);
  void putch(uint8_t ch);

  void graphic(bool enable);  // graphics mode
  void clear_display(void);
  void flush_display(void);
  void draw_pixel(int16_t x, int16_t y, uint16_t color) override;
  void bitmap(int16_t xpos, int16_t ypos, int16_t width, int16_t height, uint8_t *buff, size_t l);

  void test(void);

private:
  void init_cmd_delay(uint8_t cmd, uint32_t delay);
  void function_set(uint8_t data);
  void display_set(void);

  void putc(const char c);
  void send_data(uint8_t data);
  void send_cmd(uint8_t cmd);

  uint8_t make_ctrl(void);
  void update_ctrl(uint8_t data);

private:
  MCP23017 *_mcp23017 = nullptr;
  bool _initalized = false;
  bool _gr_mode = false;
  uint8_t *_gr_buffer = nullptr;

  bool _back_light = false;
  bool _display = false;
  bool _cursor = false;
  bool _blink = false;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_LCD12864_H__
