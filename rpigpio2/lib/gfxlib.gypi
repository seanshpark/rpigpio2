{
  "targets": [
    {
      "target_name": "rpigiop2_gfxlib",
      "type": "static_library",

      "sources": [
        "gfxlib/src/gfxlib.cpp",
      ],
      "include_dirs": [
        "gfxlib/include"
      ],
      "dependencies": [
      ],

      "direct_dependent_settings": {
        "include_dirs": [
          "gfxlib/include"
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
