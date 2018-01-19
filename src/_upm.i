/* Macro for adding JAVA run
    usage: JAVA_ADD_INSTALLISR(javaupm_a110x)
*/
%define JAVA_ADD_INSTALLISR(UPM_Classname)
%extend UPM_Classname {
    void installISR(jobject runnable)
    {
        $self->installISR(mraa_java_isr_callback, runnable);
    }
}
%enddef

/* Define GETTER in the appropiate .i file with the getter function name
*/
%define JAVA_ADD_INSTALLISR_GPIO(UPM_Classname)
%extend UPM_Classname {
    void installISR(int gpio, mraa::Edge level, jobject runnable)
    {
      // delete any existing ISR and GPIO context
      $self->uninstallISR();

      // create gpio context
      mraa::Gpio* swg_gpioIntr = $self->GETTER;

      swg_gpioIntr->dir(mraa::DIR_IN);
      swg_gpioIntr->isr(level, runnable);

    }
}
%enddef

/* Define INTERRUPT in the appropiate .i file with the interrupt type
*/
%define JAVA_ADD_INSTALLISR_INTERRUPT(UPM_Classname)
%extend UPM_Classname {
    void installISR(INTERRUPT intr, int gpio,
                    mraa::Edge level, jobject runnable)
    {
        $self->installISR(intr, gpio, level, mraa_java_isr_callback, runnable);
    }
}
%enddef

%define JAVA_ADD_INSTALLISR_EDGE(UPM_Classname)
%extend UPM_Classname {
    void installISR(mraa::Edge level, jobject runnable)
    {
      $self->installISR(level, mraa_java_isr_callback, runnable);
    }
}
%enddef

%define JAVA_ADD_INSTALLISR_PIN(UPM_Classname)
%extend UPM_Classname {
    void installISR(int pin, jobject runnable)
    {
        $self->installISR(pin, mraa_java_isr_callback, runnable);
    }

}
%enddef

/* Macro for adding JNI loadLibrary dependency
    usage: JAVA_JNI_LOADLIBRARY(javaupm_rhusb)
*/
%define JAVA_JNI_LOADLIBRARY(MyModuleName)
%pragma(java) jniclasscode=%{
static {
    try {
        System.loadLibrary("MyModuleName");
    } catch (UnsatisfiedLinkError e) {
        System.err.println("Native code library (MyModuleName) failed to load. \n" + e);
        System.exit(1);
    }
}
%}
%enddef
