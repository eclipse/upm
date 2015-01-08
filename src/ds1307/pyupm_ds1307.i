%module pyupm_ds1307
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "ds1307_doc.i"
#endif

%include "ds1307.h"
%{
    #include "ds1307.h"
%}
