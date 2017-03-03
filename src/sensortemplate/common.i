/* Include global UPM interface file */
%include "../upm.i"

/* Required for python documentation, ignored for others */
%feature("autodoc", "3");

%{
    /* SWIG preprocessor copies this directly to the output wrapper*/
    #include "sensortemplate.hpp"
%}

/* %include is a SWIG directive.  SWIG will wrap types exposed in this
 * header to the target language. */
%include "sensortemplate.hpp"
