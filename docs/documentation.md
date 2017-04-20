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
  platforms, links and many more.

When submitting a new driver, you will have to at least fill out the mandatory
fields as described below.

### The library JSON file

Let's use the bme280.json file for the BME280 sensor as an example:

```json
{
    "library": "bmp280",
    "description": "Bosch Atmospheric Sensor Library",
    "classes":
    {
        "BME280":
        {
            "names": ["Digital Humidity, Pressure, and Temperature Sensor", "Grove Temp&Humi&Barometer Sensor", "BME280"],
            "categories": ["pressure", "humidity", "temperature"],
            "connections": ["gpio", "i2c", "spi"],
            "manufacturers": ["adafruit", "seeed"],
            "kits": [],
            "images": ["bme280.jpeg"],
            "examples": ["bmp280-bme280.c", "bmp280-bme280.cxx", "BME280_Example.java", "bme280.js", , "bme280.py"],
            "usage": ["prototype", "product"],
            "description": "The BME280 is as combined digital humidity, pressure and temperature sensor based on proven sensing principles. The sensor module is housed in an extremely compact metal-lid LGA package with a footprint of only 2.5 * 2.5 mm2 with a height of 0.93 mm. Its small dimensions and its low power consumption allow the implementation in battery driven devices such as handsets, GPS modules or watches. The BME280 is register and performance compatible to the Bosch Sensortec BMP280 digital pressure sensor",
            "specifications":
            {
                "vdd": {"low": "1.7V", "high": "3.6V"},
                "ioff" : {"low": "0.0A", "high": "0.0A"},
                "iavg": {"low": "0.1uA", "high": "0.1uA"},
                "Pressure range" : "300...1100 hPa",
                "Temperature range" : "-40...85°C"
            },
            "platforms":
            {
                "Intel Joule Module":
                {
                    "notes": ["Requires pull-up resistors with carrier board"],
                    "images": [""]
                }
            },
            "urls" :
            {
                "products": ["https://www.adafruit.com/products/2652", "https://www.seeedstudio.com/Grove-Temp%26Humi%26Barometer-Sensor-%28BME280%29-p-2653.html"],
                "datasheets": ["https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf"],
                "schematics": ["https://learn.adafruit.com/assets/26693"]
            }
        }
    }
}
```

**Mandatory fields:**
- `names` Title Case descriptive names for your sensor. Multiple values can be
  used to capture the chip name, generic name, or specific name for a vendor.
  Examples: Digital Pressure Sensor, Serial MP3 Module, 
- `categories` Mention one or more categories the sensor fits in. Accepted
  values are listed in the groups.md file.
- `connections` Specifies how does the sensor connect to the board. Accepted
  values are listed in the groups.md file.
- `manufacturers` List of sensor manufacturers or vendors. Can be 'generic',
  other accepted values in groups.md.

**Optional and customizable fields**
- `kits` Specifies if the sensor is part of a kit.

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
