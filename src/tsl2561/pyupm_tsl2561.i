%module pyupm_tsl2561
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "tsl2561_doc.i"
#endif

%include "tsl2561.h"
%{
    #include "tsl2561.h"
%}
