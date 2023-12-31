{
  "targets": [
    {
      "target_name": "rpigpio2",

      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],

      "dependencies": [
        "rpigiop2_gpio",
        "rpigiop2_i2c",
        "rpigiop2_pcf8574",
        "rpigiop2_lcd1602",
      ],
      "sources": [
        "src/Wrapper.cpp",
        "src/WrapGPIO.cpp",
        "src/WrapI2C.cpp",
        "src/WrapPCF8574.cpp",
        "src/WrapLCD1602.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/gpio/include",
        "../lib/i2c/include",
        "../lib/pcf8574/include",
        "../lib/lcd1602/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
