var shell = require('shelljs');
var path = require('path');
var expect = require('chai').expect;

var rootPath = path.resolve(__dirname, '../../');
var srcPath = path.resolve(rootPath, 'src');
var examplePath = path.resolve(rootPath, 'examples');

shell.find(srcPath)
  .filter(function (file) {
    return file.match(/\.json$/);
  })
  .forEach(function (jsonFile) {
    var relativePath = path.relative(rootPath, jsonFile);

    describe(relativePath, function () {
      // Check required fields
      [
        'Library',
        'Description',
        'Sensor Class'
      ].forEach(function (fieldName) {
        it('should have a ' + fieldName + ' property', function () {
          var parsedJsonFile = require(jsonFile);
          expect(parsedJsonFile).to.have.property(fieldName);
       });
      });

      // Check Sensor Class required fields
      [
        'Name',
        'Description',
        'Aliases',
        'Categories',
        'Connections',
        'Project Type',
        'Manufacturers',
        'Image',
        'Examples',
        'Specifications',
        'Platforms',
        'Urls'
      ].forEach(function (fieldName) {
        var parsedJsonFile = require(jsonFile);
        sensorClass = parsedJsonFile['Sensor Class'];

        for(var className in sensorClass) {
          it(className + ' should have a ' + fieldName + ' property', function () {
            var parsedJsonFile = require(jsonFile);
            sensorClass = parsedJsonFile['Sensor Class'];
            expect(sensorClass[className]).to.have.property(fieldName);
          });
        }
      });
    });
  });

