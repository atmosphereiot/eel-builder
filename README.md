# EEL Utility

This is designed to be a simple command line utility to help developers generate EEL files.

## EEL Examples
See the [examples](/examples) directory for annotated example EELs.

There is a detailed walktrough creating a simple EEL [here](/examples/my_first_eel).

## General Usage

Run `node eelutil.js --new --name="tmp102" --dir="/home/nick"` to generate a bare-bones directory containing an eel metadata.json file and empty directories for files and elements. 

Once you have this directory created and you've customized the metadata file and written all of your code, you can run `node eelutil.js --generate --dir="/home/nick/tmp102_EEL" --dest="/home/nick/` to generate the .eel file, which will be located at `/home/nick/tmp102.eel`.

## Config File
This tool accepts an optional configuration file named `eel-builder.conf`. The tool will look in the following locations (in order) for the configuration file, using the first one it finds and then stopping:

1.) `${HOME}/.atmosphereiot/`
2.) `${HOME}`
3.) Directory of this project's source (the same directory as this README).

The configuration file is in JSON format and contains two keys:
* `eelSourceDir` - The home directory of any EEL source. If you, for example, generate a new eel called tmp102, a directory containing this EEL's source will be generated in `${eelSourceDir}/tmp102_EEL`. If you use this configuration option, you do not need to supply the `--dir` option on the command line. 
* `eelFileDir` - The directory that any generated .eel files will be written to. If you use this configuration option, you do not need to supply the `--dest` option on the command line. 

*NOTE*: If you supply `eelSourceDir` in the configuration file, you must use the `--name` option when you generate your eel file (eg `eelutil.js --generate --name="tmp102"`). The tool will automatically search in `${eelSourceDir}/tmp102_EEL` for the `metadata.json` file for your EEL.

### Full Usage Example with Config File
1.) Place the following in ${HOME}/.atmosphereiot/eel-builder.conf:
```js
{
    "eelSourceDir": "/home/[YOUR USERNAME]/myEelSource",
    "eelFileDir": "/home/[YOUR USERNAME]/myGeneratedEels"
}
```

Replace `[YOUR USERNAME]` with your username. If in Windows, replace `/home/[YOUR USERNAME]` with `C:\\Users\\[YOUR USERNAME]`.

2.) Run `node eelbuilder.js --new --name="tmp102"`. You should see the following output:
```
Generating project in /home/[YOUR USERNAME]/myEelSource/tmp102_EEL
Writing project metadata to /home/[YOUR USERNAME]/myEelSource/tmp102_EEL/metadata.json
Done!
```

3.) Run `node eelbuilder.js --generate --name="tmp102"`. You should see the following output:
```
Ability dir does not exist, creating it.
Unable to read ability source home/[YOUR USERNAME]/myEelSource/tmp102_EEL/elements/abilities/myability.c
Generating default stub home/[YOUR USERNAME]/myEelSource/tmp102_EEL/elements//abilities/myability.c
Writing EEL to home/[YOUR USERNAME]/myGeneratedEels/tmp102.eel
```

### Building Example EELS
Run `./build_examples.sh` to build all of the example EELs.

## Building Process
Building an eel happens in three steps

### 1.) Creating a source directory
Use the `--new` option to generate a new EEL source directory with a bare-bones metadata file. You must use the `--name` option in conjunction with this option. If you're not using a configuration file, you must also use the `--dir` option. 

The directory will be located at `${dir}/${name}_EEL`. The tool will also output this directory on the command line.

### 2.) Write your code
Create your driver, element, and ability code. 

### 3.) Generate an EEL
Use the `--generate` option to pacakge everything into a single EEL file that can be imported into Atmosphere Studio. If you're not using a configuration file, you must use the `--dir` option to point the tool to the EEL source directory, and the `--dest` option to tell the tool where to put the generated .eel file. 

If you're using the configuration file, and you've specified the `eelSourceDir` configuration option, you may use the `--name` option instead, and the tool will automatically find the source directory for you. 

## metadata.json

This is intended to be a user editable file. Here's an example:
```js
{
	"libName": "irthermoclick",
	"manufacturer": "Mikroelektronika",
	"description": "MLX90614ESF-AAA Temperature Sensor mikroBUS™ Click™ Platform Evaluation Expansion Board",
	"type": "Temperature Sensor",
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
					"input":"driverInstance",
					"driverType": "i2c"
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
This is the type of sensor. Typical values would be "Temperature Sensor", "Humidity Sensor", "Accelerometer", etc...

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
Type of the element. Typically "Embedded" + name. No spaces or other special characters, only letters and numbers.

#### icon
The icon used. Must be one of the following:

* `EmbeddedAccelerometer.svg`
* `EmbeddedADCPin.svg`
* `EmbeddedAirQuality.svg`
* `EmbeddedAmbientLight.svg`
* `EmbeddedBeep.svg`
* `EmbeddedBLEConnection.svg`
* `EmbeddedCellularConnection.svg`
* `EmbeddedCurrentMonitor.svg`
* `EmbeddedEthernetConnection.svg`
* `EmbeddedGeneric.svg`
* `EmbeddedGyroscope.svg`
* `EmbeddedHumidity.svg`
* `EmbeddedLCDScreen.svg`
* `EmbeddedLEDArray.svg`
* `EmbeddedLoraWanConnection.svg`
* `EmbeddedMagnetometer.svg`
* `EmbeddedMotion.svg`
* `EmbeddedMotor.svg`
* `EmbeddedPressure.svg`
* `EmbeddedRelay.svg`
* `EmbeddedSigfox.svg`
* `EmbeddedTemperature.svg`
* `EmbeddedTempHumidity.svg`
* `EmbeddedTouch.svg`
* `EmbeddedWifiConnection.svg`


If none of these apply, you can use `EmbeddedFunction.svg` and write an issue so we can make an applicable icon.

#### defaultAbility
The default selected ability. This must be populated with one of the element abilities.

#### defaultTrigger
The default selected trigger. This must be populated with one of the element triggers.

#### hidden
Sets the element visibility.

#### abilities
A list of objects specifying the element's abilities. See [Ability Object](#ability-object).

_NOTE_: Every element _must contain_ a 'setup' ability. This ability will be called once on startup automatically.

#### properties
A list of objects specifying the element's properties. See [Property Object](#property-object).

#### triggers
A list of triggers not associated with any specific abilities. Typically empty.

#### variables
A list of variables and their types, used to store element state. See [Relay Click](examples/relayclick/metadata.json) for an example of usage. 

It is considered good practice to use a variable called `handle` to store an integer handle for each instance of your EEL, as the [Relay Click](examples/relayclick) EEL does. This allows for multiple Relay Clicks to be used in the same project, with the handle being used to differentiate the instances. For flexibility, every EEL should do this (although not all currently do).

#### language
An object containing the language encoding for the object. This specifies how abilities and names should be printed on the screen in various languages. See [Language Object](#language-object).

### Ability Object

#### name
The name of the ability. Should not have spaces.

#### hidden
Specifies the visibility of the ability in the ability list. The setup ability, for example, is hidden since it should never be manually called by a user. Abilities created only as a means to access their triggers (eg. async abilities executed on interrupts) should be hidden as well.

#### triggers
A list of string trigger names. Typically the list will only have one trigger, but you can have multiple. 

### Property Object

#### name
The name of the property. Should not have spaces.

#### input
The type of the property. Can be number, text, select, checkbox, or driverInstance.

`driverInstance` properties are to be used whenever an EEL needs a specific embedded driver to function.

#### driverType
The driver type, if a `driverInstance` property. This can be one of the following:

* `adc`
* `ble`
* `block`
* `datetime`
* `filesytem`
* `gpio`
* `http`
* `i2c`
* `interval`
* `nfc`
* `pwm`
* `spi`
* `uart`
* `wifi`

#### value
The default value of the property.

#### inputOptions
A list of available options, only used if the input type is `select`. See [LPS22HB](examples/lps22hb/metadata.json) for an example.

### Language Object
The top level keys of the language object are the [language tags](https://tools.ietf.org/html/rfc4646). Each language tag contains an object specifying keys and values translating names in the EEL metadata to pretty names to be printed on the Atmosphere GUI. Anything the EEL metadata that will be printed on the GUI should have an associated translation in the language object. If a translation is not provided, the raw value will be used.

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
