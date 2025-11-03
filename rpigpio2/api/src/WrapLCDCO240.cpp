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

#include "WrapLCDCO240.h"
#include "WrapSPI.h"
#include "WrapGPIO.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapLCDCO240::Init(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function functions = DefineClass(env, "LCDCO240",
    {
      InstanceMethod("init", &WrapLCDCO240::API_LCDCO240_init),
      InstanceMethod("release", &WrapLCDCO240::API_LCDCO240_release),

      InstanceMethod("clear", &WrapLCDCO240::API_LCDCO240_clear),
      InstanceMethod("display", &WrapLCDCO240::API_LCDCO240_display),
      InstanceMethod("line", &WrapLCDCO240::API_LCDCO240_line),
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  // LCD12864
  exports.Set("LCDCO240", functions);
}

//--------------------------------------------------------------------------------------------------

WrapLCDCO240::WrapLCDCO240(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapLCDCO240>(info)
{
  std::cout << "WrapLCDCO240::WrapLCDCO240 " << this << std::endl;
}

Napi::Value WrapLCDCO240::API_LCDCO240_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(spi, gpio, num)").ThrowAsJavaScriptException();

  auto spiObj = info[0].As<Napi::Object>();
  auto gpioObj = info[1].As<Napi::Object>();
  auto dcpin = info[2].As<Napi::Number>();

  WrapSPI *wrap_spi = Napi::ObjectWrap<WrapSPI>::Unwrap(spiObj);
  WrapGPIO *wrap_gpio = Napi::ObjectWrap<WrapGPIO>::Unwrap(gpioObj);
  std::cout << "!!! init lcdco240 with SPI " << wrap_spi
            << ", GPIO " << wrap_gpio
            << ", DCPIN " << std::dec << dcpin.Int32Value()
            << std::endl;

  if (!wrap_spi->spi().initialized())
    Napi::Error::New(env, "lcdco240: SPI not initialized").ThrowAsJavaScriptException();
  if (!wrap_gpio->gpio().initialized())
    Napi::Error::New(env, "lcdco240: GOUI not initialized").ThrowAsJavaScriptException();

  if (!this->lcdco240().init(&wrap_spi->spi(), &wrap_gpio->gpio(), dcpin.Int32Value()))
    Napi::Error::New(env, "lcdco240 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCDCO240::API_LCDCO240_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcdco240().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCDCO240::API_LCDCO240_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcdco240().clear_display();
  this->lcdco240().flush_display();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCDCO240::API_LCDCO240_display(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcdco240().flush_display();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapLCDCO240::API_LCDCO240_line(const Napi::CallbackInfo &info)
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

  this->lcdco240().draw_line(x0, y0, x1, y1, cc);
  this->lcdco240().flush_display();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
