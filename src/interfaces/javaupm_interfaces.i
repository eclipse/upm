%module javaupm_interfaces

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_interfaces");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
