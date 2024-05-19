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

#ifndef __RPIGPIO2_API_WRAP_LCD12864_H__
#define __RPIGPIO2_API_WRAP_LCD12864_H__

#include "WrapNapi.h"

#include "lcd12864/lcd12864.h"

namespace rpigpio2
{

class WrapLCD12864 : public Napi::ObjectWrap<WrapLCD12864>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapLCD12864(const Napi::CallbackInfo &info);

public:
  Napi::Value API_LCD12864_init(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_release(const Napi::CallbackInfo &info);

  Napi::Value API_LCD12864_clear(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_home(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_display(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_cursor(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_blink(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_move(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_puts(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_putc(const Napi::CallbackInfo &info);

  Napi::Value API_LCD12864_graphic(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_line(const Napi::CallbackInfo &info);
  Napi::Value API_LCD12864_bitmap(const Napi::CallbackInfo &info);

  Napi::Value API_LCD12864_test(const Napi::CallbackInfo &info);

public:
  LCD12864 &lcd12864() { return _lcd12864; }

protected:
  LCD12864 _lcd12864;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_LCD12864_H__
