API Changes                       {#apichanges}
===============

Here's a list of other API changes made to the library that break source/binary
compatibility between releases:

 * Our **C++ header files** changed their extension from *.h* to *.hpp* in
 version 0.7.0, Intel provided examples and code samples also reflect this
 change but you will need to modify your `#include` directives in existing code
 * **my9221**, **groveledbar** and **grovecircularled** are now all part of the
 same library (my9221) and new functionality was added going to v.0.5.1
 * **stepmotor** driver API was changed significantly from v.0.4.1 to v.0.5.0
 * **eboled** library was greatly improved in version 0.4.0 and the `draw()`
 function was removed in favor of a more complete GFX library implementation
 