%module javaupm_jhd1313m1
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%template(byteVector) std::vector<uint8_t>;

%include "jhd1313m1.hpp"
%{
    #include "jhd1313m1.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_jhd1313m1)
