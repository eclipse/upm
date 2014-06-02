%module pyupm_lcm1602

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "lcm1602_doc.i"
#endif

%include "lcm1602.h"
%{
    #include "lcm1602.h"
%}
