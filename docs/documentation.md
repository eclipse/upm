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
- Stick to <80 chars per line even in comments.
- No text is allowed on the same line as the start or end of a comment /** */.

####The sensor block

This is added just before the class declaration in your header(.h) file and has 
mandatory fields. For single sensor libraries, this block will actually
follow immediately after the library block. If you have multiple physical
sensors, add this to every one.
Here's an example (disregard the "@verbatim" tags in your actual code):

```
@verbatim
/**
 * @library <lib-name>
 * @sensor <chip-id>
 * @comname <component-name>
 * @altname <alt-name>
 * @altid <alt-id>
 * @type <component-category>
 * @man <component-manufacturer>
 * @web <component-weblinks>
 * @con <connection-type>
 * @kit <component-kit>
 *
 * @brief Short class/sensor description
 * 
 *   Then add a longer
 *   description here.
 *
 * @image html <component-img.jpeg>
 * @snippet <example-name.cxx> Interesting
 */
@endverbatim
```

- `<lib-name>` When adding to an existing library this needs to match that
  library's "@defgroup", otherwise this is a new library name, generally the
  same as chip id. *Mandatory*
- `<chip-id>` Usually the chip number used by the sensor. When this is not
  available or relevant, use a unique descriptor that makes sense. *Mandatory*
- `<component-name>` A short name for your sensor, can include manufacturer
  name. *Mandatory*
- `<alt-name>` Alternative names that your sensor driver might have. *Optional*
- `<alt-id>` Alternative chip-ids that your sensor driver supports. *Optional*
- `<component-category>` Mention one or more categories the sensor fits in. Can
  be 'other'. *Mandatory*
- `<component-manufacturer>` Sensor manufacturer. Can be 'generic'. *Mandatory*
- `<component-weblinks>` Links to vendors or data-sheets. *Optional*
- `<connection-type>` Specifies how does the sensor connect to the board
  *Mandatory*
- `<component-kit>` Specifies if the sensor is part of a kit. *Optional*

Existing groups that can be used for the manufacturer, connection, category and
kit tags are found in the src/upm.h file.

Optionally, a small representative image can be placed in the "docs/images"
subfolder and linked with the "@image" tag.
**Please do not use existing, copyrighted images with your sensors!**

The example should have an 'Interesting' section which will be highlighted as
a code sample in doxygen. Everything in between such tags will show up in the
class documentation when "@snippet" is added at the end of a class docstring.
Tags use this format (in "example-name.cxx"):

```
@verbatim
 //! [Interesting]

    ...example code here...

 //! [Interesting]
@endverbatim
```

For more examples take a look at the existing headers in our github repository.

####The library block

New libraries must have the "@brief", "@defgroup" and "@ingroup" tags in one
block. This usually follows the namespace and it is common to have one sensor
per library.

You should end up with something like this:

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

In "@defgroup" use the same `<lib-name>` used in the sensor block. Multiple
sensors can be added to the same library this way.
For "@ingroup" add the same values as in the sensor block for manufacturer,
category, connection type and kit. If you have multiple classes or sensors
per library, only use the "@ingroup" tags that are common for all of them.
