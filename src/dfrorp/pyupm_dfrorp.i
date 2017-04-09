// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_dfrorp
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

%include "dfrorp.hpp"
%{
    #include "dfrorp.hpp"
%}
