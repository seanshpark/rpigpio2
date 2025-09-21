{
  "targets": [
    {
      "target_name": "rpigiop2_oled128x32",
      "type": "static_library",

      "sources": [
        "oled128x32/src/oled128x32.cpp",
      ],
      "include_dirs": [
        "oled128x32/include",
      ],
      "dependencies": [
        "rpigiop2_gpiolib",
        "rpigiop2_gpio",
        "rpigiop2_i2c",
        "rpigiop2_gfxlib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "oled128x32/include",
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
