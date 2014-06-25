%module pyupm_grove
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "grove_doc.i"
#endif

%include "grove.h"
%{
    #include "grove.h"
%}
