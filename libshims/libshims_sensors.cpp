/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "libshims_sensors"
#include <android-base/logging.h>

extern "C" float _ZNK7android6Sensor11getMaxValueEv() {
    LOG(WARNING) << "Ignoring call for Sensor::getMaxValue() of sensor native layer";
    return 0;
}
