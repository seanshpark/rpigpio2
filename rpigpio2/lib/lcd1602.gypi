{
  "targets": [
    {
      "target_name": "rpigiop2_lcd1602",
      "type": "static_library",

      "sources": [
        "lcd1602/src/lcd1602.cpp",
      ],
      "include_dirs": [
        "lcd1602/include",
      ],
      "dependencies": [
        "rpigiop2_pcf8574",
        "rpigiop2_i2c",
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "lcd1602/include",
        ],
        "libraries": [
        ],
        "link_settings": {
          "library_dirs": [
          ]
        },
      },
    }
  ]
}
