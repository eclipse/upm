/* Any java package including an interface .i requires an import for
   upm_core java package in the JNI source */
%pragma(java) jniclassimports=%{
import upm_core.*;
%}

/* Add the global UPM methods to all wrappers */
%{
#include "../upm_library_globals.hpp"
%}
%include "../upm_library_globals.hpp"
