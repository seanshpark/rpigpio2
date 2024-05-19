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

#include "mcp23017/mcp23017.h"

#include <iostream>
#include <bitset>
#include <cassert>

#define MCP23017_IODIRA   0x00  // A direction
#define MCP23017_IODIRB   0x01  // B direction

#define MCP23017_OLATA    0x14  // A output latch
#define MCP23017_OLATB    0x15  // B output latch

namespace rpigpio2
{

namespace
{

#pragma pack (1)
struct mcp23017Send
{
  uint8_t addr; // register address
  uint8_t data;
};
#pragma pack (0)

}

bool MCP23017::init(I2C *i2c)
{
  std::cout << "MCP23017::init" << std::endl;

  _i2c = i2c;

  send(MCP23017_IODIRA, 0x00); // port A to write mode
  send(MCP23017_IODIRB, 0x00); // port B to write mode
  //send(MCP23017_OLATA, 0x00); // port A to low
  //send(MCP23017_OLATB, 0x00); // port B to low

  _initalized = true;

  return true;
}

void MCP23017::release(void)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }
  //send(MCP23017_OLATA, 0x00); // port A to h
  //send(MCP23017_OLATB, 0x00); // port B to h

  _initalized = false;
  _i2c = nullptr;

  std::cout << "MCP23017::release" << std::endl;
}

// Low 8bit is for PortA, High 8bit is for PortB
void MCP23017::write(uint16_t data)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }
  uint8_t dataa = (uint8_t)(data & 0xff);
  uint8_t datab = (uint8_t)((data >> 8) & 0xff);
  send(MCP23017_OLATA, dataa);
  send(MCP23017_OLATB, datab);
}

void MCP23017::writeA(uint8_t data)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }
  send(MCP23017_OLATA, data);
}

void MCP23017::writeB(uint8_t data)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }
  send(MCP23017_OLATB, data);
}

} // namespace rpigpio2

//
// MCP23017 privates
//
namespace rpigpio2
{

void MCP23017::send(uint8_t addr, uint8_t data)
{
  mcp23017Send mcpsend;
  mcpsend.addr = addr;
  mcpsend.data = data;
  // std::cout << "Addr:" << (int)addr << " Data:" << (int)data << std::endl;
  _i2c->write_buffer((uint8_t *)&mcpsend, 2);
}

} // namespace rpigpio2
