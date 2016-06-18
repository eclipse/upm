// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_bmx055
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

/* Send "int *" and "float *" to python as intp and floatp */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "bmx055_doc.i"
#endif

%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}

%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}

%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}

%include "bmx055.hpp"
%{
    #include "bmx055.hpp"
%}

%include "bmc150.hpp"
%{
    #include "bmc150.hpp"
%}

%include "bmi055.hpp"
%{
    #include "bmi055.hpp"
%}
