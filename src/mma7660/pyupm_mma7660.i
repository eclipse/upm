%module pyupm_mma7660
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "mma7660_doc.i"
#endif

%include "mma7660.h"
%{
    #include "mma7660.h"
%}
