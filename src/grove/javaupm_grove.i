%module (docstring="Basic Grove sensors") javaupm_grove

%include "../upm.i"

%apply int {mraa::Edge}

%include "grove.hpp"
%{
    #include "grove.hpp"
%}

%include "grovebase.hpp"
%{
    #include "grovebase.hpp"
%}

%include "grovebutton.hpp"
%{
    #include "grovebutton.hpp"
%}

%include "groveled.hpp"
%{
    #include "groveled.hpp"
%}

%include "grovelight.hpp"
%{
    #include "grovelight.hpp"
%}

%include "groverelay.hpp"
%{
    #include "groverelay.hpp"
%}

%include "groverotary.hpp"
%{
    #include "groverotary.hpp"
%}

%include "groveslide.hpp"
%{
    #include "groveslide.hpp"
%}

%include "grovetemp.hpp"
%{
    #include "grovetemp.hpp"
%}

%extend upm::GroveButton {
    void installISR(mraa::Edge level, jobject runnable)
    {
      $self->installISR(level, mraa_java_isr_callback, runnable);
    }
}


JAVA_JNI_LOADLIBRARY(javaupm_grove)
