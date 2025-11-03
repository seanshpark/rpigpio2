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
/*
# Copyright (c) 2014 Adafruit Industries
# Author: Tony DiCola
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
*/

// codes are from https://github.com/pimoroni/st7789-python python and
// converted to C++ by me

#include "lcdco240/lcdco240.h"

#include <iostream>
#include <cassert>
#include <cstring> // memcpy

#include <unistd.h> // usleep

#define LCD_WIDTH  240
#define LCD_HEIGHT 240

namespace
{

// clang-format off
// LCD control chip is ST7789
enum ST7789_CMD : uint8_t
{
  ST7789_NOP = 0x00,
  ST7789_SWRESET = 0x01,
  ST7789_RDDID = 0x04,
  ST7789_RDDST = 0x09,

  ST7789_SLPIN = 0x10,
  ST7789_SLPOUT = 0x11,
  ST7789_PTLON = 0x12,
  ST7789_NORON = 0x13,

  ST7789_INVOFF = 0x20,
  ST7789_INVON = 0x21,
  ST7789_DISPOFF = 0x28,
  ST7789_DISPON = 0x29,

  ST7789_CASET = 0x2A,
  ST7789_RASET = 0x2B,
  ST7789_RAMWR = 0x2C,
  ST7789_RAMRD = 0x2E,

  ST7789_PTLAR = 0x30,
  ST7789_MADCTL = 0x36,
  ST7789_COLMOD = 0x3A,

  ST7789_FRMCTR1 = 0xB1,
  ST7789_FRMCTR2 = 0xB2,
  ST7789_FRMCTR3 = 0xB3,
  ST7789_INVCTR = 0xB4,
  ST7789_DISSET5 = 0xB6,

  ST7789_GCTRL = 0xB7,
  ST7789_GTADJ = 0xB8,
  ST7789_VCOMS = 0xBB,

  ST7789_LCMCTRL = 0xC0,
  ST7789_IDSET = 0xC1,
  ST7789_VDVVRHEN = 0xC2,
  ST7789_VRHS = 0xC3,
  ST7789_VDVS = 0xC4,
  ST7789_VMCTR1 = 0xC5,
  ST7789_FRCTRL2 = 0xC6,
  ST7789_CABCCTRL = 0xC7,

  ST7789_RDID1 = 0xDA,
  ST7789_RDID2 = 0xDB,
  ST7789_RDID3 = 0xDC,
  ST7789_RDID4 = 0xDD,

  ST7789_GMCTRP1 = 0xE0,
  ST7789_GMCTRN1 = 0xE1,

  ST7789_PWCTR6 = 0xFC
};
// clang-format on

} // namespace

namespace rpigpio2
{

LCDCO240::LCDCO240() : Gfx(LCD_WIDTH, LCD_HEIGHT)
{
  // do nothing
}

bool LCDCO240::init(SPI *spi, GPIO *gpio, int dc)
{
  std::cout << "LCDCO240::init with SPI, GPIO, " << dc << std::endl;

  _spi = spi;
  _gpio = gpio;
  _pin_dc = dc;

  size_t buffer_size = LCD_WIDTH * LCD_HEIGHT;
  _gr_buffer = new uint16_t[buffer_size];

  send_cmd(ST7789_SWRESET); // Software reset
  usleep(150000);

  send_cmd(ST7789_MADCTL);
  send_data(0x70);

  send_cmd(ST7789_FRMCTR2);  // Frame rate ctrl - idle mode
  send_data(0x0C);
  send_data(0x0C);
  send_data(0x00);
  send_data(0x33);
  send_data(0x33);

  send_cmd(ST7789_COLMOD); // Set color mode to 5-6-5 mode
  send_data(0x05);

  send_cmd(ST7789_GCTRL);
  send_data(0x14);

  send_cmd(ST7789_VCOMS);
  send_data(0x37);

  send_cmd(ST7789_LCMCTRL);
  send_data(0x2C);

  send_cmd(ST7789_VDVVRHEN);
  send_data(0x01);

  send_cmd(ST7789_VRHS);
  send_data(0x12);

  send_cmd(ST7789_VDVS);
  send_data(0x20);

  send_cmd(0xD0);
  send_data(0xA4);
  send_data(0xA1);

  send_cmd(ST7789_FRCTRL2);
  send_data(0x0F);

  send_cmd(ST7789_GMCTRP1);
  send_data(0xD0);
  send_data(0x04);
  send_data(0x0D);
  send_data(0x11);
  send_data(0x13);
  send_data(0x2B);
  send_data(0x3F);
  send_data(0x54);
  send_data(0x4C);
  send_data(0x18);
  send_data(0x0D);
  send_data(0x0B);
  send_data(0x1F);
  send_data(0x23);

  send_cmd(ST7789_GMCTRN1);
  send_data(0xD0);
  send_data(0x04);
  send_data(0x0C);
  send_data(0x11);
  send_data(0x13);
  send_data(0x2C);
  send_data(0x3F);
  send_data(0x44);
  send_data(0x51);
  send_data(0x2F);
  send_data(0x1F);
  send_data(0x1F);
  send_data(0x20);
  send_data(0x23);

  send_cmd(ST7789_INVON);

  send_cmd(ST7789_SLPOUT);

  send_cmd(ST7789_DISPON);  // display on

  _initalized = true;
  usleep(100000);

  return true;
}

void LCDCO240::release(void)
{
  if (_gr_buffer)
  {
    delete [] _gr_buffer;
    _gr_buffer = nullptr;
  }

  _spi = nullptr;
  _gpio = nullptr;
  _pin_dc = 0;
  _initalized = false;

  std::cout << "LCDCO240::release" << std::endl;
}

void LCDCO240::set_window(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  if (x1 == -1)
    x1 = width() - 1;
  if (y1 == -1)
    y1 = height() - 1;

  send_cmd(ST7789_CASET); // Column addr set
  send_data(x0 >> 8);
  send_data(x0 & 0xFF);
  send_data(x1 >> 8);
  send_data(x1 & 0xFF);

  send_cmd(ST7789_RASET); // Row addr set
  send_data(y0 >> 8);
  send_data(y0 & 0xFF);
  send_data(y1 >> 8);
  send_data(y1 & 0xFF);

  send_cmd(ST7789_RAMWR); // write to RAM
}

void LCDCO240::clear_display(void)
{
  memset(_gr_buffer, 0x00, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
}

void LCDCO240::flush_display(void)
{
  set_window();

  int32_t w = width();
  int32_t h = height();
  int32_t size = w * h;
  uint8_t *ptr = reinterpret_cast<uint8_t *>(_gr_buffer);

  _gpio->set(_pin_dc, true);

  while (size > 0)
  {
    int32_t chunk = (size > 2048) ? 2048 : size;
    int32_t bytes = chunk * sizeof(uint16_t);

    _spi->write_buffer(ptr, bytes);
    size -= chunk;
    ptr += bytes;
  }
}

void LCDCO240::draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  int16_t w = width();
  int16_t h = height();
  if ((x >= 0) && (x < w) && (y >= 0) && (y < h))
  {
    uint32_t offset = (y * w) + x;
    _gr_buffer[offset] = color;
  }
}

} // namespace rpigpio2

namespace rpigpio2
{

void LCDCO240::send_cmd(uint8_t c)
{
  _gpio->set(_pin_dc, false);
  _spi->write_byte(c);
}

void LCDCO240::send_data(uint8_t d)
{
  _gpio->set(_pin_dc, true);
  _spi->write_byte(d);
}

} // namespace rpigpio2
