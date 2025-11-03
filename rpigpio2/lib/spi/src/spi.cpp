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

#include "spi/spi.h"

#include <bitset>
#include <iostream>
#include <cassert>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

namespace rpigpio2
{

// gpio is not used for now.
bool SPI::init(GPIO *gpio, int32_t dev, uint32_t speed)
{
  std::cout << "SPI::init " << dev << ": " << std::hex << speed << std::endl;

  // support spidev0.0 ~ spidev0.1
  if (dev < 0 || dev > 1)
    return false;

  _bits_per_word = 8;
  _mode = SPI_MODE_0;

  char devfname[128];
  sprintf(devfname, "/dev/spidev0.%d", dev);
  if ((_dev_fd = open(devfname, O_RDWR)) < 0)
  {
    std::cout << "failed to open " << devfname << std::endl;
    return false;
  }

  if (ioctl(_dev_fd, SPI_IOC_WR_MODE, &_mode) < 0)
  {
    close(_dev_fd);
    _dev_fd = -1;

    std::cout << "failed to set spi dev " << dev << std::endl;
    return false;
  }
  if (ioctl(_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &_bits_per_word))
  {
    close(_dev_fd);
    _dev_fd = -1;
    std::cout << "failed to set spi bits_per_word " << _bits_per_word << std::endl;
    return false;
  }
  if (ioctl(_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
  {
    close(_dev_fd);
    _dev_fd = -1;

    std::cout << "failed to set spi speed " << speed << std::endl;
    return false;
  }

  _gpio = gpio;
  _dev = dev;
  _speed_hz = speed;

  return true;
}

void SPI::release(void)
{
  if (_dev_fd > 0)
  {
    close(_dev_fd);
    _dev_fd = -1;
  }
  _dev = 0;
  _addr = 0;
  _gpio = nullptr;

  std::cout << "SPI::release" << std::endl;
}

bool SPI::write_byte(uint8_t b)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  // std::cout << "I2C " << std::hex << int32_t(b) << std::endl;

  if (::write(_dev_fd, &b, 1) != 1)
  {
    return false;
  }
  return true;
}

bool SPI::read_byte(uint8_t &b)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  if (::read(_dev_fd, &b, 1) != 1)
  {
    return false;
  }
  // std::cout << "I2C " << std::hex << int32_t(b) << std::endl;
  return true;
}

bool SPI::write_buffer(uint8_t *b, size_t s)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  // std::cout << "I2C write_buffer" << s << std::endl;

  if ((size_t)::write(_dev_fd, b, s) != s)
  {
    return false;
  }
  return true;
}

bool SPI::read_buffer(uint8_t *b, size_t s)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  // std::cout << "I2C read_buffer" << s << std::endl;

  if ((size_t)::read(_dev_fd, b, s) != s)
  {
    return false;
  }
  return true;
}

} // namespace rpigpio2
