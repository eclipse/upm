// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_dfrec
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

%include "dfrec.hpp"
%{
    #include "dfrec.hpp"
%}
