{
  "targets": [
    {
      "target_name": "rpigiop2_tm1637",
      "type": "static_library",

      "sources": [
        "tm1637/src/tm1637.cpp",
      ],
      "include_dirs": [
        "tm1637/include",
      ],
      "dependencies": [
        "rpigiop2_gpiolib",
        "rpigiop2_gpio",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "tm1637/include",
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
