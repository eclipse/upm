%module pyupm_pn532
%include "../upm.i"
%include "../carrays_uint8_t.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "pn532_doc.i"
#endif

%include "pn532.h"
%{
    #include "pn532.h"
%}
