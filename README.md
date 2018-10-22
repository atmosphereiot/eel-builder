# EEL Utility

This is designed to be a simple command line utility to help developers generate EEL files.

## General Usage

Run `node eelutil.js --new --name="tmp102" --dir="/home/nick"` to generate a bare-bones directory containing an eel metadata.json file and empty directories for files and elements. 

Once you have this directory created and you've customized the metadata file and written all of your code, you can run `node eelutil.js --generate --dir="/home/nick/tmp102_EEL"` to generate the .eel file.

## metadata.json

This is intended to be a user editable file. Here's an example:
```js
{
  "name": "tmp102",
  "description": "",
  "icon": "",
  "image": "",
  "version": "",
  "eelVersion": "3",
  "requires": [],
  "elements": [{
    "name": "TMP102",
    "icon":"TMP102.svg",
    "abilities": [
      {
        "name": "setup",
        "code": "mysetup.c"
      },
      {
        "name": "readObjectTemperature"
      },
      {
        "name": "readAmbientTemperature"
      }
    ]
  }],
}
```

Any code or file objects should contain _paths_, not values. These paths will be read and the actual file contents will be inserted into the EEL when you run the utility with the `--generate` option. In this example, the `mysetup.c` file will be read and replaced with its contents.

For each ability, its code file will be assumed to be `[ability name].c` (eg. `readObjectTemperature.c`), unless another file is specified (see the `setup` ability). 

## Directory Structure

The tool expects a specific directory structure. There should be a top level directory called `elements` with a subdirectory for each element.The name of this subdirectory should match the name of the element. Each element directory has a subdirectory called `abilities`. The `abilities` directory contains a source file for each ability. Here is the directory tree for the example metadata above:
```
.
├── elements
│   └── TMP102
│       └── abilities
│           ├── mysetup.c
│           ├── readAmbientTemperature.c
│           └── readObjectTemperature.c
├── files
│   ├── headers
│   │   └── test.h
│   ├── objects
│   └── other
└── metadata.json

```