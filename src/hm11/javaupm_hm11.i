%module javaupm_hm11
%include "../upm.i"
%include "carrays.i"
%include "../java_buffer.i"

%{
    #include "hm11.hpp"
    speed_t int_B9600 = B9600;
%}

%include "hm11.hpp"
speed_t int_B9600 = B9600;
%array_class(char, charArray);

JAVA_JNI_LOADLIBRARY(javaupm_hm11)