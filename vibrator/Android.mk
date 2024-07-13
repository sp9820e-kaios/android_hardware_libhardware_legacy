# Copyright 2006 The Android Open Source Project

ifeq ($(VIBRATOR_HARDWARE),speaker)
LOCAL_SRC_FILES += vibrator/vibrator_speaker.c
else
LOCAL_SRC_FILES += vibrator/vibrator.c
endif

