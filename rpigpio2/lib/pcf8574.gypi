{
  "targets": [
    {
      "target_name": "rpigiop2_pcf8574",
      "type": "static_library",

      "sources": [
        "pcf8574/src/pcf8574.cpp",
      ],
      "include_dirs": [
        "pcf8574/include",
      ],
      "dependencies": [
        "rpigiop2_gpiolib",
        "rpigiop2_gpio",
        "rpigiop2_i2c",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "pcf8574/include",
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
