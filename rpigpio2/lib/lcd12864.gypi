{
  "targets": [
    {
      "target_name": "rpigiop2_lcd12864",
      "type": "static_library",

      "sources": [
        "lcd12864/src/lcd12864.cpp",
      ],
      "include_dirs": [
        "lcd12864/include",
      ],
      "dependencies": [
        "rpigiop2_mcp23017",
        "rpigiop2_i2c",
        "rpigiop2_gpio",
        "rpigiop2_gpiolib",
        "rpigiop2_gfxlib",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "lcd12864/include",
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
