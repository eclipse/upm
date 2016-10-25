#
#  Author: Sanrio Alvares <sanrio.alvares@intel.com>
#  Copyright (c) 2016 Intel Corporation.
#
#  Permission is hereby granted, free of charge, to any person obtaining
#  a copy of this software and associated documentation files (the
#  "Software"), to deal in the Software without restriction, including
#  without limitation the rights to use, copy, modify, merge, publish,
#  distribute, sublicense, and/or sell copies of the Software, and to
#  permit persons to whom the Software is furnished to do so, subject to
#  the following conditions:
#
#  The above copyright notice and this permission notice shall be
#  included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
#  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
#  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

LOCAL_PATH := $(call my-dir)

upm_cppflags := \
	-Wall \
	-Wno-sign-compare \
	-Wno-unused-parameter \
	-Werror \
	-std=gnu++11 \
	-fexceptions \
	-Wno-reorder \

upm_src_files := \
	src/utilities/upm_utilities.c \
	src/jhd1313m1/jhd1313m1.c \
	src/jhd1313m1/jhd1313m1.cxx \
	src/grove/grovebutton.cxx \
	src/grove/groverelay.cxx \
	src/st7735/st7735_gfx.cxx \
	src/st7735/st7735.cxx \
	src/hmc5883l/hmc5883l.cxx \

upm_headers := \
    $(sort $(dir $(wildcard $(LOCAL_PATH)/src/**/*.h*))) \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/types \
    $(LOCAL_PATH)/include/fti \

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cxx
LOCAL_MODULE := libupm
LOCAL_CFLAGS += -Wno-unused-parameter -DX86PLAT=1
LOCAL_CPPFLAGS += $(upm_cppflags)
LOCAL_SHARED_LIBRARIES := libcutils libpmraa
LOCAL_SRC_FILES := $(upm_src_files)
LOCAL_C_INCLUDES := $(upm_headers)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_C_INCLUDES)
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
