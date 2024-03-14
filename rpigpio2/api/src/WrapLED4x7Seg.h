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

#ifndef __RPIGPIO2_API_WRAP_LED4X7SEG_H__
#define __RPIGPIO2_API_WRAP_LED4X7SEG_H__

#include "WrapNapi.h"

#include "led4x7seg/led4x7seg.h"

namespace rpigpio2
{

class WrapLED4x7Seg : public Napi::ObjectWrap<WrapLED4x7Seg>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapLED4x7Seg(const Napi::CallbackInfo &info);

public:
  Napi::Value API_LED4x7Seg_init(const Napi::CallbackInfo &info);
  Napi::Value API_LED4x7Seg_release(const Napi::CallbackInfo &info);
  Napi::Value API_LED4x7Seg_show(const Napi::CallbackInfo &info);
  Napi::Value API_LED4x7Seg_bright(const Napi::CallbackInfo &info);
  Napi::Value API_LED4x7Seg_clear(const Napi::CallbackInfo &info);

public:
  LED4x7Seg &led4x7seg() { return _led4x7seg; }

protected:
  LED4x7Seg _led4x7seg;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_LED4X7SEG_H__
