{
  "targets": [
    {
      "target_name": "rpigiop2_spi",
      "type": "static_library",

      "sources": [
        "spi/src/spi.cpp",
      ],
      "include_dirs": [
        "spi/include",
      ],
      "dependencies": [
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "spi/include",
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
