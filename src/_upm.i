/* Macro for adding JAVA runnables.
    example usage: JAVA_ADD_INSTALLISR(upm::A110x)
        UPMClassName - UPM class name w/namespace.
*/
%define JAVA_ADD_INSTALLISR(UPMClassName)
%extend UPMClassName {
    void installISR(jobject runnable)
    {
        $self->installISR(mraa_java_isr_callback, runnable);
    }
}
%enddef

/* Macro for adding JNI loadLibrary dependency.
    example usage: JAVA_JNI_LOADLIBRARY(javaupm_rhusb)
        PackageName: Target UPM JAVA package.
*/
%define JAVA_JNI_LOADLIBRARY(PackageName)
%pragma(java) jniclasscode=%{
static {
    try {
        System.loadLibrary("PackageName");
    } catch (UnsatisfiedLinkError e) {
        System.err.println("Native code library (PackageName) failed to load. \n" + e);
        System.exit(1);
    }
}
%}
%enddef
