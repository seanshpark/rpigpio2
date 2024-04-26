
#include "gpiolib.h"

#include <cstdint>
#include <iostream>

#include <string.h>
#include <unistd.h>

using namespace rpigpio;

class RpiGpio2
{
public:
  bool init(void);
  int do_gpio_set(unsigned int gpio, GPIO_DRIVE_T drive);

protected:
  GpioLib _lib;

  uint32_t _num_gpios = 0;
  GPIO_FSEL_T _fsparam = GPIO_FSEL_MAX;
  GPIO_DRIVE_T _drive = DRIVE_MAX;
  GPIO_PULL_T _pull = PULL_MAX;
};

bool RpiGpio2::init(void)
{
  // init gpio
  int ret = _lib.init();
  if (ret < 0)
  {
    std::cerr << "Failed to initialise gpiolib - " << ret << std::endl;
    return false;
  }
  _num_gpios = ret;
  if (!_num_gpios)
  {
    std::cerr << "No GPIO chips found" << std::endl;
    return false;
  }
  // std::cerr << "Num GPIOs: " << _num_gpios << std::endl;

  _fsparam = GPIO_FSEL_OUTPUT;
  _drive = DRIVE_HIGH;

  ret = _lib.mmap();
  if (ret)
  {
    if (ret == EACCES && geteuid())
      std::cerr << "Must be root" << std::endl;
    else
      std::cerr << "Failed to mmap gpiolib - " << strerror(ret) << std::endl;
    return false;
  }

  unsigned start_pin = GPIO_INVALID, end_pin;
  _lib.get_pin_range(&start_pin, &end_pin);
  std::cerr << "start: " << start_pin << ", end_pin: " << end_pin << std::endl;

  return true;
}

int RpiGpio2::do_gpio_set(unsigned int gpio, GPIO_DRIVE_T drive)
{
  unsigned int num = gpio;

  if (!_lib.is_valid(gpio))
  {
    std::cerr << "Pin " << gpio << " invalid" << std::endl;
    return 1;
  }

  _lib.set_fsel(gpio, _fsparam);

  if (drive != DRIVE_MAX)
  {
    if (_fsparam == GPIO_FSEL_OUTPUT)
    {
      _lib.set_drive(gpio, drive);
    }
    else
    {
      std::cerr << "Can't set pin value, not an output" << std::endl;
      return 1;
    }
  }

  if (_pull != PULL_MAX)
    _lib.set_pull(gpio, _pull);

  return 0;
}

int main(void)
{
  unsigned pin;

  RpiGpio2 rg2;

  if (!rg2.init())
    return -1;

  // init pin 23, 24
  unsigned pin1 = 23;
  unsigned pin2 = 24;

  // set as output

  for (int32_t loop = 0; loop < 3; ++loop)
  {
    // set as high
    rg2.do_gpio_set(pin1, DRIVE_HIGH);
    rg2.do_gpio_set(pin2, DRIVE_HIGH);
    usleep(1000*300);
    // set as low
    rg2.do_gpio_set(pin1, DRIVE_LOW);
    rg2.do_gpio_set(pin2, DRIVE_LOW);
    usleep(1000*300);
  }
  usleep(1000*300);

  for (int32_t loop = 0; loop < 10; ++loop)
  {
    rg2.do_gpio_set(pin1, DRIVE_HIGH);
    rg2.do_gpio_set(pin2, DRIVE_LOW);
    usleep(1000*100);

    rg2.do_gpio_set(pin1, DRIVE_LOW);
    rg2.do_gpio_set(pin2, DRIVE_HIGH);
    usleep(1000*100);
  }

  for (int32_t loop = 0; loop < 3; ++loop)
  {
    for (int32_t on = 100; on < 10000; on += 100)
    {
      rg2.do_gpio_set(pin1, DRIVE_HIGH);
      rg2.do_gpio_set(pin2, DRIVE_HIGH);
      usleep(on);

      rg2.do_gpio_set(pin1, DRIVE_LOW);
      rg2.do_gpio_set(pin2, DRIVE_LOW);
      usleep(10000-on);
    }
  }
  rg2.do_gpio_set(pin1, DRIVE_LOW);
  rg2.do_gpio_set(pin2, DRIVE_LOW);

  return 0;
}
