#ifndef GPIOLIB_H
#define GPIOLIB_H

#include "gpiotypes.h"
#include "gpiochip.h"

#include <stdint.h>

#define NUM_HDR_PINS    40
#define MAX_GPIO_PINS   300
#define MAX_GPIO_CHIPS  8

#define GPIO_INVALID    (~0U)
#define GPIO_GND        (~1U)
#define GPIO_5V         (~2U)
#define GPIO_3V3        (~3U)
#define GPIO_1V8        (~4U)
#define GPIO_OTHER      (~5U)

namespace rpigpio
{

typedef struct GPIO_CHIP_INSTANCE_
{
  const GPIO_CHIP_T *chip;
  const char *name;
  const char *dtnode;
  int mem_fd;
  void *priv;
  uint64_t phys_addr;
  unsigned num_gpios;
  uint32_t base;
} GPIO_CHIP_INSTANCE_T;

class GpioLib
{
public:
  int init(void);
  int init_by_name(const char *name);
  int mmap(void);
  void set_verbose(void (*callback)(const char *));

public:
  int is_valid(unsigned gpio);

  GPIO_DIR_T get_dir(unsigned gpio);
  void set_dir(unsigned gpio, GPIO_DIR_T dir);
  GPIO_FSEL_T get_fsel(unsigned gpio);

  void set_fsel(unsigned gpio, const GPIO_FSEL_T func);
  void set_drive(unsigned gpio, GPIO_DRIVE_T drv);
  void set(unsigned gpio);
  void clear(unsigned gpio);
  int get_level(unsigned gpio);  /* The actual level observed */ 
  GPIO_DRIVE_T get_drive(unsigned gpio);  /* What it is being driven as */
  GPIO_PULL_T get_pull(unsigned gpio);
  void set_pull(unsigned gpio, GPIO_PULL_T pull);

  void get_pin_range(unsigned *first, unsigned *last);
  unsigned for_pin(int pin);
  int to_pin(unsigned gpio);
  unsigned get_gpio_by_name(const char *name, int namelen);
  const char *get_name(unsigned gpio);
  const char *get_gpio_fsel_name(unsigned gpio, GPIO_FSEL_T fsel);
  const char *get_fsel_name(GPIO_FSEL_T fsel);
  const char *get_pull_name(GPIO_PULL_T pull);
  const char *get_drive_name(GPIO_DRIVE_T drive);

protected:
  GPIO_CHIP_INSTANCE_T *
  create_instance(const GPIO_CHIP_T *chip, uint64_t phys_addr, const char *name,
                  const char *dtnode);
  int get_interface(unsigned gpio, const GPIO_CHIP_INTERFACE_T **iface_ptr,
                    void **priv, unsigned *offset);
  int update_interface(unsigned gpio);

protected:
  const GPIO_CHIP_INTERFACE_T *_iface = nullptr;
  unsigned _if_offset = 0;
  void *_if_priv = nullptr;
  unsigned _num_gpio_chips = 0;
  GPIO_CHIP_INSTANCE_T _gpio_chips[MAX_GPIO_CHIPS] = { nullptr, };

protected:
  unsigned _num_gpios = 0;
  unsigned _first_hdr_pin = GPIO_INVALID;
  unsigned _last_hdr_pin = GPIO_INVALID;
  const char *_gpio_names[MAX_GPIO_PINS];
  unsigned _hdr_gpios[NUM_HDR_PINS + 1];
};

} // namespace rpigpio

#endif // GPIOLIB_H
