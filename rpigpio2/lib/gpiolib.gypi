{
  "targets": [
    {
      "target_name": "rpigiop2_gpiolib",
      "type": "static_library",

      "sources": [
        "gpiolib/src/gpiochip_bcm2712.cpp",
        "gpiolib/src/gpiochip_bcm2835.cpp",
        "gpiolib/src/gpiochip_rp1.cpp",
        "gpiolib/src/gpiochip.cpp",
        "gpiolib/src/util.cpp",
        "gpiolib/src/gpiolib.cpp",
      ],
      "include_dirs": [
        "gpiolib/include",
        "gpiolib/src",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "gpiolib/include",
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
