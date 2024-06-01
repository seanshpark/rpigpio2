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

#include "WrapMCP23017.h"
#include "WrapI2C.h"

#include <cassert>
#include <iostream>

namespace rpigpio2
{

void WrapMCP23017::Init(Napi::Env &env, Napi::Object &exports)
{
  // MCP23017 chip
  // clang-format off
  Napi::Function functions = DefineClass(env, "MCP23017",
    {
      InstanceMethod("init", &WrapMCP23017::API_MCP23017_init),
      InstanceMethod("release", &WrapMCP23017::API_MCP23017_release),
      InstanceMethod("modeA", &WrapMCP23017::API_MCP23017_modeA),
      InstanceMethod("modeB", &WrapMCP23017::API_MCP23017_modeB),
      InstanceMethod("write", &WrapMCP23017::API_MCP23017_write),
      InstanceMethod("writeA", &WrapMCP23017::API_MCP23017_writeA),
      InstanceMethod("writeB", &WrapMCP23017::API_MCP23017_writeB),
      InstanceMethod("readA", &WrapMCP23017::API_MCP23017_readA),
      InstanceMethod("readB", &WrapMCP23017::API_MCP23017_readB),
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  exports.Set("MCP23017", functions);
}

//--------------------------------------------------------------------------------------------------

WrapMCP23017::WrapMCP23017(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WrapMCP23017>(info)
{
  std::cout << "WrapMCP23017::WrapMCP23017 " << this << std::endl;
}

Napi::Value WrapMCP23017::API_MCP23017_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(i2c)").ThrowAsJavaScriptException();

  auto i2cObj = info[0].As<Napi::Object>();
  WrapI2C *wrap_i2c = Napi::ObjectWrap<WrapI2C>::Unwrap(i2cObj);
  std::cout << "!!! init mcp23017 with i2c " << wrap_i2c << std::endl;

  if (!wrap_i2c->i2c().initialized())
    Napi::Error::New(env, "mcp23017: i2c not initialized").ThrowAsJavaScriptException();

  if (!this->mcp23017().init(&wrap_i2c->i2c()))
    Napi::Error::New(env, "mcp23017 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->mcp23017().release();

  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_modeA(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  auto data = info[0].As<Napi::Number>();
  this->mcp23017().modeA(data.Int32Value() & 0xff);
  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_modeB(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  auto data = info[0].As<Napi::Number>();
  this->mcp23017().modeB(data.Int32Value() & 0xff);
  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  auto data = info[0].As<Napi::Number>();
  this->mcp23017().write(data.Int32Value() & 0xffff);
  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_writeA(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  auto data = info[0].As<Napi::Number>();
  this->mcp23017().writeA(data.Int32Value() & 0xff);
  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_writeB(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  auto data = info[0].As<Napi::Number>();
  this->mcp23017().writeB(data.Int32Value() & 0xff);
  return Napi::Number::New(env, 0);
}

Napi::Value WrapMCP23017::API_MCP23017_readA(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  auto val = this->mcp23017().readA();
  return Napi::Number::New(env, val);
}

Napi::Value WrapMCP23017::API_MCP23017_readB(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  auto val = this->mcp23017().readB();
  return Napi::Number::New(env, val);
}

} // namespace rpigpio2
