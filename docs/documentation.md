Writing sensor documentation                        {#documentation}
=====================

It is highly encouraged to provide at least some basic documentation for the
sensors that you want to add to UPM:

- Try to have no warnings in doxygen, this is generally fairly easy.
- Have the specific sensor manufacturer/model & version that you used, if you
  support multiple versions please list.
- Simple comments do not need full stops
- Stick to <80 chars per line even in comments
- No text is allowed on the same line as the start or end of a comment /** */

New libraries must have the "@brief", "@defgroup" and "@ingroup" tags in one
block. This usually follows the namespace and it is common to have one sensor
per library.

Here's how this looks:

```
@verbatim
/**
 * @brief Short description for entire library
 * @defgroup <name> <libupm-name>
 * @ingroup <manufacturer> <connection type> <sensor category> (<addl group>)
 */
@endverbatim
```

If you have multiple classes or sensors per library, only use the "@ingroup"
tags that are common for all of them.

All classes should then have a "@brief", "@ingroup" and "@snippet". For single
sensor libraries this block can follow immediately after the one above like in
this example:

```
@verbatim
/**
 * @brief Short class/sensor description
 *
 * Then add a longer
 * description here.
 *
 * @ingroup <name> (<addl group>)
 * @image html <name.jpeg>
 * @snippet <name.cxx> Interesting
 */
@endverbatim
```

Libraries with multiple sensors can add specific "@ingroup" tags here, but make
sure that the first one is the "<name>" specified in the library "@defgroup"
tag. An example of such a library for reference is our libupm-i2clcd.

Optionally, a small representative image can be placed in the "docs/images"
subfolder. **Please do not use existing, copyrighted images with your sensors!**

The example should have an 'Interesting' section which will be highlighted as
a code sample in doxygen. Everything in between such tags will show up in the
class documentation when "@snippet" is added at the end of a class docstring.
Tags use this format:

```
@verbatim
 //! [Interesting]

    ...example code here...

 //! [Interesting]
@endverbatim
```

Existing groups that can be used for the manufacturer, connection and category
tags are found in the src/upm.h file.

For more examples take a look at the existing headers in our github repository.
