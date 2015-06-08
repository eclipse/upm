%module pyupm_m24lr64e
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "m24lr64e_doc.i"
#endif

%include "m24lr64e.h"
%{
    #include "m24lr64e.h"
%}
