# build dependencies
TEXPACKR_LOCAL_PATH := $(call my-dir)

# dependencies that need to be built first
# libpng 	- downloads libpng source code then place at ../libpng
#  		  then use our Android.mk by placing there
#  		  as well you need to comment out pngpriv.h for ZLIB_VERNUM != PNG_ZLIB_VERNUM
# libvector	- clone from https://github.com/haxpor/vector_c and place at ../vector_c
# libhashmap_c	- clone from https://github.com/haxpor/hashmap_c and place at ../hashmap_c
#
include $(TEXPACKR_LOCAL_PATH)/../libpng/Android.mk
include $(TEXPACKR_LOCAL_PATH)/../vector_c/Android.mk
include $(TEXPACKR_LOCAL_PATH)/../hashmap_c/Android.mk

LOCAL_PATH := $(TEXPACKR_LOCAL_PATH)

# shared library

include $(CLEAR_VARS)

LOCAL_MODULE := texpackr

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := src/cli.c \
	src/lib.c \
	src/meta.c \
	src/png_util.c \
	src/sprite.c \
	src/treetrv.c

LOCAL_CFLAGS := -Wall -std=c99 -O2 -pedantic
LOCAL_LDLIBS := 
LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := vector hashmap_c png

ifeq ($(NDK_DEBUG),1)
    cmd-strip :=
endif

include $(BUILD_SHARED_LIBRARY)

# static library

LOCAL_MODULE := texpackr_static
LOCAL_MODULE_FILENAME := libtexpackr

LOCAL_CFLAGS := -Wall -std=c99 -O2 -pedantic
LOCAL_LDLIBS := 
LOCAL_STATIC_LIBRARIES := vector hashmap_c png
LOCAL_SHARED_LIBRARIES :=
LOCAL_EXPORT_LDLIBS := 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)
