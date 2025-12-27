/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "quadracfa_dummy"

#include <android-base/logging.h>

extern "C" void remosaic_init(/* unused */) {
    LOG(INFO) << "inside dummy remosaic_init 1";
}

extern "C" unsigned int remosaic_gainmap_gen(/* unused */) {
    LOG(INFO) << "inside dummy remosaic_gainmap_gen 1";
    return 0;
}

extern "C" unsigned int remosaic_process(/* unused */) {
    LOG(INFO) << "inside dummy remosaic_process 1";
    return 0;
}

extern "C" void remosaic_process_param_set(/* unused */) {
    LOG(INFO) << "inside dummy remosaic_process_param_set 1";
}

extern "C" void remosaic_deinit(/* unused */) {
    LOG(INFO) << "inside dummy remosaic_deinit 1";
}
