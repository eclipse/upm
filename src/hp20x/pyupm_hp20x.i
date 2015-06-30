%module pyupm_hp20x
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "hp20x_doc.i"
#endif

%include "hp20x.h"
%{
    #include "hp20x.h"
%}
