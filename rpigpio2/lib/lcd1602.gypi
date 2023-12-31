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
        "pcf8574/include",
        "i2c/include",
        "gpio/include",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
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
