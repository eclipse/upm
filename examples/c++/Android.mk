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

upm_headers := \
    $(sort $(dir $(wildcard $(LOCAL_PATH)/../../src/**/*.h*))) \
    $(LOCAL_PATH)/../../include \

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cxx
LOCAL_CPPFLAGS := $(upm_cppflags)
LOCAL_SHARED_LIBRARIES := libcutils libupm libpmraa
LOCAL_C_INCLUDES := $(upm_headers)
LOCAL_MODULE := jhd1313m1-lcd
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := jhd1313m1-lcd.cxx
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cxx
LOCAL_CPPFLAGS := $(upm_cppflags)
LOCAL_SHARED_LIBRARIES := libcutils libupm libpmraa
LOCAL_C_INCLUDES := $(upm_headers)
LOCAL_MODULE := grove-relay
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := groverelay.cxx
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cxx
LOCAL_CPPFLAGS := $(upm_cppflags)
LOCAL_SHARED_LIBRARIES := libcutils libupm libpmraa
LOCAL_C_INCLUDES := $(upm_headers)
LOCAL_MODULE := spi-display
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := st7735.cxx
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cxx
LOCAL_CPPFLAGS := $(upm_cppflags)
LOCAL_SHARED_LIBRARIES := libcutils libupm libpmraa
LOCAL_C_INCLUDES := $(upm_headers)
LOCAL_MODULE := grove-button
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := grovebutton.cxx
include $(BUILD_EXECUTABLE)
