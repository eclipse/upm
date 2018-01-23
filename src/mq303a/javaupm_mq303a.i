%module javaupm_mq303a
%include "../upm.i"

%{
    #include "mq303a.hpp"
%}

%include "mq303a.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mq303a)