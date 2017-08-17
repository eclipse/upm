var shell = require('shelljs');
var path = require('path');
var expect = require('chai').expect;

var rootPath = path.resolve(__dirname, '../../');
var srcPath = path.resolve(rootPath, 'src');
var examplePath = path.resolve(rootPath, 'examples');

var sensorTemplateJson = require(path.join(srcPath, 'sensortemplate/sensortemplate.json'));

/**
 *  Get type name
 */
function getTypeName(value) {
  if(Array.isArray(value)) {
    return 'array'
  }
  return typeof value;
}

/**
 * Check property
 * @param {object} target Target Object to check
 * @param {string} propertyName Target property to check in target object
 * @param {object} propertySpecs Property specs
 * @param {string} propertySpecs.comment
 * @param {string} propertySpecs.type
 * @param {boolean} propertySpecs.required
 */
function checkProperty(target, propertyName, propertySpecs) {
  // Fail if a required property is missing
  if (propertySpecs.required) {
    expect(target).to.have.property(propertyName);
  }
  // Check non required property
  if (target[propertyName]) {
    var propertyType = getTypeName(target[propertyName]);
    var errorMsg = propertyName + ' property should be a ' + propertySpecs.type;
    expect(propertyType).to.be.equal(propertySpecs.type, errorMsg);
  }
}

/**
 * Check object specs
 * @param {object} object Json schema specification
 * @param {object} target Json object to check
 */
function checkObjectSpecs(object, target) {
  for (var key in object) {
    /**
     * Properties specs start with a // character
     * Properties that are not specs are skipped
     */
    if (key.indexOf('//') !== 0) {
      continue;
    }

    /**
     * We need to do a closure here to make
     * async calls like it() to hold the
     * current values of object and key in
     * the iteration
     */
    (function (object, key) {
      var keySpecs = object[key];
      var realKeyName = key.replace('// ', '');

      /**
       * "TemplateItem" is a special case
       * For objects, the TemplateItem define
       * the specs for each of the items in the object
       */
      if (realKeyName === 'TemplateItem') {
        for (var key in target) {
          (function(key, target){
            /**
             * Create a test case for each property in the
             * target object. This test will check the property
             * spectations
             */
            it('Test for ' + key + ' property', function () {
              checkProperty(target, key, keySpecs);
            });

            if (target[key] && keySpecs.type === 'object') {
              (function (object, target) {
                describe(key, function () {
                  checkObjectSpecs(object, target);
                });
              })(object['TemplateItem'], target[key]);
            }
          })(key, target);
        }
      }
      else {
        /**
         * Create a test case for each property in the
         * target object. This test will check the property
         * spectations
         */
        it('Test for ' + realKeyName + ' property', function () {
          checkProperty(target, realKeyName, keySpecs);
        });

        if (target[realKeyName] && keySpecs.type === 'object') {
          (function (object, target) {
            describe(realKeyName, function () {
              checkObjectSpecs(object, target);
            });
          })(object[realKeyName], target[realKeyName]);
        }
      }
    })(object, key);
  }
}

shell.find(srcPath)
  .filter(function (file) {
    return file.match(/\.json$/);
  })
  .filter(function (file) {
    // Filter sensortemplate.json file
    return !file.match(/sensortemplate\.json$/);
  })
  .forEach(function (jsonFile) {
    var relativePath = path.relative(rootPath, jsonFile);

    /**
     * For each json file found, create a Test Suite
     */
    describe(relativePath, function () {
      var parsedJson = require(jsonFile);
      checkObjectSpecs(sensorTemplateJson, parsedJson);
    });
  });
