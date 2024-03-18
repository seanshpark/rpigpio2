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

#ifndef __RPIGPIO2_API_WRAP_GPIO_H__
#define __RPIGPIO2_API_WRAP_GPIO_H__

#include "WrapNapi.h"

#include "gpio/gpio2.h"

namespace rpigpio2
{

class WrapGPIO : public Napi::ObjectWrap<WrapGPIO>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapGPIO(const Napi::CallbackInfo &info);

public:
  Napi::Value API_GPIO_init(const Napi::CallbackInfo &info);
  Napi::Value API_GPIO_release(const Napi::CallbackInfo &info);
  Napi::Value API_GPIO_pin(const Napi::CallbackInfo &info);
  Napi::Value API_GPIO_set(const Napi::CallbackInfo &info);
  Napi::Value API_GPIO_delay(const Napi::CallbackInfo &info);

public:
  GPIO &gpio() { return _gpio; }

protected:
  GPIO _gpio;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_GPIO_H__
