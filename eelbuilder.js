const mkdirp = require("mkdirp");
const minimist = require('minimist');
const fs = require('fs');
const walksync = require('walk-sync');
const crypto = require('crypto');

var args = minimist(process.argv.slice(2), {  
    string: ['name','dir','dest'],
    boolean: ['new', 'help', 'generate', 'refresh'],
    alias: {'h' :'help'},
});

if( args.help || process.argv.length === 2) {
	console.log("Atmosphere EEL Utility");
	console.log("---------------------");
	console.log("--new: Generate a new bare-bones EEL directory");
	console.log("--name: Set EEL name. Used in conjunction with --new");
	console.log("--generate: Generate an EEL");
	console.log("--dir: Specify EEL project directory. Used in conjunction with --generate or --new.");
	console.log("--dest: Specify location to put generated EEL");
	console.log("\nTypical Usage");
	console.log("---------------------");
	console.log("node eelutil.js --new --name=\"tmp102\" --dir=\"~\" -> Generates a new project structure in user's home directory");
	console.log("node eelutil.js --generate --dir=\"~/TMP102_EEL\" -> Generates new EEL file from project located in ~/TMP102_EEL");
	return;
}

function getSafeEelName(friendlyName) {
	return friendlyName.replace(/ /g, '_');
}

function writeMetaData(dirName, eelName) {
	var metaData = {
		libName: eelName, 
		manufacturer: "",
		description: "",
		icon: "",
		image: "",
		version: "",
		eelVersion: "3",
		shoppingCarLinks: {

		},
		requires: ["embedded"],
		elements: [{
			name: "myelement",
			type: "",
			icon: "",
			hidden: false,
			abilities: [{
				name: "myability",
				triggers:[]
			}],
			properties: [{
				name: "myproperty",
			}],
			triggers: [],
			variables: []
		}]
	};

	console.log("Writing project metadata to " + dirName + "/metadata.json");
	fs.writeFileSync(dirName + '/metadata.json', JSON.stringify(metaData, null, 2));
}

function generate(eelDir) {
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

	if( args.dest ) {
		var eelDest = args.dest;
	} else {
		var eelDest = eelDir;
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

	console.log("Writing EEL to " + eelDest);
	
	fs.writeFileSync(eelDest + '/' + getSafeEelName(metadata.libName) + '.eel', eelJSONWithMD5);
}

if( args.generate && args.dir ) {
	generate(args.dir);
}


if( args.new && args.name ) {
	var safeName = getSafeEelName(args.name);
	var dirName = safeName + '_EEL';

	if( args.dir ) {
		dirName = args.dir + '/' + dirName;
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
