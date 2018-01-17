%module (docstring="TTP223 Touch Sensor") javaupm_ttp223

%include "../upm.i"

%apply int {mraa::Edge}


%{
    #include "ttp223.hpp"
%}

%include "ttp223.hpp"

#%ignore installISR(mraa::Edge , void *, void *)

%extend upm::TTP223 {
    void installISR(mraa::Edge level, jobject runnable)
    {
      $self->installISR(level, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_ttp223)
