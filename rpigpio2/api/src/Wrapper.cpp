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

#include "Wrapper.h"

#include "WrapGPIO.h"
#include "WrapI2C.h"
#include "WrapPCF8574.h"
#include "WrapTM1637.h"
#include "WrapMCP23017.h"
#include "WrapLCD1602.h"
#include "WrapLCD12864.h"
#include "WrapLED4x7Seg.h"
#include "WrapOLED128x64.h"
#include "WrapOLED128x32.h"

#include <cassert>
#include <iostream>

namespace
{

// Temporary unique ID for testing
volatile uint32_t __id = 0;

uint32_t __get_uid(void)
{
  __id++;
  return __id;
}

} // namespace

namespace rpigpio2
{

// Init is static
void Wrapper::Init(Napi::Env &env, Napi::Object &exports)
{
  std::cout << "Wrapper::Init" << std::endl;
  WrapGPIO::Init(env, exports);
  WrapI2C::Init(env, exports);
  WrapPCF8574::Init(env, exports);
  WrapTM1637::Init(env, exports);
  WrapMCP23017::Init(env, exports);
  WrapLCD1602::Init(env, exports);
  WrapLCD12864::Init(env, exports);
  WrapLED4x7Seg::Init(env, exports);
  WrapOLED128x64::Init(env, exports);
  WrapOLED128x32::Init(env, exports);
}

//--------------------------------------------------------------------------------------------------

Wrapper::Wrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Wrapper>(info)
{
  _id = __get_uid();
  std::cout << "Wrapper::Wrapper " << this << ": " << _id << std::endl;
}

} // namespace rpigpio2

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
  rpigpio2::Wrapper::Init(env, exports);
  return exports;
}

NODE_API_MODULE(rpigpio2, Initialize);
