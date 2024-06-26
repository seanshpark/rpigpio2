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

#ifndef __RPIGPIO2_LIB_TM1637_H__
#define __RPIGPIO2_LIB_TM1637_H__

#include "gpio/gpio2.h"

namespace rpigpio2
{

class TM1637
{
public:
  TM1637() = default;
  virtual ~TM1637() = default;

public:
  bool init(GPIO *gpio, int32_t clock, int32_t dio);
  void release(void);

  void write(uint8_t data);
  void writes(uint8_t *data, int32_t length);
  void bright(uint8_t value);
  void clear(void);

  void test(void);

public:
  bool initialized(void) { return _initalized; }

private:
  void dio_start(void);
  void dio_stop(void);
  void write_byte(uint8_t b);
  void skip_ack(void);

private:
  GPIO *_gpio = nullptr;
  int32_t _pin_clock = 0;
  int32_t _pin_dio = 0;
  bool _initalized = false;
};

/**
 * TM1637 brightness 0 ~ 8
 *        0 Turn off
 *        1 1/16
 *        2 2/16
 *        3 4/16
 *        4 10/16
 *        5 11/16
 *        6 12/16
 *        7 13/16
 *        8 14/16
 */

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_TM1637_H__
