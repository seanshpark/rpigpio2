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

#ifndef __RPIGPIO2_API_WRAP_I2C_H__
#define __RPIGPIO2_API_WRAP_I2C_H__

#include "WrapNapi.h"

#include "i2c/i2c.h"

namespace rpigpio2
{

class WrapI2C : public Napi::ObjectWrap<WrapI2C>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapI2C(const Napi::CallbackInfo &info);

public:
  Napi::Value API_I2C_init(const Napi::CallbackInfo &info);
  Napi::Value API_I2C_release(const Napi::CallbackInfo &info);
  Napi::Value API_I2C_write(const Napi::CallbackInfo &info);

public:
  I2C &i2c() { return _i2c; }

protected:
  I2C _i2c;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_I2C_H__
