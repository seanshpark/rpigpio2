{
  "targets": [
    {
      "target_name": "rpigiop2_i2c",
      "type": "static_library",

      "sources": [
        "i2c/src/i2c.cpp",
      ],
      "include_dirs": [
        "i2c/include",
      ],
      "dependencies": [
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "i2c/include",
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
