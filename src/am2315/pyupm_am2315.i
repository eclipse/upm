%module pyupm_am2315
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "am2315_doc.i"
#endif

%include "am2315.h"
%{
    #include "am2315.h"
%}
