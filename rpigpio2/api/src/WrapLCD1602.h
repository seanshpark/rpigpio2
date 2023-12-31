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

#ifndef __RPIGPIO2_API_WRAP_LCD1602_H__
#define __RPIGPIO2_API_WRAP_LCD1602_H__

#include "WrapNapi.h"

#include "lcd1602/lcd1602.h"

namespace rpigpio2
{

class WrapLCD1602 : public Napi::ObjectWrap<WrapLCD1602>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapLCD1602(const Napi::CallbackInfo &info);

public:
  Napi::Value API_LCD1602_init(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_release(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_clear(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_home(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_display(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_cursor(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_blink(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_puts(const Napi::CallbackInfo &info);
  Napi::Value API_LCD1602_move(const Napi::CallbackInfo &info);

public:
  LCD1602 &lcd1602() { return _lcd1602; }

protected:
  LCD1602 _lcd1602;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_LCD1602_H__
