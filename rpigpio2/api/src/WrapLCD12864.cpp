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

#include "WrapLCD12864.h"
#include "WrapMCP23017.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapLCD12864::Init(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function functions = DefineClass(env, "LCD12864",
    {
      InstanceMethod("init", &WrapLCD12864::API_LCD12864_init),
      InstanceMethod("release", &WrapLCD12864::API_LCD12864_release),

      InstanceMethod("clear", &WrapLCD12864::API_LCD12864_clear),
      InstanceMethod("home", &WrapLCD12864::API_LCD12864_home),
      InstanceMethod("display", &WrapLCD12864::API_LCD12864_display),
      InstanceMethod("cursor", &WrapLCD12864::API_LCD12864_cursor),
      InstanceMethod("blink", &WrapLCD12864::API_LCD12864_blink),
      InstanceMethod("move", &WrapLCD12864::API_LCD12864_move),
      InstanceMethod("puts", &WrapLCD12864::API_LCD12864_puts),
      InstanceMethod("putc", &WrapLCD12864::API_LCD12864_putc),

      InstanceMethod("graphic", &WrapLCD12864::API_LCD12864_graphic),
      InstanceMethod("line", &WrapLCD12864::API_LCD12864_line),
      InstanceMethod("bitmap", &WrapLCD12864::API_LCD12864_bitmap),

      InstanceMethod("test", &WrapLCD12864::API_LCD12864_test),
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  // LCD12864
  exports.Set("LCD12864", functions);
}

//--------------------------------------------------------------------------------------------------

WrapLCD12864::WrapLCD12864(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapLCD12864>(info)
{
  std::cout << "WrapLCD12864::WrapLCD12864 " << this << std::endl;
}

Napi::Value WrapLCD12864::API_LCD12864_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(mcp23017)").ThrowAsJavaScriptException();

  auto mcp23017Obj = info[0].As<Napi::Object>();
  WrapMCP23017 *wrap_mcp23017 = Napi::ObjectWrap<WrapMCP23017>::Unwrap(mcp23017Obj);
  std::cout << "!!! init lcd12864 with mcp23017 " << wrap_mcp23017 << std::endl;

  if (!wrap_mcp23017->mcp23017().initialized())
    Napi::Error::New(env, "lcd12864: mcp23017 not initialized").ThrowAsJavaScriptException();

  if (!this->lcd12864().init(&wrap_mcp23017->mcp23017()))
    Napi::Error::New(env, "lcd12864 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd12864().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd12864().clear();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_home(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd12864().home();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_display(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "display " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd12864().display(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_cursor(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "cursor " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd12864().cursor(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_blink(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "blink " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd12864().blink(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_move(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 argument(row, col)").ThrowAsJavaScriptException();
  }

  auto row = info[0].As<Napi::Number>();
  auto col = info[1].As<Napi::Number>();
  this->lcd12864().move(row.Int32Value() & 0xff, col.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_puts(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::String>();
  std::string value = std::string(data);
  this->lcd12864().puts(value.c_str());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_putc(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto ch = info[0].As<Napi::Number>();
  this->lcd12864().putch(ch.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_graphic(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  // std::cout << "display " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd12864().graphic(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_line(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 5)
  {
    Napi::Error::New(env, "Requre 5 argument(x0, y0, x1, y1, c)").ThrowAsJavaScriptException();
  }

  auto nx0 = info[0].As<Napi::Number>();
  auto ny0 = info[1].As<Napi::Number>();
  auto nx1 = info[2].As<Napi::Number>();
  auto ny1 = info[3].As<Napi::Number>();
  auto ncc = info[4].As<Napi::Number>();
  int16_t x0 = int16_t(nx0.Int32Value());
  int16_t y0 = int16_t(ny0.Int32Value());
  int16_t x1 = int16_t(nx1.Int32Value());
  int16_t y1 = int16_t(ny1.Int32Value());
  uint16_t cc = uint16_t(ncc.Int32Value());

  this->lcd12864().draw_line(x0, y0, x1, y1, cc);
  this->lcd12864().flush_display();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCD12864::API_LCD12864_bitmap(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 5)
  {
    Napi::Error::New(env, "Requre 5 argument(x, y, w, h, bmp)").ThrowAsJavaScriptException();
  }

  auto ix = info[0].As<Napi::Number>();
  auto iy = info[1].As<Napi::Number>();
  auto iw = info[2].As<Napi::Number>();
  auto ih = info[3].As<Napi::Number>();
  int16_t x = int16_t(ix.Int32Value());
  int16_t y = int16_t(iy.Int32Value());
  int16_t w = int16_t(iw.Int32Value());
  int16_t h = int16_t(ih.Int32Value());

  if (info[4].IsTypedArray())
  {
    auto typeddata = info[4].As<Napi::TypedArray>();
    auto data = typeddata.ArrayBuffer();
    auto buffer = reinterpret_cast<uint8_t *>(data.Data());
    this->lcd12864().bitmap(x, y, w, h, buffer, data.ByteLength());
  }
  else
  {
    Napi::Error::New(env, "Failed to access bitmap").ThrowAsJavaScriptException();
  }
  return Napi::Number::New(env, 0);
}


Napi::Value WrapLCD12864::API_LCD12864_test(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd12864().test();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
