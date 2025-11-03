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

#ifndef __RPIGPIO2_LIB_SPI_H__
#define __RPIGPIO2_LIB_SPI_H__

#include "gpio/gpio2.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

namespace rpigpio2
{

class SPI
{
public:
  SPI() = default;
  virtual ~SPI() = default;

public:
  bool init(GPIO *gpio, int32_t dev, uint32_t speed);
  void release(void);

public:
  bool initialized(void) { return _dev_fd > 0; }

public:
  bool write_byte(uint8_t b);
  bool write_buffer(uint8_t *b, size_t s);
  bool read_buffer(uint8_t *b, size_t s);

  bool read_byte(uint8_t &b);

private:
  GPIO *_gpio = nullptr; // not used for now
  int _dev_fd = -1;
  int32_t _dev = 0;  // /dev/spidevX.Y
  int32_t _addr = 0; // SPI chip select address
  uint8_t _mode = SPI_MODE_0; // Or SPI_MODE_1, SPI_MODE_2, SPI_MODE_3
  uint8_t _bits_per_word = 8;
  uint32_t _speed_hz = 500000; // 500 kHz
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_SPI_H__
