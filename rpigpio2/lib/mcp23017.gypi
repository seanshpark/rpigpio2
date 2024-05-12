{
  "targets": [
    {
      "target_name": "rpigiop2_mcp23017",
      "type": "static_library",

      "sources": [
        "mcp23017/src/mcp23017.cpp",
      ],
      "include_dirs": [
        "mcp23017/include",
      ],
      "dependencies": [
        "rpigiop2_gpiolib",
        "rpigiop2_gpio",
        "rpigiop2_i2c",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "mcp23017/include",
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
