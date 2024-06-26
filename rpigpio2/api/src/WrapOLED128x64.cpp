/*
 * Copyright 2022 saehie.park@gmail.com
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

#include "WrapOLED128x64.h"
#include "WrapI2C.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapOLED128x64::Init(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function functions = DefineClass(env, "OLED128x64",
    {
      InstanceMethod("init", &WrapOLED128x64::API_OLED128x64_init),
      InstanceMethod("release", &WrapOLED128x64::API_OLED128x64_release),

      InstanceMethod("cmd", &WrapOLED128x64::API_OLED128x64_cmd),
      InstanceMethod("data", &WrapOLED128x64::API_OLED128x64_data),

      InstanceMethod("addrMode", &WrapOLED128x64::API_OLED128x64_addrMode),
      InstanceMethod("colAddr", &WrapOLED128x64::API_OLED128x64_colAddr),
      InstanceMethod("pageAddr", &WrapOLED128x64::API_OLED128x64_pageAddr),

      InstanceMethod("clear", &WrapOLED128x64::API_OLED128x64_clear),

      InstanceMethod("line", &WrapOLED128x64::API_OLED128x64_line),
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  // OLED128x64
  exports.Set("OLED128x64", functions);
}

//--------------------------------------------------------------------------------------------------

WrapOLED128x64::WrapOLED128x64(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapOLED128x64>(info)
{
  std::cout << "WrapOLED128x64::WrapOLED128x64 " << this << std::endl;
}

Napi::Value WrapOLED128x64::API_OLED128x64_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(i2c)").ThrowAsJavaScriptException();

  auto i2cObj = info[0].As<Napi::Object>();
  WrapI2C *wrap_i2c = Napi::ObjectWrap<WrapI2C>::Unwrap(i2cObj);
  std::cout << "!!! init lcd1602 with i2c " << wrap_i2c << std::endl;

  if (!wrap_i2c->i2c().initialized())
    Napi::Error::New(env, "oled128x64: i2c not initialized").ThrowAsJavaScriptException();

  if (!this->oled128x64().init(&wrap_i2c->i2c()))
    Napi::Error::New(env, "oled128x64 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->oled128x64().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_cmd(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(cmd)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  if (!this->oled128x64().cmd(data.Int32Value() & 0xff))
  {
    Napi::Error::New(env, "Failed to OLED128x64 send commands").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_data(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  // if (info.Length() != 1)
  //{
  //  Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  //}

  if (info[0].IsArrayBuffer())
  {
    if (info.Length() != 1)
    {
      Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
    }

    auto data = info[0].As<Napi::ArrayBuffer>();
    auto buffer = reinterpret_cast<uint8_t *>(data.Data());
    // std::cout << "oled128x64 data array buffer " << data.ByteLength() << std::endl;
    if (!this->oled128x64().data(buffer, data.ByteLength()))
    {
      Napi::Error::New(env, "Failed to OLED128x64 send data").ThrowAsJavaScriptException();
    }
  }
  else if (info[0].IsArray())
  {
    // auto data = info[0].As<Napi::Array>();
    // std::cout << "NYI oled128x64 data array " << data.Length() << std::endl;
    // TODO call data
  }
  else if (info[0].IsTypedArray())
  {
    if (info.Length() != 1 && info.Length() != 3)
    {
      Napi::Error::New(env, "Requre 1 or 3 arguments").ThrowAsJavaScriptException();
    }
    auto typeddata = info[0].As<Napi::TypedArray>();
    auto data = typeddata.ArrayBuffer();
    auto buffer = reinterpret_cast<uint8_t *>(data.Data());

    if (info.Length() == 1)
    {
      // std::cout << "oled128x64 data typed array " << data.ByteLength() << std::endl;
      if (!this->oled128x64().data(buffer, data.ByteLength()))
      {
        Napi::Error::New(env, "Failed to OLED128x64 send data").ThrowAsJavaScriptException();
      }
    }
    else if (info.Length() == 3)
    {
      auto start = info[1].As<Napi::Number>();
      auto length = info[2].As<Napi::Number>();

      std::cout << "oled128x64 data typed array " << data.ByteLength() << ": "
                << start.Uint32Value() << ", " << length.Uint32Value() << std::endl;
      if (!this->oled128x64().data(buffer, start.Uint32Value(), length.Uint32Value()))
      {
        Napi::Error::New(env, "Failed to OLED128x64 send data").ThrowAsJavaScriptException();
      }
    }
  }
  else if (info[0].IsNumber())
  {
    auto data = info[0].As<Napi::Number>();
    // std::cout << "oled128x64 data " << data.Int32Value() << std::endl;
    if (!this->oled128x64().data(data.Int32Value() & 0xff))
    {
      Napi::Error::New(env, "Failed to OLED128x64 send data").ThrowAsJavaScriptException();
    }
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_addrMode(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(mode)").ThrowAsJavaScriptException();
  }

  auto mode = info[0].As<Napi::Number>();
  // mode is one of 0:Horz, 1:Vert, 2:Page(default)
  if (!this->oled128x64().addrMode(mode.Int32Value() & 0xff))
  {
    Napi::Error::New(env, "Failed to OLED128x64 address mode").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_colAddr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 argument(start, end)").ThrowAsJavaScriptException();
  }

  auto colStart = info[0].As<Napi::Number>();
  auto colEnd = info[1].As<Napi::Number>();
  // 0 ~ 127
  if (!this->oled128x64().colAddr(colStart.Int32Value() & 0xff, colEnd.Int32Value() & 0xff))
  {
    Napi::Error::New(env, "Failed to OLED128x64 column address").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_pageAddr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 argument(start, end)").ThrowAsJavaScriptException();
  }

  auto pageStart = info[0].As<Napi::Number>();
  auto pageEnd = info[1].As<Napi::Number>();
  // 0 ~ 7
  if (!this->oled128x64().pageAddr(pageStart.Int32Value() & 0xff, pageEnd.Int32Value() & 0xff))
  {
    Napi::Error::New(env, "Failed to OLED128x64 page address").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->oled128x64().clear();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapOLED128x64::API_OLED128x64_line(const Napi::CallbackInfo &info)
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

  this->oled128x64().draw_line(x0, y0, x1, y1, cc);
  this->oled128x64().flush_display();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
