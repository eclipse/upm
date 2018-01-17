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
