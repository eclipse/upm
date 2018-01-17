%module javaupm_button
%include "../upm.i"

%apply int {mraa::Edge}

%{
    #include "button.hpp"
%}

%include "button.hpp"

#%ignore

%extend upm::Button {
    void installISR(mraa::Edge level, jobject runnable)
    {
      $self->installISR(level, mraa_java_isr_callback, runnable);
    }
}

JAVA_JNI_LOADLIBRARY(javaupm_button)
