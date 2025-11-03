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

#include "WrapSPI.h"
#include "WrapGPIO.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapSPI::Init(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function funcSPI = DefineClass(env, "SPI",
    {
      InstanceMethod("init", &WrapSPI::API_SPI_init),
      InstanceMethod("release", &WrapSPI::API_SPI_release),
      InstanceMethod("write", &WrapSPI::API_SPI_write),
      InstanceMethod("read", &WrapSPI::API_SPI_read)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(funcSPI);
  env.SetInstanceData(constructor);

  // SPI
  exports.Set("SPI", funcSPI);
}

//--------------------------------------------------------------------------------------------------

WrapSPI::WrapSPI(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapSPI>(info)
{
  std::cout << "WrapSPI::WrapSPI " << this << std::endl;
}

Napi::Value WrapSPI::API_SPI_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(gpio, dev, speed)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  WrapGPIO *wrap_gpio = Napi::ObjectWrap<WrapGPIO>::Unwrap(gpioObj);
  std::cout << "!!! init spi with gpio " << wrap_gpio << std::endl;

  if (!wrap_gpio->gpio().initialized())
    Napi::Error::New(env, "gpio not initialized").ThrowAsJavaScriptException();

  auto dev = info[1].As<Napi::Number>();
  auto speed = info[2].As<Napi::Number>();

  if (!this->spi().init(&wrap_gpio->gpio(), dev.Int32Value(), speed.Int32Value()))
    Napi::Error::New(env, "spi init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapSPI::API_SPI_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->spi().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapSPI::API_SPI_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  if (not info[0].IsArrayBuffer())
  {
    Napi::Error::New(env, "Argument shoud be Buffer").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::ArrayBuffer>();
  auto buffer = reinterpret_cast<uint8_t *>(data.Data());
  if (!this->spi().write_buffer(buffer, data.ByteLength()))
  {
    Napi::Error::New(env, "Failed to write SPI").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

Napi::Value WrapSPI::API_SPI_read(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  if (not info[0].IsArrayBuffer())
  {
    Napi::Error::New(env, "Argument shoud be Buffer").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::ArrayBuffer>();
  auto buffer = reinterpret_cast<uint8_t *>(data.Data());
  if (!this->spi().read_buffer(buffer, data.ByteLength()))
  {
    Napi::Error::New(env, "Failed to read SPI").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

} // namespace rpigpio2
