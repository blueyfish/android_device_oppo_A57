/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "msm_flash.h"

static msm_flash_t pmic_flash_def =
{
    .flash_name = "pmic",
    .flash_type = 0,
    .flash_driver_type = FLASH_DRIVER_PMIC,
    .num_of_flash = 1,
    .frame_skip_main_flash_on = 3,
    .frame_skip_main_flash_off = 3,
    .frame_skip_torch_on = 2,
    .frame_skip_torch_off = 2,
};

void *pmic_flash_driver_open_lib(void)
{
    return &pmic_flash_def;
}
