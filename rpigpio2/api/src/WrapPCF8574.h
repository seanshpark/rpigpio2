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

#ifndef __RPIGPIO2_API_WRAP_PCF8574_H__
#define __RPIGPIO2_API_WRAP_PCF8574_H__

#include "WrapNapi.h"

#include "pcf8574/pcf8574.h"

namespace rpigpio2
{

class WrapPCF8574 : public Napi::ObjectWrap<WrapPCF8574>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapPCF8574(const Napi::CallbackInfo &info);

public:
  Napi::Value API_PCF8574_init(const Napi::CallbackInfo &info);
  Napi::Value API_PCF8574_release(const Napi::CallbackInfo &info);
  Napi::Value API_PCF8574_write(const Napi::CallbackInfo &info);

public:
  PCF8574 &pcf8574() { return _pcf8574; }

protected:
  PCF8574 _pcf8574;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_PCF8574_H__
