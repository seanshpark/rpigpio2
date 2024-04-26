{
  "targets": [
    {
      "target_name": "rpigiop2_led4x7seg",
      "type": "static_library",

      "sources": [
        "led4x7seg/src/led4x7seg.cpp",
      ],
      "include_dirs": [
        "led4x7seg/include",
        "tm1637/include",
        "gpio/include",
        "gpiolib/include",
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
