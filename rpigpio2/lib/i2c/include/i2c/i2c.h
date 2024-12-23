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

#ifndef __RPIGPIO2_LIB_I2C_H__
#define __RPIGPIO2_LIB_I2C_H__

#include "gpio/gpio2.h"

namespace rpigpio2
{

class I2C
{
public:
  I2C() = default;
  virtual ~I2C() = default;

public:
  bool init(GPIO *gpio, int32_t dev, int32_t addr);
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
  int32_t _dev = 0;  // /dev/i2c-?
  int32_t _addr = 0; // I2C slave address
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_I2C_H__
