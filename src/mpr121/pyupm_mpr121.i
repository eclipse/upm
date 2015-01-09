%module pyupm_mpr121
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mpr121_doc.i"
#endif

%include "mpr121.h"
%{
    #include "mpr121.h"
%}
