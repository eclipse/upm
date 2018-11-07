var shjs = require('shelljs');
var path = require('path');
var assert = require('assert');

/**
 * @returns {string} A relative path from this repository root
 */
function getRelativePath(absolutePath) {
  return path.relative(process.cwd(), absolutePath)
}

// Find all .json files in upm/src directory
var upmSrcPath = path.resolve(__dirname, '../upm/src');
console.log('Finding json files in ' + getRelativePath(upmSrcPath));
var jsonFiles = shjs.find(upmSrcPath).filter(function(file) {
  return file.match(/\.json$/);
});

// Report the ammount of json files found in upm/src
console.log('Found ' + jsonFiles.length + ' json files');

// Load _assets/content/sensorDetail.json
var sensorDetailPath = path.resolve(__dirname, '../_assets/content/sensorDetail.json');
console.log('Loading sensor details from ' + getRelativePath(sensorDetailPath));
var sensorDetailJson = require(sensorDetailPath);

// Check if the ammount of json files match the sensor details
console.log('Checking if sensor detail contains an entry for every json file in ' + getRelativePath(upmSrcPath));
console.log('jsonFiles.length: ' + jsonFiles.length);
console.log('sensorDetailJson.length: ' + sensorDetailJson.length);
var errorMessage = 'Sensor Details and Json files count does not match.';
assert.equal(jsonFiles.length, sensorDetailJson.length, errorMessage);
