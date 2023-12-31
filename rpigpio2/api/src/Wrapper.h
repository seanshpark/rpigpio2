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

#ifndef __RPIGPIO2_API_WRAPPER_H__
#define __RPIGPIO2_API_WRAPPER_H__

#include "WrapNapi.h"

namespace rpigpio2
{

class Wrapper : public Napi::ObjectWrap<Wrapper>
{
public:
  static void Init(Napi::Env &env, Napi::Object &exports);

public:
  Wrapper(const Napi::CallbackInfo &info);

public:
  uint32_t id() { return _id; }

private:
  uint32_t _id = 0;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_API_WRAPPER_H__
