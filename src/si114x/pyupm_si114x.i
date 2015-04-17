%module pyupm_si114x
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "si114x_doc.i"
#endif

%include "si114x.h"
%{
    #include "si114x.h"
%}
