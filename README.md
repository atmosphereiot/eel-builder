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

### EEL Top Level Fields
#### libName
This is the name of your EEL. It should not have spaces, and is typically all lowercase.

#### manufacturer
The manufacturer of your sensor. This is optional, but recommended.

#### description
A user friendly description of the sensor and what it does.

#### type
There are some standard sensor types that Atmosphere uses to group sensors together. TODO: Options?

#### icon
A photo of the sensor, can be left blank.

#### image
A manufacturer image, can be left blank. 

#### version
An EEL version field. This is not used within the tool at all, so you can place what you'd like here, or nothing at all.

#### eelVersion
Should always be 3.

#### requires
A list of features required by the EEL to function properly. Options: adc, spi, i2c, pwm, ble, wifi, gpio, embedded, uart

Typically, the list will be something like `["embedded", "i2c"]`

### Element Fields
These are fields that are specific to the given element object

#### name
Name of the element

#### type
Type of the element. Typically "Embedded" + name

#### icon
The icon used in the element toolbox. TODO: what about desginer view?

#### defaultAbility
The default selected ability. This must be populated with one of the element abilities.

#### defaultTrigger
The default selected trigger. This must be populated with one of the element triggers.

#### hidden
Sets the element visibility.

#### abilities
A list of objects specifying the element's abilities. See [Ability Object](#ability-object).

### Ability Object


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