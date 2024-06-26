#ifndef __RPIGPIO2_LIB_GPIOLIB_GPIOTYPES_H__
#define __RPIGPIO2_LIB_GPIOLIB_GPIOTYPES_H__

namespace rpigpio2
{

typedef enum
{
  GPIO_FSEL_UNKNOWN = -1,
  GPIO_FSEL_FUNC0 = 0,
  GPIO_FSEL_FUNC1,
  GPIO_FSEL_FUNC2,
  GPIO_FSEL_FUNC3,
  GPIO_FSEL_FUNC4,
  GPIO_FSEL_FUNC5,
  GPIO_FSEL_FUNC6,
  GPIO_FSEL_FUNC7,
  GPIO_FSEL_FUNC8,
  /* ... */
  GPIO_FSEL_INPUT = 0x10,
  GPIO_FSEL_OUTPUT,
  GPIO_FSEL_GPIO, /* Preserves direction if possible, else input */
  GPIO_FSEL_NONE, /* If possible, else input */
  GPIO_FSEL_MAX
} GPIO_FSEL_T;

typedef enum
{
  PULL_NONE,
  PULL_DOWN,
  PULL_UP,
  PULL_MAX
} GPIO_PULL_T;

typedef enum
{
  DIR_INPUT,
  DIR_OUTPUT,
  DIR_MAX,
} GPIO_DIR_T;

typedef enum
{
  DRIVE_LOW,
  DRIVE_HIGH,
  DRIVE_MAX
} GPIO_DRIVE_T;

typedef struct GPIO_CHIP_INTERFACE_ GPIO_CHIP_INTERFACE_T;
typedef struct GPIO_CHIP_ GPIO_CHIP_T;

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_GPIOLIB_GPIOTYPES_H__
