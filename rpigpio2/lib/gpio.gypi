{
  "targets": [
    {
      "target_name": "rpigiop2_gpio",
      "type": "static_library",

      "sources": [
        "gpio/src/gpio2.cpp",
      ],
      "include_dirs": [
        "gpio/include"
      ],
      "dependencies": [
        "rpigiop2_gpiolib"
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "gpio/include",
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
