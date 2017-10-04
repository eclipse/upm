Writing sensor documentation                        {#documentation}
=====================

It is highly encouraged to provide at least some basic documentation for the
sensors that you want to add to UPM:

- If you don't add documentation, the code review will take very long and
  your contribution could be rejected.
- Try to have no warnings in doxygen, this is generally fairly easy.
- Have the specific sensor manufacturer/model & version that you used, if you
  support multiple versions please list.
- Simple comments do not need full stops.
- Stick to <80 chars per line where possible.
- No text is allowed on the same line as the start or end of a comment /** */.

We currently document our libraries in the following way:

* **Doxygen** is used for documenting the API and generating the categories on
  the [UPM Libraries page](https://iotdk.intel.com/docs/master/upm/modules.html).
  You can learn more about the Doxygen syntax [here](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html).
* **JSON** is used to provide sensor specifications, descriptions, supported
  platforms, links and other details.

When submitting a new driver, you will have to at least fill out the mandatory
fields as described below.

### The library JSON file

Let's use the BME280 class snippet from the bmp280.json file as an example:

```json
{
    "Library": "bmp280",
    "Description": "Bosch Atmospheric Sensor Library",
    "Sensor Class":
    {
        "BME280":
        {
            "Name": "Digital Humidity, Pressure, and Temperature Sensor",
            "Description": "The BME280 is as combined digital humidity, pressure and temperature sensor based on proven sensing principles. The sensor module is housed in an extremely compact metal-lid LGA package with a footprint of only 2.5 * 2.5 mm2 with a height of 0.93 mm. Its small dimensions and its low power consumption allow the implementation in battery driven devices such as handsets, GPS modules or watches. The BME280 is register and performance compatible to the Bosch Sensortec BMP280 digital pressure sensor",
            "Aliases": ["bme280", "Grove - Barometer Sensor(BME280)"],
            "Categories": ["pressure", "humidity", "temperature"],
            "Connections": ["gpio", "i2c", "spi"],
            "Project Type": ["prototyping", "industrial"],
            "Manufacturers": ["adafruit", "seeed", "bosch"],
            "Examples":
            {
                "Java": ["BMP280_Example.java"],
                "Python": ["bmp280.py"],
                "Node.js": ["bmp280.js"],
                "C++": ["bmp280.cxx"],
                "C": ["bmp280.c"]
            },
            "Specifications":
            {
                "Vdd": {"unit": "v", "low" : 1.7, "high": 3.6},
                "Ioff" : {"unit": "mA", "low" : 0.0, "high": 0.0},
                "Iavg": {"unit": "mA", "low" : 1, "high": 2},
                "Pressure Range": {"unit": "hpA", "low" : 300, "high": 1100},
                "Temperature Range": {"unit": "C", "low" : -40, "high": 85}
            },
            "Platforms":
            {
                "Intel Joule Module":
                {
                    "Notes": ["Requires pull-up resistors with carrier board"]
                }
            },
            "Urls" :
            {
                "Product Pages": ["https://www.adafruit.com/products/2652"],
                "Datasheets": ["https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf"],
                "Schematics": ["https://learn.adafruit.com/assets/26693"]
            }
        }
    }
}
```

#### Mandatory fields:

For the library:

- `Library` The name of the library. This is appended to the upm prefix during
  a build.
- `Description` A short description of the library and what it contains.

For the sensor classes:

- `Sensor Class` This is the object containing the sensor class(es) within the
  library. Class objects need to match the name used in code.
- `Name` Title Case descriptive names for your sensor. Multiple values can be
  used to capture the chip name, generic name, or specific name for a vendor.
  Examples: Digital Pressure Sensor, Serial MP3 Module
- `Description` A more detailed explanation what the sensor does and how it
  works.
- `Categories` Mention one or more categories the sensor fits in. Accepted
  values are listed in the groups.md file.
- `Connections` Specifies how does the sensor connect to the board. Accepted
  values are listed in the groups.md file.
- `Project Type` What time of projects is the sensor suited for. For example:
  prototyping, industrial, commercial.
- `Manufacturers` List of sensor manufacturers or vendors. Can be 'generic',
  other accepted values in groups.md.
- `Examples` Names of the example files provided with the library. At a minimum,
  the `C++` example needs to be provided.
- `Urls` At least one link for `Product Pages` needs to be provided. Additional
  links to `Datasheets` or `Schematics` can be added.

#### Optional and customizable fields

- `Kits` Specifies if the sensor is part of a kit. Accepted values are listed
 in the groups.md file.
- `Image` Name of the image file provided with the sensor class.
- `Specifications` Relevant sensor specifications as listed in the datasheet.
- `Platforms` Platform specific notes or known limitations and workarounds.

As mentioned, accepted values for some of the fields are listed under the
[groups.md](../src/groups.md) file. If needed, you can add new categories
for your sensor library following the existing format.

JSON files are automatically checked for correctness and required fields on
code submissions.

**Please do not use copyrighted images with your sensors!**

Images from Seeed, DFRobot, Sparkfun or Adafruit are permitted.

### Doxygen tags

#### The library doxygen block

New libraries must have the "@brief", "@defgroup" and "@ingroup" tags in one
block. This usually follows the namespace and it is common to have one sensor
per library.

You should end up with something like this (disregard the "@verbatim" tags in
your actual code):

```
@verbatim
/**
 * @brief Short description for entire library
 *
 * Optional longer description.
 *
 * @defgroup <lib-name> libupm-<lib-name>
 * @ingroup <manufacturer> <connection> <category> (<kit>)
 */
@endverbatim
```

Use `<lib-name>` to name the library.

For "@ingroup" add the same values as in the sensor block for manufacturer,
category, connection type and kit. If you have multiple classes or sensors
per library, only use the "@ingroup" tags that are common for all of them.

Existing groups that can be used for the manufacturer, connection, category and
kit tags are found in the *src/groups.md* file.

#### The sensor doxygen block

This is added just before the class declaration in your header(.hpp) file and has
one mandatory field. For single sensor libraries, this block will actually follow
immediately after the library block. If you have multiple sensor classes, add
this to every one.

Here's an example:

```
@verbatim
/**
 * @library <lib-name>
 * @brief Short class/sensor description
 *
 *   Then add a longer
 *   description here.
 */
@endverbatim
```

When adding to an existing library, `<lib-name>` needs to match that library's
"@defgroup".

For more examples take a look at the existing headers in our github repository.
Also, make sure to check our [sensortemplate](contributions.md#creating-a-new-sensor-library-using-the-sensortemplate)
as it can facilitate new sensor additions.

Existing header files might have additional fields under the sensor block. These
have been used in the past to generate sensor pages outside of doxygen, but they
are now deprecated and not required for new additions.
