%module javaupm_lcm1602
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%template(byteVector) std::vector<uint8_t>;

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_lcm1602)
