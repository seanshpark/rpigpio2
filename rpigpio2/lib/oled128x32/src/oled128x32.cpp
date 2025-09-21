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
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "oled128x32/oled128x32.h"

#include <iostream>
#include <cassert>
#include <cstring> // memcpy

#include <unistd.h> // usleep

#define OLED_WIDTH  128
#define OLED_HEIGHT 64

#define OLED_BLACK  0
#define OLED_WHITE  1
#define OLED_INVERSE 2

namespace rpigpio2
{

  OLED128x32::OLED128x32() : Gfx(OLED_WIDTH, OLED_HEIGHT)
{
  // do nothing
}

bool OLED128x32::init(I2C *i2c)
{
  std::cout << "OLED128x64::init with I2C" << std::endl;

  _i2c = i2c;

  size_t buffer_size = OLED_WIDTH * ((OLED_HEIGHT + 7) / 8);
  _buffer = new uint8_t[buffer_size];

  _temp_size = buffer_size + 32; // 32 for some margin
  _temp_buff = new uint8_t[_temp_size];
  _initalized = true;

  init_ssd1309();

  _initalized = true;

  return true;
}

void OLED128x32::release(void)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }

  if (_temp_buff)
  {
    delete _temp_buff;
    _temp_buff = nullptr;
  }
  _temp_size = 0;

  if (_buffer)
  {
    delete _buffer;
    _buffer = nullptr;
  }

  _initalized = false;
  _i2c = nullptr;

  std::cout << "OLED128x32::release" << std::endl;
}

/**
 * @brief send one command
 *
 * @param v command of SSD1309
 * @return true when success
 * @return false when failed
 */
bool OLED128x32::cmd(uint8_t v)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }
  _temp_buff[0] = 0x00; // command for SSD1309
  _temp_buff[1] = v;
  return _i2c->write_buffer(_temp_buff, 2);
}

/**
 * @brief send one byte of data
 *
 * @param v datato send
 * @return true when success
 * @return false when failed
 */
bool OLED128x32::data(uint8_t v)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  _temp_buff[0] = 0x40; // data for SSD1309
  _temp_buff[1] = v;

  return _i2c->write_buffer(_temp_buff, 2);
}

void OLED128x32::chk_resize_temp(size_t s)
{
  if (_temp_size < s + 1)
  {
    if (_temp_buff != nullptr)
    {
      delete _temp_buff;
    }
    _temp_buff = new uint8_t[s + 1];
    _temp_size = s + 1;
  }
}

/**
 * @brief send data from buffer with size
 *
 * @param b buffer address
 * @param s size of buffer
 * @return true when success
 * @return false when failed
 */
bool OLED128x32::data(uint8_t *b, size_t l)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  chk_resize_temp(l);

  _temp_buff[0] = 0x40; // data for SSD1309
  memcpy(&_temp_buff[1], b, l);

  // +1 is for command 0x40
  return _i2c->write_buffer(_temp_buff, l + 1);
}

bool OLED128x32::data(uint8_t *b, uint32_t s, uint32_t l)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  chk_resize_temp(l);

  _temp_buff[0] = 0x40; // data for SSD1309
  memcpy(&_temp_buff[1], b + s, l);

  // +1 is for command 0x40
  return _i2c->write_buffer(_temp_buff, l + 1);
}

bool OLED128x32::addrMode(uint8_t v)
{
  if (v >= 3)
    return false;

  cmd(0x20);
  cmd(v);

  return true;
}

bool OLED128x32::colAddr(uint8_t s, uint8_t e)
{
  if (s > e)
    return false;
  if (s > 127 || e > 127)
    return false;

  cmd(0x21);
  cmd(s);
  cmd(e);

  return true;
}

bool OLED128x32::pageAddr(uint8_t s, uint8_t e)
{
  if (s > e)
    return false;
  if (s > 7 || e > 7)
    return false;

  cmd(0x22);
  cmd(s);
  cmd(e);

  return true;
}

void OLED128x32::clear()
{
  clear_display();
  flush_display();
}

void OLED128x32::test(void)
{
  // TODO
}

void OLED128x32::init_ssd1309(void)
{
  // https://github.com/iliapenev/ssd1306_i2c/blob/master/ssd1306_i2c.c

  cmd(0xAE); // SSD1306_DISPLAYOFF

  cmd(0xD5); // SSD1306_SETDISPLAYCLOCKDIV
  cmd(0x80);
  cmd(0xA8); // SSD1306_SETMULTIPLEX, 32-1
  cmd(OLED_HEIGHT - 1);
  cmd(0xD3); // SSD1306_SETDISPLAYOFFSET
  cmd(0x00);

  cmd(0xb0); // page start address mode 0

  cmd(0x40 | 0x00); // SSD1306_SETSTARTLINE + line# 0

  cmd(0x20); // SSD1306_MEMORYMODE
  cmd(0x00);
  // b00: Horz addressing mode
  // b01: vert addressing mode
  // b10: page addressing mode
  // b11: invalid

  cmd(0xA0 | 0x01); // SSD1306_SEGREMAP
  cmd(0xC0);        // SSD1306_COMSCANDEC <--> 0xC8
  // c0 : up -> down
  // c8 : down -> up
  cmd(0xDA); // SSD1306_SETCOMPINS
  cmd(0x12); // SSD1306_SETCOMPINS

  cmd(0xA4); // SSD1306_DISPLAYALLON_RESUME
  cmd(0xA6); // SSD1306_NORMALDISPLAY <-> 0xA7(inverse)
  cmd(0x2E); // SSD1306_DEACTIVATE_SCROLL

  cmd(0x81); // SSD1306_SETCONTRAST, internal vcc
  cmd(0xCF);
  cmd(0xD9); // SSD1306_SETPRECHARGE, internal vcc
  cmd(0xF1);
  cmd(0xDB); // SSD1306_SETVCOMDETECT, 0x00, 0x20(reset), 0x30
  cmd(0x20);
  cmd(0xD5); // Set OSC Freq, 0x80(reset)
  cmd(0x80);

  // chargepump sequence must be 0x8d, 0x14, 0xaf
  cmd(0x8D); // SSD1306_CHARGEPUMP,
  cmd(0x14);
  cmd(0xAF); // SSD1306_DISPLAYON
}

void OLED128x32::clear_display(void)
{
  memset(_buffer, 0, OLED_WIDTH * ((OLED_HEIGHT + 7) / 8));
}

void OLED128x32::flush_display(void)
{
  size_t data_size = OLED_WIDTH * ((OLED_HEIGHT + 7) / 8);
  _temp_buff[0] = 0x40; // data for SSD1306
  memcpy(&_temp_buff[1], _buffer, data_size);
  _i2c->write_buffer(_temp_buff, 1 + data_size);
}

void OLED128x32::draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  y *= 2;
  if ((x >= 0) && (x < width()) && (y >= 0) && (y < height()))
  {
    // Pixel is in-bounds. Rotate coordinates if needed.
    // TODO rotation
    switch (color)
    {
    case OLED_WHITE:
      _buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7));
      break;
    case OLED_BLACK:
      _buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7));
      break;
    case OLED_INVERSE:
      _buffer[x + (y / 8) * OLED_WIDTH] ^= (1 << (y & 7));
      break;
    }
  }
}

void OLED128x32::draw_fast_hline(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  draw_fast_hline_internal(x, y, w, color);
}

void OLED128x32::draw_fast_vline(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  draw_fast_vline_internal(x, y, h, color);
}

void OLED128x32::draw_fast_hline_internal(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  y *= 2;
  if ((y >= 0) && (y < OLED_HEIGHT)) // Y coord in bounds?
  {
    if (x < 0) // Clip left
    {
      w += x;
      x = 0;
    }
    if ((x + w) > OLED_WIDTH) // Clip right
    {
      w = (OLED_WIDTH - x);
    }
    if (w > 0) // Proceed only if width is positive
    {
      uint8_t *pBuf = &_buffer[(y / 8) * OLED_WIDTH + x], mask = 1 << (y & 7);
      switch (color)
      {
      case OLED_WHITE:
        while (w--)
        {
          *pBuf++ |= mask;
        };
        break;
      case OLED_BLACK:
        mask = ~mask;
        while (w--)
        {
          *pBuf++ &= mask;
        };
        break;
      case OLED_INVERSE:
        while (w--)
        {
          *pBuf++ ^= mask;
        };
        break;
      }
    }
  }
}

void OLED128x32::draw_fast_vline_internal(int16_t x, int16_t __y, int16_t __h, uint16_t color)
{
  __y *= 2;
  if ((x >= 0) && (x < OLED_WIDTH)) // X coord in bounds?
  {
    if (__y < 0) // Clip top
    {
      __h += __y;
      __y = 0;
    }
    if ((__y + __h) > OLED_HEIGHT) // Clip bottom
    {
      __h = (OLED_HEIGHT - __y);
    }
    if (__h > 0) // Proceed only if height is now positive
    {
      // this display doesn't need ints for coordinates,
      // use local byte registers for faster juggling
      uint8_t y = __y, h = __h;
      uint8_t *pBuf = &_buffer[(y / 8) * OLED_WIDTH + x];

      // do the first partial byte, if necessary - this requires some masking
      uint8_t mod = (y & 7);
      if (mod)
      {
        // mask off the high n bits we want to set
        mod = 8 - mod;
        // note - lookup table results in a nearly 10% performance
        // improvement in fill* functions
        // uint8_t mask = ~(0xFF >> mod);
        static const uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE};
        uint8_t mask = premask[mod];
        // adjust the mask if we're not going to reach the end of this byte
        if (h < mod)
          mask &= (0XFF >> (mod - h));

        switch (color) {
        case OLED_WHITE:
          *pBuf |= mask;
          break;
        case OLED_BLACK:
          *pBuf &= ~mask;
          break;
        case OLED_INVERSE:
          *pBuf ^= mask;
          break;
        }
        pBuf += OLED_WIDTH;
      }

      if (h >= mod) // More to go?
      {
        h -= mod;
        // Write solid bytes while we can - effectively 8 rows at a time
        if (h >= 8)
        {
          if (color == OLED_INVERSE)
          {
            // separate copy of the code so we don't impact performance of
            // black/white write version with an extra comparison per loop
            do
            {
              *pBuf ^= 0xFF;      // Invert byte
              pBuf += OLED_WIDTH; // Advance pointer 8 rows
              h -= 8;             // Subtract 8 rows from height
            } while (h >= 8);
          }
          else
          {
            // store a local value to work with
            uint8_t val = (color != OLED_BLACK) ? 255 : 0;
            do
            {
              *pBuf = val;        // Set byte
              pBuf += OLED_WIDTH; // Advance pointer 8 rows
              h -= 8;             // Subtract 8 rows from height
            } while (h >= 8);
          }
        }

        if (h) // Do the final partial byte, if necessary
        {
          mod = h & 7;
          // this time we want to mask the low bits of the byte,
          // vs the high bits we did above
          // uint8_t mask = (1 << mod) - 1;
          // note - lookup table results in a nearly 10% performance
          // improvement in fill* functions
          static const uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
          uint8_t mask = postmask[mod];
          switch (color)
          {
          case OLED_WHITE:
            *pBuf |= mask;
            break;
          case OLED_BLACK:
            *pBuf &= ~mask;
            break;
          case OLED_INVERSE:
            *pBuf ^= mask;
            break;
          }
        }
      }
    } // endif positive height
  } // endif x in bounds
}

} // namespace rpigpio2
