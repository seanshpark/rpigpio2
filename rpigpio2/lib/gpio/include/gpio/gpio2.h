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

#ifndef __RPIGPIO2_LIB_GPIO_GPIO2_H__
#define __RPIGPIO2_LIB_GPIO_GPIO2_H__

#include <gpiolib/gpiolib.h>

#include <cstdint>
#include <cstdlib>

namespace rpigpio2
{

class GPIO
{
public:
  enum PIN
  {
    IN = 1,
    OUT = 2,
    ALT0 = 3,
  };

public:
  GPIO() = default;
  virtual ~GPIO() = default;

public:
  bool init(void);
  void release(void);

  void cfg(int32_t pin, PIN dir);
  int set(int32_t pin, bool val);

  bool initialized(void) { return _initialized;}

private:
  GpioLib _lib;
  bool _initialized = false;

  uint32_t _num_gpios = 0;
  GPIO_FSEL_T _fsparam = GPIO_FSEL_MAX;
  GPIO_DRIVE_T _drive = DRIVE_MAX;
  GPIO_PULL_T _pull = PULL_MAX;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_GPIO_GPIO2_H__
