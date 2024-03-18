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

#include "gpiochip.h"

// NOTE nodejs fails to find __start_gpiochips symbol when loading .node file
//      with process.dlopen() from 'gpiochips' section
//      __gpio_chips array is to make this possible in traditional simple way
// TODO revise this with chip registry + self regitered chip

#define EXTERN_GPIO_CHIP(name) \
  extern rpigpio2::GPIO_CHIP_T name##_chip

EXTERN_GPIO_CHIP(brcmstb);
EXTERN_GPIO_CHIP(bcm2712);
EXTERN_GPIO_CHIP(bcm2712_aon);
EXTERN_GPIO_CHIP(bcm2712c0);
EXTERN_GPIO_CHIP(bcm2712c0_aon);
EXTERN_GPIO_CHIP(bcm2712d0);
EXTERN_GPIO_CHIP(bcm2712d0_aon);
EXTERN_GPIO_CHIP(bcm2835);
EXTERN_GPIO_CHIP(bcm2711);
EXTERN_GPIO_CHIP(rp1);

namespace rpigpio2
{

const GPIO_CHIP_T __gpio_chips[] = {
  brcmstb_chip,
  bcm2712_chip,
  bcm2712_aon_chip,
  bcm2712c0_chip,
  bcm2712c0_aon_chip,
  bcm2712d0_chip,
  bcm2712d0_aon_chip,
  bcm2835_chip,
  bcm2711_chip,
  rp1_chip
};

} // namespace rpigpio2
