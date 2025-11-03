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

#ifndef __RPIGPIO2_API_WRAP_LCDCO240_H__
#define __RPIGPIO2_API_WRAP_LCDCO240_H__

#include "WrapNapi.h"

#include "lcdco240/lcdco240.h"

namespace rpigpio2
{

class WrapLCDCO240 : public Napi::ObjectWrap<WrapLCDCO240>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapLCDCO240(const Napi::CallbackInfo &info);

public:
  Napi::Value API_LCDCO240_init(const Napi::CallbackInfo &info);
  Napi::Value API_LCDCO240_release(const Napi::CallbackInfo &info);

  Napi::Value API_LCDCO240_clear(const Napi::CallbackInfo &info);
  Napi::Value API_LCDCO240_display(const Napi::CallbackInfo &info);
  Napi::Value API_LCDCO240_line(const Napi::CallbackInfo &info);

public:
  LCDCO240 &lcdco240() { return _lcdco240; }

protected:
  LCDCO240 _lcdco240;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_LCDCO240_H__
