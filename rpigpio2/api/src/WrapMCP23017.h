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

#ifndef __RPIGPIO2_API_WRAP_MCP23017_H__
#define __RPIGPIO2_API_WRAP_MCP23017_H__

#include "WrapNapi.h"

#include "mcp23017/mcp23017.h"

namespace rpigpio2
{

class WrapMCP23017 : public Napi::ObjectWrap<WrapMCP23017>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  WrapMCP23017(const Napi::CallbackInfo &info);

public:
  Napi::Value API_MCP23017_init(const Napi::CallbackInfo &info);
  Napi::Value API_MCP23017_release(const Napi::CallbackInfo &info);
  Napi::Value API_MCP23017_write(const Napi::CallbackInfo &info);
public:
  MCP23017 &mcp23017() { return _mcp23017; }

protected:
  MCP23017 _mcp23017;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAP_MCP23017_H__
