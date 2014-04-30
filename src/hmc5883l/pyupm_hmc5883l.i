%module pyupm_hmc5883l

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "hmc5883l_doc.i"
#endif

%include "hmc5883l.h"
%{
    #include "hmc5883l.h"
%}
