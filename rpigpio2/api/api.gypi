{
  "targets": [
    {
      "target_name": "rpigpio2",

      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],

      "dependencies": [
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
        "rpigiop2_gfxlib",
        "rpigiop2_i2c",
        "rpigiop2_lcd1602",
        "rpigiop2_led4x7seg",
        "rpigiop2_pcf8574",
        "rpigiop2_tm1637",
        "rpigiop2_oled128x64",
      ],
      "sources": [
        "src/Wrapper.cpp",
        "src/WrapGPIO.cpp",
        "src/WrapI2C.cpp",
        "src/WrapLCD1602.cpp",
        "src/WrapLED4x7Seg.cpp",
        "src/WrapPCF8574.cpp",
        "src/WrapTM1637.cpp",
        "src/WrapOLED128x64.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/gpio/include",
        "../lib/gpiolib/include",
        "../lib/gfxlib/include",
        "../lib/i2c/include",
        "../lib/lcd1602/include",
        "../lib/led4x7seg/include",
        "../lib/pcf8574/include",
        "../lib/tm1637/include",
        "../lib/oled128x64/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
