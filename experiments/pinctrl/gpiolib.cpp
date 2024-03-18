#define _FILE_OFFSET_BITS 64

#include "gpiolib.h"
#include "gpiochip.h"
#include "util.h"

#include <cassert>

#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof(_a[0]))

namespace rpigpio
{

const char *__pull_names[] = {"pn", "pd", "pu", "--"};
const char *__drive_names[] = {"dl", "dh", "--"};
const char *__fsel_names[] = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "??",
                              "??", "??", "??", "??", "??", "??", "ip", "op", "gp", "no"};

void (*verbose_callback)(const char *);

GPIO_CHIP_INSTANCE_T *GpioLib::create_instance(const GPIO_CHIP_T *chip, uint64_t phys_addr,
                                               const char *name, const char *dtnode)
{
  if (_num_gpio_chips >= MAX_GPIO_CHIPS)
  {
    assert(false);
    return nullptr;
  }
  GPIO_CHIP_INSTANCE_T *inst = &_gpio_chips[_num_gpio_chips];

  inst->chip = chip;
  inst->name = name ? name : chip->name;
  inst->dtnode = dtnode;
  inst->phys_addr = phys_addr;
  inst->priv = NULL;
  inst->base = 0;

  printf("create_instance name: %s\r\n", inst->name);
  printf("create_instance dtnode: %s\r\n", inst->dtnode);

  inst->priv = chip->interface->gpio_create_instance(chip, dtnode);
  if (!inst->priv)
    return NULL;

  _num_gpio_chips++;
  printf("create_instance _num_gpio_chips: %d\r\n", _num_gpio_chips);

  return inst;
}

int GpioLib::get_interface(unsigned gpio, const GPIO_CHIP_INTERFACE_T **iface_ptr, void **priv,
                           unsigned *offset)
{
  assert(iface_ptr != nullptr);
  assert(priv != nullptr);

  *iface_ptr = NULL;
  for (unsigned i = 0; i < _num_gpio_chips; i++)
  {
    GPIO_CHIP_INSTANCE_T *inst = &_gpio_chips[i];
    const GPIO_CHIP_T *chip = inst->chip;
    if (gpio >= inst->base && gpio < (inst->base + inst->num_gpios))
    {
      *iface_ptr = chip->interface;
      *priv = inst->priv;
      *offset = gpio - inst->base;
      return 0;
    }
  }
  return -1;
}

int GpioLib::is_valid(unsigned gpio) { return gpio < MAX_GPIO_PINS && !!_gpio_names[gpio]; }

int GpioLib::update_interface(unsigned gpio)
{
  return get_interface(gpio, &_iface, &_if_priv, &_if_offset);
}

GPIO_DIR_T GpioLib::get_dir(unsigned gpio)
{
  if (update_interface(gpio) == 0)
    return _iface->gpio_get_dir(_if_priv, _if_offset);
  return DIR_MAX;
}

void GpioLib::set_dir(unsigned gpio, GPIO_DIR_T dir)
{
  if (update_interface(gpio) == 0)
    _iface->gpio_set_dir(_if_priv, _if_offset, dir);
}

GPIO_FSEL_T GpioLib::get_fsel(unsigned gpio)
{
  GPIO_FSEL_T fsel = GPIO_FSEL_MAX;

  if (update_interface(gpio) == 0)
    fsel = _iface->gpio_get_fsel(_if_priv, _if_offset);

  if (fsel == GPIO_FSEL_GPIO)
  {
    if (get_dir(gpio) == DIR_OUTPUT)
      fsel = GPIO_FSEL_OUTPUT;
    else
      fsel = GPIO_FSEL_INPUT;
  }

  return fsel;
}

void GpioLib::set_fsel(unsigned gpio, const GPIO_FSEL_T func)
{
  printf("!!! gpio_set_fsel: %d, %d\r\n", gpio, func);

  if (update_interface(gpio) == 0)
    _iface->gpio_set_fsel(_if_priv, _if_offset, func);
}

void GpioLib::set_drive(unsigned gpio, GPIO_DRIVE_T drv)
{
  if (update_interface(gpio) == 0)
    _iface->gpio_set_drive(_if_priv, _if_offset, drv);
}

void GpioLib::set(unsigned gpio)
{
  if (update_interface(gpio) == 0)
  {
    _iface->gpio_set_drive(_if_priv, _if_offset, DRIVE_HIGH);
    _iface->gpio_set_dir(_if_priv, _if_offset, DIR_OUTPUT);
  }
}

void GpioLib::clear(unsigned gpio)
{
  if (update_interface(gpio) == 0)
  {
    _iface->gpio_set_drive(_if_priv, _if_offset, DRIVE_LOW);
    _iface->gpio_set_dir(_if_priv, _if_offset, DIR_OUTPUT);
  }
}

// 0: lo
// 1: hi
int GpioLib::get_level(unsigned gpio)
{
  if (update_interface(gpio) == 0)
    return _iface->gpio_get_level(_if_priv, _if_offset);
  return 0;
}

GPIO_DRIVE_T GpioLib::get_drive(unsigned gpio)
{
  if (update_interface(gpio) == 0)
    return _iface->gpio_get_drive(_if_priv, _if_offset);
  return DRIVE_MAX;
}

GPIO_PULL_T GpioLib::get_pull(unsigned gpio)
{
  if (update_interface(gpio) == 0)
    return _iface->gpio_get_pull(_if_priv, _if_offset);
  return PULL_MAX;
}

void GpioLib::set_pull(unsigned gpio, GPIO_PULL_T pull)
{
  if (update_interface(gpio) == 0)
    _iface->gpio_set_pull(_if_priv, _if_offset, pull);
}

void GpioLib::get_pin_range(unsigned *first, unsigned *last)
{
  if (_first_hdr_pin == GPIO_INVALID)
  {
    unsigned i;

    for (i = 0; i < _num_gpio_chips; i++)
    {
      if (!strncmp(_gpio_chips[i].name, "bcm2", 4) || !strcmp(_gpio_chips[i].name, "rp1"))
      {
        // Assume it's the standard RPi 40-pin header layout
        uint32_t base = _gpio_chips[i].base;

        _hdr_gpios[3] = base + 2;
        _hdr_gpios[5] = base + 3;
        _hdr_gpios[7] = base + 4;
        _hdr_gpios[8] = base + 14;
        _hdr_gpios[10] = base + 15;
        _hdr_gpios[11] = base + 17;
        _hdr_gpios[12] = base + 18;
        _hdr_gpios[13] = base + 27;
        _hdr_gpios[15] = base + 22;
        _hdr_gpios[16] = base + 23;
        _hdr_gpios[18] = base + 24;
        _hdr_gpios[19] = base + 10;
        _hdr_gpios[21] = base + 9;
        _hdr_gpios[18] = base + 24;
        _hdr_gpios[22] = base + 25;
        _hdr_gpios[23] = base + 11;
        _hdr_gpios[24] = base + 8;
        _hdr_gpios[26] = base + 7;
        _hdr_gpios[27] = base + 0;
        _hdr_gpios[28] = base + 1;
        _hdr_gpios[29] = base + 5;
        _hdr_gpios[31] = base + 6;
        _hdr_gpios[32] = base + 12;
        _hdr_gpios[33] = base + 13;
        _hdr_gpios[35] = base + 19;
        _hdr_gpios[36] = base + 16;
        _hdr_gpios[37] = base + 26;
        _hdr_gpios[38] = base + 20;
        _hdr_gpios[40] = base + 21;

        _first_hdr_pin = 1;
        _last_hdr_pin = 40;
        break;
      }
    }
  }
  if (first)
    *first = _first_hdr_pin;
  if (last)
    *last = _last_hdr_pin;
}

unsigned GpioLib::for_pin(int pin)
{
  if (pin >= 1 && pin <= NUM_HDR_PINS)
    return _hdr_gpios[pin];
  return GPIO_INVALID;
}

int GpioLib::to_pin(unsigned gpio)
{
  int i;

  for (i = 1; i <= NUM_HDR_PINS; i++)
  {
    if (_hdr_gpios[i] == gpio)
      return i;
  }
  return -1;
}

unsigned GpioLib::get_gpio_by_name(const char *name, int name_len)
{
  unsigned gpio;

  if (!name_len)
    name_len = strlen(name);
  for (gpio = 0; gpio < _num_gpios; gpio++)
  {
    const char *gpio_name = _gpio_names[gpio];
    const char *p;

    if (!gpio_name)
      continue;

    for (p = gpio_name; *p;)
    {
      int len = strcspn(p, "/");
      if (len == name_len && memcmp(name, p, name_len) == 0)
        return gpio;
      p += len;
      if (*p == '/')
        p++;
    }
  }

  return GPIO_INVALID;
}

const char *GpioLib::get_name(unsigned gpio)
{
  if (gpio < _num_gpios)
    return _gpio_names[gpio];

  switch (gpio)
  {
    case GPIO_INVALID:
      return "-";
    case GPIO_GND:
      return "gnd";
    case GPIO_5V:
      return "5v";
    case GPIO_3V3:
      return "3v3";
    case GPIO_1V8:
      return "1v8";
    case GPIO_OTHER:
    default:
      return "???";
  }
}

const char *GpioLib::get_gpio_fsel_name(unsigned gpio, GPIO_FSEL_T fsel)
{
  if (update_interface(gpio) == 0)
    return _iface->gpio_get_fsel_name(_if_priv, _if_offset, fsel);
  return NULL;
}

const char *GpioLib::get_fsel_name(GPIO_FSEL_T fsel)
{
  if ((unsigned)fsel < ARRAY_SIZE(__fsel_names))
    return __fsel_names[fsel];
  return NULL;
}

const char *GpioLib::get_pull_name(GPIO_PULL_T pull)
{
  if ((unsigned)pull < ARRAY_SIZE(__pull_names))
    return __pull_names[pull];
  return NULL;
}

const char *GpioLib::get_drive_name(GPIO_DRIVE_T drive)
{
  if ((unsigned)drive < ARRAY_SIZE(__drive_names))
    return __drive_names[drive];
  return NULL;
}

namespace
{

const GPIO_CHIP_T *_find_chip(const char *name)
{
  const GPIO_CHIP_T *chip;

  printf("!!! gpio_find_chip: %s\r\n", name);
  for (chip = &__start_gpiochips; name && chip < &__stop_gpiochips; chip++)
  {
    printf("!!! chip-name: %s\r\n", chip->name);
    printf("!!! chip-compatible: %s\r\n", chip->compatible);
    if (!strcmp(name, chip->name) || !strcmp(name, chip->compatible))
      return chip;
  }

  return NULL;
}

} // namespace

int GpioLib::init(void)
{
  const GPIO_CHIP_T *chip;
  GPIO_CHIP_INSTANCE_T *inst;
  char pathbuf[FILENAME_MAX];
  char gpiomem_idx[4];
  const char *dtpath = "/proc/device-tree";
  const char *p;
  char *alias = NULL, *names, *end, *compatible;
  uint64_t phys_addr;
  size_t names_len;
  unsigned gpio_base;
  unsigned pin, i;

  for (pin = 0; pin <= NUM_HDR_PINS; pin++)
    _hdr_gpios[pin] = GPIO_INVALID;

  // There is currently only one header layout
  _hdr_gpios[1] = GPIO_3V3;
  _hdr_gpios[17] = GPIO_3V3;
  _hdr_gpios[2] = GPIO_5V;
  _hdr_gpios[4] = GPIO_5V;
  _hdr_gpios[6] = GPIO_GND;
  _hdr_gpios[9] = GPIO_GND;
  _hdr_gpios[14] = GPIO_GND;
  _hdr_gpios[20] = GPIO_GND;
  _hdr_gpios[25] = GPIO_GND;
  _hdr_gpios[30] = GPIO_GND;
  _hdr_gpios[34] = GPIO_GND;
  _hdr_gpios[39] = GPIO_GND;

  if (verbose_callback)
    (*verbose_callback)("GPIO chips:\n");

  dt_set_path(dtpath);

  for (i = 0;; i++)
  {
    sprintf(pathbuf, "gpio%d", i);
    sprintf(gpiomem_idx, "%d", i);
    alias = dt_read_prop("/aliases", pathbuf, NULL);
    if (!alias && i == 0)
    {
      alias = dt_read_prop("/aliases", "gpio", NULL);
      gpiomem_idx[0] = 0;
    }
    if (!alias)
      break;

    compatible = dt_read_prop(alias, "compatible", NULL);
    if (!compatible)
    {
      sprintf(pathbuf, "%s/..", alias);
      compatible = dt_read_prop(pathbuf, "compatible", NULL);
    }

    chip = _find_chip(compatible);
    dt_free(compatible);

    if (!chip)
    {
      // Skip the unknown gpio chip
      dt_free(alias);
      continue;
    }

    phys_addr = dt_parse_addr(alias);
    if (phys_addr == INVALID_ADDRESS)
    {
      dt_free(alias);
      return -1;
    }

    inst = create_instance(chip, phys_addr, NULL, alias);
    if (!inst)
    {
      dt_free(alias);
      return -1;
    }

    sprintf(pathbuf, "/dev/gpiomem%s", gpiomem_idx);
    inst->mem_fd = open(pathbuf, O_RDWR | O_SYNC);
  }

  gpio_base = 0;
  _num_gpios = 0;

  printf("gpiolib_init num_gpio_chips: %d\r\n", _num_gpio_chips);

  for (i = 0; i < _num_gpio_chips; i++)
  {
    unsigned gpio;

    inst = &_gpio_chips[i];
    inst->base = gpio_base;
    chip = inst->chip;
    inst->num_gpios = chip->interface->gpio_count(inst->priv);

    if (verbose_callback)
    {
      char msg_buf[100];
      snprintf(msg_buf, sizeof(msg_buf), "  %" PRIx64 ": %s (%d gpios)\n", inst->phys_addr,
               chip->name, inst->num_gpios);
      (*verbose_callback)(msg_buf);
    }

    if (!inst->num_gpios)
      continue;
    _num_gpios = gpio_base + inst->num_gpios;
    gpio_base = ROUND_UP(_num_gpios, 100);

    if (_num_gpios > MAX_GPIO_PINS)
      return -1;

    names = dt_read_prop(inst->dtnode, "gpio-line-names", &names_len);
    end = names + names_len;

    for (gpio = 0, p = names; gpio < inst->num_gpios; gpio++)
    {
      static const char *names[] = {"ID_SD", "ID_SC"};
      static const int pins[] = {27, 28};
      char name_buf[32];
      const char *name = "-", *arch_name;

      if (p && p < end)
      {
        name = p;
        p += strlen(p) + 1;
        if (sscanf(name, "PIN%u", &pin) != 1 || pin < 1 || pin > NUM_HDR_PINS)
        {
          unsigned i;
          pin = 0;
          for (i = 0; i < ARRAY_SIZE(names); i++)
          {
            if (strcmp(name, names[i]) == 0)
            {
              pin = pins[i];
              break;
            }
          }
        }

        if (pin >= 1)
        {
          _hdr_gpios[pin] = inst->base + gpio;
          if ((pin < _first_hdr_pin) || (_first_hdr_pin == GPIO_INVALID))
            _first_hdr_pin = pin;
          if ((pin > _last_hdr_pin) || (_last_hdr_pin == GPIO_INVALID))
            _last_hdr_pin = pin;
        }
      }

      arch_name = chip->interface->gpio_get_name(inst->priv, gpio);
      if (!name[0] || !arch_name)
      {
        _gpio_names[inst->base + gpio] = NULL;
        continue;
      }

      if (strcmp(name, arch_name) != 0)
      {
        if (strcmp(name, "-") == 0)
        {
          name = arch_name;
        }
        else
        {
          if (snprintf(name_buf, sizeof(name_buf), "%s/%s", name, arch_name) >=
              (int)sizeof(name_buf))
            name_buf[sizeof(name_buf) - 1] = '\0';
          name = name_buf;
        }
      }

      _gpio_names[inst->base + gpio] = strdup(name);
    }

    dt_free(names);
  }

  // On a board with PINs, show pins 1-40
  if (_first_hdr_pin == 3)
    _first_hdr_pin = 1;

  return (int)_num_gpios;
}

int GpioLib::init_by_name(const char *name)
{
  const GPIO_CHIP_T *chip;
  GPIO_CHIP_INSTANCE_T *inst;
  unsigned gpio;
  int pin;

  for (pin = 0; pin <= NUM_HDR_PINS; pin++)
    _hdr_gpios[pin] = GPIO_INVALID;

  if (verbose_callback)
    (*verbose_callback)("GPIO chips:\n");

  chip = _find_chip(name);
  if (!chip)
    return 0;

  inst = create_instance(chip, 0, NULL, NULL);
  if (!inst)
    return -1;

  inst->num_gpios = chip->interface->gpio_count(inst->priv);

  _num_gpios = inst->num_gpios;

  for (gpio = 0; gpio < inst->num_gpios; gpio++)
  {
    const char *name = chip->interface->gpio_get_name(inst->priv, gpio);
    if (!name)
    {
      _gpio_names[inst->base + gpio] = NULL;
      continue;
    }

    _gpio_names[gpio] = strdup(name);
  }

  if (inst->num_gpios && verbose_callback)
  {
    char msg_buf[100];
    snprintf(msg_buf, sizeof(msg_buf), "  %s (%d gpios)\n", chip->name, inst->num_gpios);
    (*verbose_callback)(msg_buf);
  }

  return (int)_num_gpios;
}

int GpioLib::mmap(void)
{
  int pagesize = getpagesize();
  int mem_fd = -1;
  unsigned i;

  for (i = 0; i < _num_gpio_chips; i++)
  {
    GPIO_CHIP_INSTANCE_T *inst;
    const GPIO_CHIP_T *chip;
    void *gpio_map;
    void *new_priv;
    unsigned align;

    inst = &_gpio_chips[i];
    chip = inst->chip;

    align = inst->phys_addr & (pagesize - 1);

    if (inst->mem_fd >= 0)
    {
      gpio_map = ::mmap(NULL,                   /* Any address in our space will do */
                        chip->size + align,     /* Map length */
                        PROT_READ | PROT_WRITE, /* Enable reading & writing */
                        MAP_SHARED,             /* Shared with other processes */
                        inst->mem_fd,           /* File to map */
                        0                       /* Offset to GPIO peripheral */
      );
    }
    else
    {
      if (mem_fd < 0)
      {
        mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (mem_fd < 0)
          return errno;
      }
      gpio_map = ::mmap(NULL,                   /* Any address in our space will do */
                        chip->size + align,     /* Map length */
                        PROT_READ | PROT_WRITE, /* Enable reading & writing */
                        MAP_SHARED,             /* Shared with other processes */
                        mem_fd,                 /* File to map */
                        inst->phys_addr - align /* Offset to GPIO peripheral */
      );
    }

    if (gpio_map == MAP_FAILED)
      return errno;

    volatile uint32_t *base = (volatile uint32_t *)((char *)gpio_map + align);
    new_priv = chip->interface->gpio_probe_instance(inst->priv, base);
    //(volatile uint32_t* *)((char *)gpio_map + align));
    if (!new_priv)
      return -1;

    inst->priv = new_priv;
  }

  return 0;
}

void GpioLib::set_verbose(void (*callback)(const char *))
{
  //
  verbose_callback = callback;
}

} // namespace rpigpio
