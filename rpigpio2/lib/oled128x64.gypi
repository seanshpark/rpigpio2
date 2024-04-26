{
  "targets": [
    {
      "target_name": "rpigiop2_oled128x64",
      "type": "static_library",

      "sources": [
        "oled128x64/src/oled128x64.cpp",
      ],
      "include_dirs": [
        "oled128x64/include",
      ],
      "dependencies": [
        "rpigiop2_gpiolib",
        "rpigiop2_gpio",
        "rpigiop2_i2c",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "oled128x64/include",
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
