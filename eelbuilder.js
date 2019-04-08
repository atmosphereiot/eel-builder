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

			curPath[fileName] = fs.readFileSync(eelDir + '/files/' + file, 'utf8');
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
		console.error('FAILED TO CONFIRM MD5 for EEL!');
	}

	var eelFileName = path.join(eelDest, getSafeEelName(metadata.libName) + ".eel");

	console.log("Writing EEL to " + eelFileName);

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
