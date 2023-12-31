{
  "targets": [
    {
      "target_name": "rpigiop2_gpio",
      "type": "static_library",

      "sources": [
        "gpio/src/gpio.cpp",
      ],
      "include_dirs": [
        "gpio/include"
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
