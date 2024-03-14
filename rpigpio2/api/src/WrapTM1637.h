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

#ifndef __RPIGPIO2_API_WRAP_TM1637_H__
#define __RPIGPIO2_API_WRAP_TM1637_H__

#include "WrapNapi.h"

#include "tm1637/tm1637.h"

namespace rpigpio2
{

class WrapTM1637 : public Napi::ObjectWrap<WrapTM1637>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapTM1637(const Napi::CallbackInfo &info);

public:
  Napi::Value API_TM1637_init(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_release(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_write(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_writes(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_bright(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_clear(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_test(const Napi::CallbackInfo &info);

public:
  TM1637 &tm1637() { return _tm1637; }

protected:
  TM1637 _tm1637;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_TM1637_H__
