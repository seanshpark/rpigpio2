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

#ifndef __RPIGPIO2_API_WRAP_OLED128X32_H__
#define __RPIGPIO2_API_WRAP_OLED128X32_H__

#include "WrapNapi.h"

#include "oled128x32/oled128x32.h"

namespace rpigpio2
{

class WrapOLED128x32 : public Napi::ObjectWrap<WrapOLED128x32>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
WrapOLED128x32(const Napi::CallbackInfo &info);

public:
  Napi::Value API_OLED128x32_init(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_release(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_cmd(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_data(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_addrMode(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_colAddr(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_pageAddr(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_clear(const Napi::CallbackInfo &info);
  Napi::Value API_OLED128x32_line(const Napi::CallbackInfo &info);

public:
  OLED128x32 &oled128x32() { return _oled128x32; }

protected:
  OLED128x32 _oled128x32;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_OLED128X32_H__
