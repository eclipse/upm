%module pyupm_itg3200
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "itg3200_doc.i"
#endif

%include "itg3200.h"
%{
    #include "itg3200.h"
%}
