# EEL Utility

This is designed to be a simple command line utility to help developers generate EEL files.

## General Usage

Run `node eelutil.js --new --name="tmp102" --dir="/home/nick"` to generate a bare-bones directory containing an eel metadata.json file and empty directories for files and elements. 

Once you have this directory created and you've customized the metadata file and written all of your code, you can run `node eelutil.js --generate --dir="/home/nick/tmp102_EEL"` to generate the .eel file.

## Building Process
Building an eel happens in two steps

### 1.) Creating a source directory
Use the `--new` option to generate a new EEL source directory with a bare-bones metadata file. 

### 2.) Write your code
Create your driver, element, and ability code. 

### 3.) Generate an EEL
Use the `--generate` option to pacakge everything into a single EEL file that can be imported into Atmosphere Studio.

## metadata.json

This is intended to be a user editable file. Here's an example:
```js
{
	"libName": "irthermoclick",
	"manufacturer": "Mikroelektronika",
	"description": "MLX90614ESF-AAA Temperature Sensor mikroBUS™ Click™ Platform Evaluation Expansion Board",
	"type": "Temperature Sensor",
	"icon": "",
	"image": "",
	"version": "",
	"eelVersion": "3",
	"shoppingCartLinks":{
		"digikey":{
			"links":{
				"info":"https://www.digikey.com/product-detail/en/mikroelektronika/MIKROE-1362/1471-1076-ND/4495441"
			},
			
			"cartData":{
				"part":"1471-1076-ND",
				"partid":"4495441",
				"source":"dkstudio",
				"qty":"1"
			}
		}
	},
	"requires": [
		"embedded", "i2c"
	],
	"elements": [
		{
			"name": "IRThermoClick",
			"type": "EmbeddedIRThermoClick",
			"icon": "EmbeddedTempHumidity.svg",
			"defaultAbility": "readTemperature",
			"defaultTrigger": "temperatureRead",
			"hidden": false,
			"abilities": [
				{
					"name": "setup",
				  "hidden":true
				},
				{
					"name": "readTemperature",
					"triggers": ["temperatureRead"]
				}
			],
			"properties":[
				{
					"name":"i2cInstance",
					"input":"number",
					"value":"ATMO_DEFAULT_I2C"
				},
				
				{
					"name":"i2cAddress",
					"input":"number",
					"value":"0x5A"
				}
			],
			"triggers": [],
			"variables": [],
			"language": {
				"en-US": {
					"EmbeddedIRThermoClick": "IRThermo Click",
					"i2cInstance": "I2C Driver Instance",
					"i2cAddress": "I2C Address",
					"readTemperature": "Read Temperature",
					"temperatureRead": "Temperature Read",
					"setup": "Setup"
				}
			}
		}
	]
}
```

Any code or file objects should contain _paths_, not values. These paths will be read and the actual file contents will be inserted into the EEL when you run the utility with the `--generate` option. In this example, the `mysetup.c` file will be read and replaced with its contents.

For each ability, its code file will be assumed to be `[ability name].c` (eg. `readObjectTemperature.c`), unless another file is specified (see the `setup` ability). 

### EEL Fields
#### libName
This is the name of your EEL. It should not have spaces, and is typically all lowercase.

#### manufacturer
The manufacturer of your sensor. This is optional, but recommended.

#### description
A user friendly description of the sensor and what it does.

#### type
There are some standard sensor types that Atmosphere uses to group sensors together. 

## Directory Structure

The tool expects a specific directory structure. There should be a top level directory called `elements` with a subdirectory for each element.The name of this subdirectory should match the name of the element. Each element directory has a subdirectory called `abilities`. The `abilities` directory contains a source file for each ability. Here is the directory tree for the example metadata above:
```
.
├── elements
│   └── EmbeddedIRThermoClick
│       └── abilities
│           ├── readTemperature.c
│           └── setup.c
├── files
│   └── common
│       ├── headers
│       │   └── irthermoclick.h
│       └── objects
│           └── irthermoclick.c
├── metadata.json
└── README.md
```