%module pyupm_grovemd
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "grovemd_doc.i"
#endif

%include "grovemd.h"
%{
    #include "grovemd.h"
%}
