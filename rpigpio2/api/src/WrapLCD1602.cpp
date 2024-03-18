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

#include "WrapLCD1602.h"
#include "WrapPCF8574.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapLCD1602::Init(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function functions = DefineClass(env, "LCD1602",
    {
      InstanceMethod("init", &WrapLCD1602::API_LCD1602_init),
      InstanceMethod("release", &WrapLCD1602::API_LCD1602_release),

      InstanceMethod("clear", &WrapLCD1602::API_LCD1602_clear),
      InstanceMethod("home", &WrapLCD1602::API_LCD1602_home),
      InstanceMethod("display", &WrapLCD1602::API_LCD1602_display),
      InstanceMethod("cursor", &WrapLCD1602::API_LCD1602_cursor),
      InstanceMethod("blink", &WrapLCD1602::API_LCD1602_blink),
      InstanceMethod("puts", &WrapLCD1602::API_LCD1602_puts),
      InstanceMethod("putc", &WrapLCD1602::API_LCD1602_putc),
      InstanceMethod("move", &WrapLCD1602::API_LCD1602_move),
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  // LCD1602
  exports.Set("LCD1602", functions);
}

//--------------------------------------------------------------------------------------------------

WrapLCD1602::WrapLCD1602(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapLCD1602>(info)
{
  std::cout << "WrapLCD1602::WrapLCD1602 " << this << std::endl;
}

Napi::Value WrapLCD1602::API_LCD1602_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(pcf8574)").ThrowAsJavaScriptException();

  auto pcf8574Obj = info[0].As<Napi::Object>();
  WrapPCF8574 *wrap_pcf8574 = Napi::ObjectWrap<WrapPCF8574>::Unwrap(pcf8574Obj);
  std::cout << "!!! init lcd1602 with pcf8574 " << wrap_pcf8574 << std::endl;

  if (!wrap_pcf8574->pcf8574().initialized())
    Napi::Error::New(env, "lcd1602: pcf8574 not initialized").ThrowAsJavaScriptException();

  if (!this->lcd1602().init(&wrap_pcf8574->pcf8574()))
    Napi::Error::New(env, "lcd1602 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().clear();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_home(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().home();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_display(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "display " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().display(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_cursor(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "cursor " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().cursor(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_blink(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "blink " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().blink(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_puts(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::String>();
  std::string value = std::string(data);
  this->lcd1602().puts(value.c_str());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_putc(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto ch = info[0].As<Napi::Number>();
  this->lcd1602().putch(ch.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD1602::API_LCD1602_move(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 argument(row, col)").ThrowAsJavaScriptException();
  }

  auto row = info[0].As<Napi::Number>();
  auto col = info[1].As<Napi::Number>();
  this->lcd1602().move(row.Int32Value() & 0xff, col.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
