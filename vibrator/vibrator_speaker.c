/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hardware_legacy/vibrator.h>
#include "qemu.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <signal.h>

#define LOG_TAG "vibrator"

#define THE_DEVICE "/dev/pipe/vibrator.player.ctrl"

int vibrator_exists()
{
    int fd;

#ifdef QEMU_HARDWARE
//    if (qemu_check()) {
//        return 1;
//    }
#endif

    fd = open(THE_DEVICE, O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        return 0;
    close(fd);
    return 1;
}

static int sendit(int timeout_ms)
{
    int nwr, ret, fd;
    char value[20];


#ifdef QEMU_HARDWARE
//    if (qemu_check()) {
//        return qemu_control_command( "vibrator:%d", timeout_ms );
//    }
#endif
	ALOGD("vibrator timeout_ms %d",timeout_ms);

	if(timeout_ms > 0 && timeout_ms < 20){
		ALOGD("vibrator timeout_ms %d, timeout is too short",timeout_ms);
		return 0;
	}
	fd = open(THE_DEVICE,  O_WRONLY | O_NONBLOCK);

    if(fd < 0) {
		ALOGD("open /dev/pipe/vibrator.player.ctrl fail");
        return errno;
    }

    nwr = sprintf(value, "%08x", timeout_ms);
    ret = write(fd, value, nwr);

    close(fd);

    return (ret == nwr) ? 0 : -1;
}

int vibrator_on(int timeout_ms)
{
    ALOGD("vibrator_on");
    char value[PROPERTY_VALUE_MAX];
    if (property_get("media.vibrator.enable", value, "0") < 0) {
        ALOGE("vibrator_on property get error!");
        return -1;
    }
    if (strcmp(value, "1") == 0) {
    } else {
        ALOGE("vibrator_on do not allow vibrate!");
        return -1;
    }

    return sendit(timeout_ms);
}

int vibrator_off()
{
    ALOGD("vibrator_off");
    return sendit(0);
}
