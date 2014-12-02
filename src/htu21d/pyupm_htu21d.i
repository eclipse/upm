%module pyupm_htu21d
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "htu21d_doc.i"
#endif

%include "htu21d.h"
%{
    #include "htu21d.h"
%}
