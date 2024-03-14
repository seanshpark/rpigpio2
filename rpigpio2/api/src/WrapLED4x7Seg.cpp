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

#include "WrapLED4x7Seg.h"
#include "WrapTM1637.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapLED4x7Seg::Init(Napi::Env &env, Napi::Object &exports)
{
  std::cout << "WrapLED4x7Seg::Init" << std::endl;

  // LED4x7Seg module
  // clang-format off
  Napi::Function functions = DefineClass(env, "LED4x7Seg",
    {
      InstanceMethod("init",    &WrapLED4x7Seg::API_LED4x7Seg_init),
      InstanceMethod("release", &WrapLED4x7Seg::API_LED4x7Seg_release),
      InstanceMethod("show",    &WrapLED4x7Seg::API_LED4x7Seg_show),
      InstanceMethod("bright",  &WrapLED4x7Seg::API_LED4x7Seg_bright),
      InstanceMethod("clear",   &WrapLED4x7Seg::API_LED4x7Seg_clear)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  exports.Set("LED4x7Seg", functions);
}

//--------------------------------------------------------------------------------------------------

WrapLED4x7Seg::WrapLED4x7Seg(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapLED4x7Seg>(info)
{
  std::cout << "WrapLED4x7Seg::WrapLED4x7Seg " << this << std::endl;
}

Napi::Value WrapLED4x7Seg::API_LED4x7Seg_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(tm1637)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  WrapTM1637 *wrapper = Napi::ObjectWrap<WrapTM1637>::Unwrap(gpioObj);
  std::cout << "!!! init led4x7seg with tm1637 " << wrapper << std::endl;

  if (!this->led4x7seg().init(&wrapper->tm1637()))
    Napi::Error::New(env, "led4x7seg init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLED4x7Seg::API_LED4x7Seg_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->led4x7seg().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLED4x7Seg::API_LED4x7Seg_show(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::String>();

  std::string value = std::string(data);
  if (value.length() != 5)
  {
    Napi::Error::New(env, "data length not 5)").ThrowAsJavaScriptException();
  }

  this->led4x7seg().show(value);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLED4x7Seg::API_LED4x7Seg_bright(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->led4x7seg().bright(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLED4x7Seg::API_LED4x7Seg_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->led4x7seg().clear();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
