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

#include "gpio/gpio2.h"

#include <cstdint>
#include <iostream>

#include <string.h>
#include <unistd.h>

namespace rpigpio2
{

bool GPIO::init(void)
{
  std::cout << "GPIO::init" << std::endl;

  // init gpio
  int ret = _lib.init();
  if (ret < 0)
  {
    std::cerr << "Failed to initialise gpiolib - " << ret << std::endl;
    return false;
  }
  _num_gpios = ret;
  if (!_num_gpios)
  {
    std::cerr << "No GPIO chips found" << std::endl;
    return false;
  }
  // std::cerr << "Num GPIOs: " << _num_gpios << std::endl;

  _fsparam = GPIO_FSEL_OUTPUT;
  _drive = DRIVE_HIGH;

  ret = _lib.mmap();
  if (ret)
  {
    if (ret == EACCES && geteuid())
      std::cerr << "Must be root" << std::endl;
    else
      std::cerr << "Failed to mmap gpiolib - " << strerror(ret) << std::endl;
    return false;
  }

  unsigned start_pin = GPIO_INVALID, end_pin;
  _lib.get_pin_range(&start_pin, &end_pin);
  std::cerr << "start: " << start_pin << ", end_pin: " << end_pin << std::endl;

  _initialized = true;

  return true;
}

void GPIO::release(void)
{
  _lib.munmap();
  _lib.release();

  std::cout << "GPIO::release" << std::endl;
  _initialized = false;
}

void GPIO::cfg(int32_t gpio, PIN dir)
{
  // TODO
}

int GPIO::set(int32_t gpio, bool val)
{
  GPIO_DRIVE_T drive = val ? DRIVE_HIGH : DRIVE_LOW;

  if (!_lib.is_valid(gpio))
  {
    std::cerr << "Pin " << gpio << " invalid" << std::endl;
    return 1;
  }

  _lib.set_fsel(gpio, _fsparam);

  if (drive != DRIVE_MAX)
  {
    if (_fsparam == GPIO_FSEL_OUTPUT)
    {
      _lib.set_drive(gpio, drive);
    }
    else
    {
      std::cerr << "Can't set pin value, not an output" << std::endl;
      return 1;
    }
  }

  if (_pull != PULL_MAX)
    _lib.set_pull(gpio, _pull);

  return 0;
}

} // namespace rpigpio2
