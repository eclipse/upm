API Changes                       {#apichanges}
===============

**IMPORTANT NOTICE**

Our **C++ header files** will change their extension from *.h* to *.hpp*. This
change is anticipated in the upcoming version and unfortunately will break
source compatibility with previous versions of UPM for every library. You will
need to update your code and change to the new extension format in your
`#include` directives.

Here's a list of other API changes made to the library that break source/binary
compatibility between releases:

 * **my9221**, **groveledbar** and **grovecircularled** are now all part of the
 same library (my9221) and new functionality was added going to v.0.5.1
 * **stepmotor** driver API was changed significantly from v.0.4.1 to v.0.5.0
 * **eboled** library was greatly improved in version 0.4.0 and the `draw()`
 function was removed in favor of a more complete GFX library implementation
 