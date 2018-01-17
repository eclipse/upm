%module javaupm_ozw
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%apply unsigned char *OUTPUT { uint8_t *val };

%include "ozw.hpp"
%{
    #include "ozw.hpp"
%}

%include "ozwinterface.hpp"
%{
    #include "ozwinterface.hpp"
%}

%include "ozwdump.hpp"
%{
    #include "ozwdump.hpp"
%}

%include "aeotecss6.hpp"
%{
    #include "aeotecss6.hpp"
%}

%include "aeotecsdg2.hpp"
%{
    #include "aeotecsdg2.hpp"
%}

%include "aeotecdw2e.hpp"
%{
    #include "aeotecdw2e.hpp"
%}

%include "aeotecdsb09104.hpp"
%{
    #include "aeotecdsb09104.hpp"
%}

%include "tzemt400.hpp"
%{
    #include "tzemt400.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_ozw)
