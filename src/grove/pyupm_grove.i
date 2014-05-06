%module pyupm_grove

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "grove_doc.i"
#endif

%include "grove.h"
%{
    #include "grove.h"
%}
