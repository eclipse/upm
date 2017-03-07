%module javaupm_md
%include "../upm.i"
%include "enums.swg"

%javaconst(1);
enum MD_REG_T {
	MD_REG_SET_SPEED           = 0x82,
    MD_REG_SET_PWM_FREQ        = 0x84,
    MD_REG_SET_DIRECTION       = 0xaa,
    MD_REG_SET_MOTOR_A         = 0xa1, // not documented
    MD_REG_SET_MOTOR_B         = 0xa5, // not documented
    MD_REG_STEPPER_ENABLE      = 0x1a,
    MD_REG_STEPPER_DISABLE     = 0x1b,
    MD_REG_STEPPER_NUM_STEPS   = 0x1c
};

enum MD_STEP_DIRECTION_T {
    MD_STEP_DIR_CCW    = 0x01,
    MD_STEP_DIR_CW     = 0x00
};

enum MD_DC_DIRECTION_T {
	MD_DIR_CCW    = 0x02,
    MD_DIR_CW     = 0x01
};

enum MD_STEP_MODE_T {
	MD_STEP_MODE1 = 0x00,
    MD_STEP_MODE2 = 0x01
};

%{
    #include "md.hpp"
%}

%include "md.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_md");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
