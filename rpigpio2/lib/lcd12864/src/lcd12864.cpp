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

#include "lcd12864/lcd12864.h"

#include <iostream>
#include <cassert>
#include <cstring> // memcpy

#include <unistd.h> // usleep

#define LCD_WIDTH  128
#define LCD_HEIGHT 64

#define LCD_BLACK  0
#define LCD_WHITE  1
#define LCD_INVERSE 2

namespace
{

// clang-format off
// LCD control chip is ST7920

#define LCD_CMD_DDRAMADDR     0b10000000
#define LCD_CMD_GRRAMADDR     LCD_CMD_DDRAMADDR
#define LCD_CMD_CGRAMADDR     0b01000000
#define LCD_CMD_FUNCSET       0b00100000
#define LCD_CMD_CURSOR        0b00010000
#define LCD_CMD_DISPLAY       0b00001000
#define LCD_CMD_ENTRY         0b00000100
#define LCD_CMD_RETHOME       0b00000010
#define LCD_CMD_CLEAR         0b00000001

// LCD_CMD_FUNCSET
#define LCD_FUNCSET_8BIT      0b00010000
#define LCD_FUNCSET_4BIT      0b00000000
#define LCD_FUNCSET_EXT       0b00000100
#define LCD_FUNCSET_BASIC     0b00000000
#define LCD_FUNCSET_EX_GRON   0b00000010
#define LCD_FUNCSET_EX_GROFF  0b00000000

// LCD_CMD_DISPLAY
#define LCD_DISPLAY_ON        0b00000100
#define LCD_DISPLAY_OFF       0b00000000
#define LCD_DISPLAY_CUR       0b00000010
#define LCD_DISPLAY_CUR_OFF   0b00000000
#define LCD_DISPLAY_BLINK     0b00000001
#define LCD_DISPLAY_BLINK_OFF 0b00000000

// MCP23018 to LCD12864 PortA bits for control
#define MCP_LCD_RS 0b10000000 // LCD RS (Register Select: Inst=0 / Data=1)
#define MCP_LCD_RW 0b01000000 // LCD RW (R=0 or W=1)
#define MCP_LCD_EN 0b00100000 // LCD EN (Enable=1)
#define MCP_LCD_PS 0b00010000 // LCD PSB (Parallel=1 / Serial=0)
#define MCP_LCD_RT 0b00000100 // LCD RESET (Reset=0 / Norm=1)
#define MCP_LCD_BL 0b00000010 // LCD BL (BackLignt)

// clang-format on

} // namespace

namespace rpigpio2
{

LCD12864::LCD12864() : Gfx(LCD_WIDTH, LCD_HEIGHT)
{
  // do nothing
}

void LCD12864::init_cmd_delay(uint8_t cmd, uint32_t delay)
{
  send_cmd(cmd);
  usleep(delay);
}

bool LCD12864::init(MCP23017 *mcp23017)
{
  std::cout << "LCD12864::init with MCP23017" << std::endl;

  _mcp23017 = mcp23017;
  _gr_mode = false;

  _back_light = true;
  _display = false;
  _cursor = false;
  _blink = false;

  size_t buffer_size = LCD_WIDTH * ((LCD_HEIGHT + 7) / 8);
  _gr_buffer = new uint8_t[buffer_size];

  // from ST7920 data sheet

  // reset
  update_ctrl(MCP_LCD_RT | MCP_LCD_PS);
  usleep(10000);
  update_ctrl(MCP_LCD_PS);
  usleep(50000);

  // begin initialize sequence
  init_cmd_delay(LCD_CMD_FUNCSET | LCD_FUNCSET_8BIT, 110);
  init_cmd_delay(LCD_CMD_FUNCSET | LCD_FUNCSET_8BIT, 40);
  init_cmd_delay(LCD_CMD_DISPLAY, 110);
  init_cmd_delay(LCD_CMD_CLEAR, 12000);
  init_cmd_delay(LCD_CMD_ENTRY | LCD_CMD_RETHOME, 1000);
  // end initialize sequence

  // clear
  init_cmd_delay(LCD_CMD_DISPLAY | LCD_DISPLAY_ON, 1000);
  init_cmd_delay(LCD_CMD_RETHOME, 1000);

  return true;
}

void LCD12864::release(void)
{
  // update_ctrl(MCP_LCD_RT | MCP_LCD_PS); // do not reset

  if (_gr_buffer)
  {
    delete _gr_buffer;
    _gr_buffer = nullptr;
  }

  _mcp23017 = nullptr;
  _initalized = false;

  std::cout << "LCD12864::release" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void LCD12864::clear()
{
  if (_gr_mode)
  {
    clear_display();
    flush_display();
    return;
  }
  uint8_t cmd = LCD_CMD_CLEAR;
  send_cmd(cmd);
  usleep(16000); // 1.6 msec
}

void LCD12864::home()
{
  uint8_t cmd = LCD_CMD_RETHOME;
  send_cmd(cmd);
  usleep(72);
}

void LCD12864::display(bool enable)
{
  _back_light = enable;
  _display = enable;
  display_set();
  usleep(72);
}

void LCD12864::cursor(bool enable)
{
  _cursor = enable;
  display_set();
  usleep(72);
}

void LCD12864::blink(bool enable)
{
  _blink = enable;
  display_set();
  usleep(72);
}

void LCD12864::move(uint8_t row, uint8_t col)
{
  // there are 16 ascii characters per line,
  // but only even address can be set per line
  // this is for positioning two byte characters (Hangul, Hanja)
  static const uint8_t addr[] = { 0x00, 0x10, 0x08, 0x18};

  uint8_t cmd = LCD_CMD_DDRAMADDR;
  uint8_t ram_offset;

  if (row > 0x03)
    row = 0x03;
  if (col > 0x0f)
    col = 0x0f;

  ram_offset = addr[row] + col;
  ram_offset &= 0b01111111;
  cmd = cmd | ram_offset;
  send_cmd(cmd);
  usleep(72);
}

void LCD12864::puts(const char *str)
{
  while (*str != '\x00')
  {
    putc(*str++);
  }
}

void LCD12864::putch(uint8_t ch)
{
  send_data(ch);
}

void LCD12864::graphic(bool enable)
{
  _gr_mode = enable;
  if (enable)
  {
    function_set(LCD_FUNCSET_8BIT);
    usleep(80);
    function_set(LCD_FUNCSET_8BIT | LCD_FUNCSET_EXT);
    usleep(80);
    function_set(LCD_FUNCSET_8BIT | LCD_FUNCSET_EXT | LCD_FUNCSET_EX_GRON);
    usleep(80);
    return;
  }
  function_set(LCD_FUNCSET_8BIT);
  usleep(80);
  function_set(LCD_FUNCSET_8BIT | LCD_FUNCSET_EXT);
  usleep(80);
  function_set(LCD_FUNCSET_8BIT | LCD_FUNCSET_EXT);
  usleep(80);
}

void LCD12864::clear_display(void)
{
  memset(_gr_buffer, 0, LCD_WIDTH * ((LCD_HEIGHT + 7) / 8));
}

void LCD12864::flush_display(void)
{
  uint8_t y, x;
  uint8_t cmd;
  uint32_t offset;

  offset = 0;
  // upper half
  for (y = 0; y < 32; y++)
  {
    cmd = LCD_CMD_GRRAMADDR | y;
    send_cmd(cmd); // y pos
    cmd = LCD_CMD_GRRAMADDR;
    send_cmd(cmd); // x pos
    for (x = 0; x < 16; x++)
    {
      send_data(_gr_buffer[offset++]);
    }
  }
  for (y = 0; y < 32; y++)
  {
    cmd = LCD_CMD_GRRAMADDR | y;
    send_cmd(cmd); // y pos
    cmd = LCD_CMD_GRRAMADDR | 8;
    send_cmd(cmd); // x pos
    for (x = 0; x < 16; x++)
    {
      send_data(_gr_buffer[offset++]);
    }
  }
}

void LCD12864::draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < width()) && (y >= 0) && (y < height()))
  {
    // Pixel is in-bounds. Rotate coordinates if needed.
    // TODO rotation
     uint32_t offset = (y << 4) + (x >> 3);
    switch (color)
    {
    case LCD_WHITE:
      _gr_buffer[offset] |= (0x80 >> (x & 7));
      break;
    case LCD_BLACK:
      _gr_buffer[offset] &= ~(0x80 >> (x & 7));
      break;
    case LCD_INVERSE:
      _gr_buffer[offset] ^= (0x80 >> (x & 7));
      break;
    }
  }
}

void LCD12864::bitmap(int16_t xpos, int16_t ypos, int16_t width, int16_t height, uint8_t *buff, size_t l)
{
  uint32_t offset;
  uint8_t yy, y1, y2;
  uint8_t xx, x1, xw;
  uint8_t cmdy, cmdx;

  printf("!!! bitmap, %d, %d, %d, %d\r\n", xpos, ypos, width, height);

  y1 = ypos;
  y2 = y1 + height;
  x1 = xpos >> 3;
  xw = width >> 3;

  printf("   > %d, %d, %d, %d\r\n", (int32_t)y1, (int32_t)y2, (int32_t)x1, (int32_t)xw);
  for (yy = y1; yy < y2; ++yy)
  {
    for (xx = 0; xx < xw; xx++)
    {
      if (yy < 32)
      {
        cmdy = LCD_CMD_GRRAMADDR | yy;
        cmdx = LCD_CMD_GRRAMADDR | ((x1 + xx) >> 1);
      }
      else
      {
        cmdy = LCD_CMD_GRRAMADDR | (yy - 32);
        cmdx = LCD_CMD_GRRAMADDR | 0x08 | ((x1 + xx) >> 1);
      }
      send_cmd(cmdy);
      send_cmd(cmdx);
      send_data(buff[offset++]);
      xx++;
      if (xx < xw) {
        send_data(buff[offset++]);
      }
    }
    if (offset > l)
      return;
  }
}

void LCD12864::test(void)
{
  // test
}

} // namespace rpigpio2

namespace rpigpio2
{

void LCD12864::function_set(uint8_t data)
{
  uint8_t cmd = LCD_CMD_FUNCSET;
  cmd |= data;
  send_cmd(cmd);
  usleep(72);
}

void LCD12864::display_set(void)
{
  uint8_t cmd = LCD_CMD_DISPLAY;
  cmd |= _display ? LCD_DISPLAY_ON : 0;
  cmd |= _cursor ? LCD_DISPLAY_CUR : 0;
  cmd |= _blink ? LCD_DISPLAY_BLINK : 0;
  send_cmd(cmd);
  usleep(72);
}

void LCD12864::putc(const char c)
{
  send_data(c);
}

uint8_t LCD12864::make_ctrl(void)
{
  uint8_t ctrl = MCP_LCD_RT | MCP_LCD_PS; // Normal mode (NOT reset), Parallel
  ctrl |= _back_light ? MCP_LCD_BL : 0;
  return ctrl;
}

void LCD12864::send_data(uint8_t data)
{
  uint8_t ctrl = make_ctrl();
  // RS=1 for data (not instuction)
  // RW=0 for write
  ctrl |= MCP_LCD_RS;
  ctrl &= ~MCP_LCD_RW;
  update_ctrl(ctrl);
  //usleep(1);
  _mcp23017->writeB(data);
  //usleep(1);

  ctrl |= MCP_LCD_EN;
  update_ctrl(ctrl);
  usleep(1);

  ctrl &= ~MCP_LCD_EN;
  update_ctrl(ctrl);
  //usleep(1);
}

void LCD12864::send_cmd(uint8_t cmd)
{
  uint8_t ctrl = make_ctrl();
  // RS=0 for instruction (not data)
  // RW=0 for write
  ctrl &= ~MCP_LCD_RS;
  ctrl &= ~MCP_LCD_RW;
  update_ctrl(ctrl);
  //usleep(1);
  _mcp23017->writeB(cmd);
  //usleep(1);

  ctrl |= MCP_LCD_EN;
  update_ctrl(ctrl);
  usleep(1);

  ctrl &= ~MCP_LCD_EN;
  update_ctrl(ctrl);
  //usleep(1);
}

void LCD12864::update_ctrl(uint8_t data)
{
  _mcp23017->writeA(data);
}

} // namespace rpigpio2
