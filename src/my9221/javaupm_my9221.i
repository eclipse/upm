%module javaupm_my9221
%include "../upm.i"

%include "my9221.hpp"
%{
    #include "my9221.hpp"
%}

%include "groveledbar.hpp"
%{
    #include "groveledbar.hpp"
%}

%include "grovecircularled.hpp"
%{
    #include "grovecircularled.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_my9221)
