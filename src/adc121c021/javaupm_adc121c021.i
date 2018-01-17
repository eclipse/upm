%module javaupm_adc121c021
%include "../upm.i"
/*%include "../carrays_uint16_t.i"
*/
%{
    #include "adc121c021.hpp"
%}

%include "adc121c021.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_adc121c021)