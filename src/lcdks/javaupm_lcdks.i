%module javaupm_lcdks
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"
%include "stdint.i"

%template(byteVector) std::vector<uint8_t>;

%include "lcdks.hpp"
%{
    #include "lcdks.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_lcdks)
