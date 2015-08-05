%module javaupm_a110x
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
/*
%feature("director") ISRHelper;
%feature("nodirector") A110x;
*/
%apply jobject { void * };
/*
%{
    #include "isrhelper.h"
%}
%include "isrhelper.h"
*/
%{
    #include "a110x.h"
%}
%include "a110x.h"
