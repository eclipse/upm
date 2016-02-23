// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
 
%module pyupm_apa102
 
%include "../upm.i"
 
 
%feature("autodoc", "3");
 
 
%include "apa102.h"
 
%{
 
    #include "apa102.h"
%}
