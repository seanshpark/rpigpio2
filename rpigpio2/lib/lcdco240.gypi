{
  "targets": [
    {
      "target_name": "rpigiop2_lcdco240",
      "type": "static_library",

      "sources": [
        "lcdco240/src/lcdco240.cpp",
      ],
      "include_dirs": [
        "lcdco240/include",
      ],
      "dependencies": [
        "rpigiop2_spi",
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
        "rpigiop2_gfxlib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "lcdco240/include",
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
