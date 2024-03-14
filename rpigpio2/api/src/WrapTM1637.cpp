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

#include "WrapTM1637.h"
#include "WrapGPIO.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapTM1637::Init(Napi::Env &env, Napi::Object &exports)
{
  std::cout << "WrapTM1637::Init" << std::endl;

  // TM1637 chip
  // clang-format off
  Napi::Function functions = DefineClass(env, "TM1637",
    {
      InstanceMethod("init",    &WrapTM1637::API_TM1637_init),
      InstanceMethod("release", &WrapTM1637::API_TM1637_release),
      InstanceMethod("write",   &WrapTM1637::API_TM1637_write),
      InstanceMethod("writes",  &WrapTM1637::API_TM1637_writes),
      InstanceMethod("bright",  &WrapTM1637::API_TM1637_bright),
      InstanceMethod("clear",   &WrapTM1637::API_TM1637_clear),
      InstanceMethod("test",    &WrapTM1637::API_TM1637_test)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  exports.Set("TM1637", functions);
}

//--------------------------------------------------------------------------------------------------

WrapTM1637::WrapTM1637(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapTM1637>(info)
{
  std::cout << "WrapTM1637::WrapTM1637 " << this << std::endl;
}

Napi::Value WrapTM1637::API_TM1637_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(gpio, cmd, dio)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  WrapGPIO *wrap_gpio = Napi::ObjectWrap<WrapGPIO>::Unwrap(gpioObj);
  std::cout << "!!! init tm1637 with gpio " << wrap_gpio << std::endl;

  if (!wrap_gpio->gpio().initialized())
    Napi::Error::New(env, "gpio not initialized").ThrowAsJavaScriptException();

  auto cmd = info[1].As<Napi::Number>();
  auto dio = info[2].As<Napi::Number>();

  if (!this->tm1637().init(&wrap_gpio->gpio(), cmd.Int32Value(), dio.Int32Value()))
    Napi::Error::New(env, "tm1637 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->tm1637().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->tm1637().write(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_writes(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  uint32_t length = info.Length();
  if (length != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(Buffer)").ThrowAsJavaScriptException();
  }

  Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();

  this->tm1637().writes(buffer.Data(), buffer.Length());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_bright(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->tm1637().bright(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->tm1637().clear();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapTM1637::API_TM1637_test(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->tm1637().test();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
