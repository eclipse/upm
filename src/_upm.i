/*
 * This file provides additional non-wrapper code functionality for use
 * throughout the UPM repository.  This file is intended to be %imported
 * from the top-level upm.i (not directly by UPM libraries).
 *
 * No wrapper code will be generated from importing this .i file.
 */


/* Macro for adding JAVA installISR w/runnable.
    UPM_Classname - UPM class name w/namespace.

    example usage: JAVA_ADD_INSTALLISR(upm::A110x)
*/
%define JAVA_ADD_INSTALLISR(UPM_Classname)
%extend UPM_Classname {
void installISR(jobject runnable)
{
    $self->installISR(mraa_java_isr_callback, runnable);
}
}
%enddef

/* Macro for adding JAVA installISR w/runnable for a GPIO
    UPM_Classname - UPM class name w/namespace.

    example usage: JAVA_ADD_INSTALLISR_GPIO(upm::A110x)

    Requires: Define GETTER in the appropiate .i file with the getter
    function name.
*/
%define JAVA_ADD_INSTALLISR_GPIO(UPM_Classname)
%extend UPM_Classname {
void installISR(int gpio, mraa::Edge level, jobject runnable)
{
  /* delete any existing ISR and GPIO context */
  $self->uninstallISR();

  /* create gpio context */
  mraa::Gpio* swg_gpioIntr = $self->GETTER;

  swg_gpioIntr->dir(mraa::DIR_IN);
  swg_gpioIntr->isr(level, runnable);
}
}
%enddef

/* Macro for adding JAVA installISR for GPIO interrupt w/runnable.
    UPM_Classname - UPM class name w/namespace.

    example usage: JAVA_ADD_INSTALLISR_INTERRUPT(upm::A110x)

    Requires: Define INTERRUPT in the appropiate .i file with the
    interrupt type.
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

/* Macro for adding JAVA installISR for mraa::Edge w/runnable.
    UPM_Classname - UPM class name w/namespace.

    example usage: JAVA_ADD_INSTALLISR_EDGE(upm::A110x)
*/
%define JAVA_ADD_INSTALLISR_EDGE(UPM_Classname)
%extend UPM_Classname {
void installISR(mraa::Edge level, jobject runnable)
{
  $self->installISR(level, mraa_java_isr_callback, runnable);
}
}
%enddef

/* Macro for adding JAVA installISR for pin w/runnable.
    UPM_Classname - UPM class name w/namespace.

    example usage: JAVA_ADD_INSTALLISR_PIN(upm::A110x)
*/
%define JAVA_ADD_INSTALLISR_PIN(UPM_Classname)
%extend UPM_Classname {
void installISR(int pin, jobject runnable)
{
    $self->installISR(pin, mraa_java_isr_callback, runnable);
}
}
%enddef

/* Macro for adding JNI loadLibrary dependency.
    MyModuleName: Target UPM JAVA package.

    example usage: JAVA_JNI_LOADLIBRARY(javaupm_rhusb)
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
