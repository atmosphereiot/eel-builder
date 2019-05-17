const mkdirp = require("mkdirp");
const minimist = require('minimist');
const fs = require('fs');
const walksync = require('walk-sync');
const crypto = require('crypto');
const os = require('os');
var path = require('path');
const eol = require('eol')

var args = minimist(process.argv.slice(2), {  
    string: ['name','dir','dest'],
    boolean: ['new', 'help', 'generate', 'refresh', 'verbose'],
    alias: {'h' :'help'},
});

var VALID_INPUTS = ["driverInstance", "number", "text", "checkbox", "select"];
var VALID_ICONS = ["EmbeddedAccelerometer.svg",
	"EmbeddedAccelerometerMagnetometer.svg",
	"EmbeddedADCPin.svg",
	"EmbeddedAirQuality.svg",
	"EmbeddedAmbientLight.svg",
	"EmbeddedBeep.svg",
	"EmbeddedBLEConnection.svg",
	"EmbeddedCellularConnection.svg",
	"EmbeddedCurrentMonitor.svg",
	"EmbeddedEthernetConnection.svg",
	"EmbeddedGeneric.svg",
	"EmbeddedGyroscope.svg",
	"EmbeddedHumidity.svg",
	"EmbeddedLCDScreen.svg",
	"EmbeddedLEDArray.svg",
	"EmbeddedLoraWanConnection.svg",
	"EmbeddedMagnetometer.svg",
	"EmbeddedMotion.svg",
	"EmbeddedMotor.svg",
	"EmbeddedPressure.svg",
	"EmbeddedRelay.svg",
	"EmbeddedSigfox.svg",
	"EmbeddedTemperature.svg",
	"EmbeddedTempHumidity.svg",
	"EmbeddedTouch.svg",
	"EmbeddedWifiConnection.svg"];

var VALID_FILE_TYPES = ["c", "h", "cpp", "txt"];

var VALID_DRIVERS = [
	"adc",
	"ble",
	"block",
	"cellular",
	"datetime",
	"embeddedCloud",
	"filesytem",
	"gpio",
	"http",
	"i2c",
	"interval",
	"nfc",
	"pwm",
	"spi",
	"uart",
	"wifi"
]

if( args.help || process.argv.length === 2) {
	console.log("Atmosphere EEL Utility");
	console.log("---------------------");
	console.log("--new: Generate a new bare-bones EEL directory");
	console.log("--name: Set EEL name. Used in conjunction with --new");
	console.log("--generate: Generate an EEL from an EEL directory");
	console.log("--dir: Specify EEL project source directory. Used in conjunction with --generate or --new.");
	console.log("--dest: Specify location to put generated EEL file");
	console.log("--verbose: Provide verbose output");
	console.log("\nTypical Usage");
	console.log("---------------------");
	console.log("node eelutil.js --new --name=\"tmp102\" --dir=\"~\" -> Generates a new project structure in user's home directory");
	console.log("node eelutil.js --generate --dir=\"~/TMP102_EEL\" -> Generates new EEL file from project located in ~/TMP102_EEL");
	return;
}

function loadConfig() {
	// Look in these locations, in order, for a configuration file
	var configFileLocations = [
		path.join(os.homedir(), '/.atmosphereiot/eel-builder.conf'),
		path.join(os.homedir(), '/eel-builder.conf'),
		path.join(__dirname, '/eel-builder.conf'),
	];

	for(var i = 0; i < configFileLocations.length; ++i) {
		if(fs.existsSync(configFileLocations[i])) {
			if(args.verbose) {
				console.log("Found config file at: " + configFileLocations[i]);
			}
			var configFileData = fs.readFileSync(configFileLocations[i]);
			return JSON.parse(configFileData);
		}
	}

	// Didn't find anything
	return {};
}

function getSafeEelName(friendlyName) {
	return friendlyName.replace(/ /g, '_');
}

function writeMetaData(dirName, eelName) {
	var metaData = {
		libName: eelName, 
		manufacturer: "",
		description: "",
		version: "",
		eelVersion: "3",
		shoppingCarLinks: {

		},
		requires: ["embedded"],
		elements: [{
			name: "myelement",
			type: "",
			icon: "",
			defaultAbility: "",
			defaultTrigger: "",
			hidden: false,
			abilities: [{
				name: "setup",
				hidden: true,
				triggers:[]
			}],
			properties: [{
				name: "myproperty",
				type: "number",
				value: 0
			}],
			triggers: [],
			variables: [],
			language: {
				"en-US": {
					
				}
			}
		}]
	};

	console.log("Writing project metadata to " + dirName + "/metadata.json");
	fs.writeFileSync(dirName + '/metadata.json', JSON.stringify(metaData, null, 2));
}

function checkForLanguageTag(elementdata, tag) {
	// Validate language for element metadata
	var success = true;

	Object.keys(elementdata.language).forEach(function(language) {
		if(elementdata.language[language][tag] == undefined) {
			success = false;
		}
	});

	return success;
}

function validate(metadata, isVariant) {
	console.log(`Validating ${metadata.libName}`);
	var alphaNumericRegex = new RegExp("^[a-zA-Z0-9]*$");
	var success = true;

	if(!alphaNumericRegex.test(metadata.libName)) {
		console.log(`ERROR: libName (${metadata.libName}) must contain only letters and numbers`);
		success = false;
	}

	metadata.elements.forEach(function(element) {
		if(!alphaNumericRegex.test(element.name)) {
			console.log(`ERROR: Element Name (${element.name}) must contain only letters and numbers`);
			success = false;
		}

		if(element.icon != undefined) {
			if(!VALID_ICONS.includes(element.icon)) {
				console.log(`ERROR: Element Icon (${element.icon}) is not valid. Valid options: ${VALID_ICONS}`);
				success = false;
			}
		} else {
			if(!isVariant) {
				console.log(`ERROR: Element Icon field missing for element ${element.name}`);
				success = false;
			}
		}


		if(!alphaNumericRegex.test(element.type)) {
			console.log(`ERROR: Element Type (${element.type}) must contain only letters and numbers`);
			success = false;
		}

		if(!checkForLanguageTag(element, element.type)) {
			console.log(`ERROR: Language tag for element type ${element.type} missing`);
			success = false;
		}

		if(element.defaultAbility != undefined) {
			if(!alphaNumericRegex.test(element.defaultAbility)) {
				console.log(`ERROR: Element Default Ability (${element.defaultAbility}) must contain only letters and numbers`);
				success = false;
			}
		} else {
			if(!isVariant) {
				console.log(`ERROR: Element Default Ability field missing for element ${element.name}`);
				success = false;	
			}
		}


		if(element.defaultTrigger != undefined) {
			if(!alphaNumericRegex.test(element.defaultTrigger)) {
				console.log(`ERROR: Element Default Trigger (${element.defaultTrigger}) must contain only letters and numbers`);
				success = false;
			}
		} else {
			if(!isVariant) {
				console.log(`ERROR: Element Default Trigger field missing for element ${element.name}`);
				success = false;		
			}
		}

		if(element.abilities != undefined) {
			element.abilities.forEach(function(ability) {
				if(!alphaNumericRegex.test(ability.name)) {
					console.log(`ERROR: Ability Name (${ability.name}) must contain only letters and numbers`);
					success = false;
				}
				
				if(ability.hidden == undefined || ability.hidden == false) {
					if(!checkForLanguageTag(element, ability.name)) {
						console.log(`ERROR: Language tag for ability (${ability.name}) missing`);
						success = false;
					}
				}
				
				if(ability.triggers) {
					ability.triggers.forEach(function(trigger) {
						if(!alphaNumericRegex.test(trigger)) {
							console.log(`ERROR: Trigger (${trigger}) must contain only letters and numbers`);
							success = false;
						}
	
						if(!checkForLanguageTag(element, trigger)) {
							console.log(`ERROR: Language tag for trigger (${trigger}) missing`);
							success = false;
						}
					});
				}
			});
		} else {
			if(!isVariant) {
				console.log(`ERROR: Element Abilities field missing for element ${element.name}`);
				success = false;	
			}
		}

		if(element.properties != undefined) {
			element.properties.forEach(function(property) {
				if(!alphaNumericRegex.test(property.name)) {
					console.log(`ERROR: Property Name (${property.name}) must contain only letters and numbers`);
					success = false;
				}
	
				if(!VALID_INPUTS.includes(property.input)) {
					console.log(`ERROR: Property Input (${property.input}) invalid. Valid options: ${VALID_INPUTS}`);
					success = false;
				}
	
				if(property.driverType) {
					if(!VALID_DRIVERS.includes(property.driverType)) {
						console.log(`ERROR: Driver Type (${property.driverType}) invalid. Valid options: ${VALID_DRIVERS}`);
						success = false;
					}
				}
	
				if(property.input == "select" && property.inputOptions == undefined) {
					console.log(`ERROR: Property ${property.name} Missing Input Options for Input Type "select"`);
					success = false;
					
				}
	
				if(property.input == "driverInstance" && property.driverType == undefined) {
					console.log(`ERROR: Property ${property.name} Missing Driver Type for Input Type "driverInstance"`);
					success = false;
				}
	
				
			});
		} else {
			if(!isVariant) {
				console.log(`ERROR: Element Properties field missing for element ${element.name}`);
				success = false;	
			}
		}
	});

	// Validate variants
	if(metadata.variants) {
		console.log(`Validating variant ${variant.libname}`);
		metadata.variants.forEach(function(variant) {
			if(!validate(variant, true)) {
				success = false;
			}
		});
	}

	return success;
}

function generate(eelDir, eelDest) {

	mkdirp.sync(eelDest);

	try {
		var metadata = JSON.parse(fs.readFileSync(eelDir + '/metadata.json','utf8'));
	} catch (error) {
		console.log("Error opening and parsing " + eelDir + '/metadata.json');
		console.log(error);
		return;
	}

	metadata.elements.forEach(function(element) {


		// Check abilities
		element.abilities.forEach(function(ability) {

			// If a specific code file was specified
			var abilityDir = eelDir + '/elements/' + element.type + '/abilities/';

			if( ability.code ) {
				var codePath = abilityDir + ability.code;
			} else {
				var codePath = abilityDir + ability.name + '.c';
			}

			if(!fs.existsSync(abilityDir))
			{
				console.log("Ability dir does not exist, creating it.")
				mkdirp.sync(abilityDir);
			}

			try {
				var code = fs.readFileSync(codePath, 'utf8');
			} catch(error) {
				console.log("Unable to read ability source " + codePath);
				console.log("Generating default stub " + codePath);
				var code = 'return ATMO_Status_Success;';
				fs.writeFileSync(codePath, code, 'utf8');
			}

			ability.code = code;
		});
	});

	if(!validate(metadata, false)) {
		console.log("Failed to validate EEL metadata\n");
		return;
	}

	metadata.files = {};

	if(fs.existsSync(eelDir + '/files')) {
		// Populate files
		walksync(eelDir + '/files', { directories: false }).forEach(function(file) {

			// For each file, split out each dir level
			splitPath = file.split('/');
			var dirs = splitPath.slice(0, splitPath.length - 1);
			var fileName = splitPath[splitPath.length - 1];

			// Keep digging down, creating an object for each subdir
			var curPath = metadata.files;

			dirs.forEach(function(dir) {
				if(!(dir in curPath)) {
					curPath[dir] = {};
				}
				curPath = curPath[dir];
			});

			if(!VALID_FILE_TYPES.includes(fileName.split('.').pop())) {
				console.log(`ERROR: Invalid File ${fileName} will be ignored`);
			} else {
				curPath[fileName] = fs.readFileSync(eelDir + '/files/' + file, 'utf8');
			}
		});
	}

	var eelJSONWithoutMD5 = JSON.stringify(metadata, null, 2);
	
	var eelMD5 = crypto.createHash('md5').update(eelJSONWithoutMD5).digest("hex");
	
	metadata.md5 = eelMD5;
	
	var eelJSONWithMD5 = JSON.stringify(metadata, null, 2);
	
	var checkEelMD5Data = JSON.parse(eelJSONWithMD5);
	delete checkEelMD5Data.md5;
	var testEELMD5 = crypto.createHash('md5').update(JSON.stringify(checkEelMD5Data, null, 2)).digest("hex");
	
	if(testEELMD5 !== eelMD5) {
		console.error('ERROR: Failed to confirm MD5 for EEL');
	}

	var eelFileName = path.join(eelDest, getSafeEelName(metadata.libName) + ".eel");

	console.log("Writing EEL to " + eelFileName + "\n");

	// Replace newlines
	fs.writeFileSync(eelFileName, eol.lf(eelJSONWithMD5));
}

function getEelDirName(eelName) {
	var safeName = getSafeEelName(args.name);
	var dirName = safeName + '_EEL';
	return dirName;
}

var config = loadConfig();

if(args.verbose) {
	if(config) {
		console.log(JSON.stringify(config, null, 2));
	}
}

var dir = args.dir || config.eelSourceDir;
var dest = args.dest || config.eelFileDir || dir;

if( args.generate ) {

	if(!dir) {
		console.log("Error: You must enter a directory via --dir or a configuration file.");
		return;
	}

	if(!dest) {
		console.log("Error: You must enter a directory via --dest or a configuration file.");
		return;
	}

	// If the user is using a configuration file
	// They don't need to supply --dir and --dest arguments
	// They can just supply --name and the tool will find the directory
	if(!fs.existsSync(path.join(dir, 'metadata.json'))) {
		if(args.verbose) {
			console.log(`metadata.json not found in ${path.join(dir, 'metadata.json')}`);
		}
		if(!args.name) {
			console.log("metadata.json not found at " + dir + ", please specify full directory via --dir or EEL name with --name");
			return;
		}

		var dirName = getEelDirName(args.name);
		var possibleDir = path.join(dir, dirName);

		if(args.verbose) {
			console.log(`Trying ${possibleDir}`);
		}

		if(!fs.existsSync(possibleDir)) {
			console.log("metadata.json not found at " + possibleDir + ", please specify full directory via --dir or EEL name with --name");
			return;
		}

		generate(possibleDir, dest);
	} else {
		generate(dir, dest);
	}
}

if( args.new && args.name ) {
	var dirName = getEelDirName(args.name);

	if( dir ) {
		dirName = dir + '/' + dirName;
	} else {
		console.log("Error: You must enter a directory via --dir or a configuration file.");
		return;
	}

	console.log("Generating project in " + dirName);
	mkdirp.sync(dirName);
	mkdirp.sync(dirName + '/elements');
	mkdirp.sync(dirName + '/files/common/objects');
	mkdirp.sync(dirName + '/files/common/headers');
	mkdirp.sync(dirName + '/files/common/other');

	writeMetaData(dirName, args.name);
	console.log("Done!");
}
