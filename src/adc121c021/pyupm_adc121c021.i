// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adc121c021
%include "../upm.i"

%feature("autodoc", "3");

#ifdef DOXYGEN
%include "adc121c021_doc.i"
#endif

%include "adc121c021.hpp"
%{
    #include "adc121c021.hpp"
%}
