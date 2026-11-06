/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "msm_flash.h"

static msm_flash_t lm3642_flash_def =
{
    .flash_name = "lm3642",
    .flash_type = 0,
    .flash_driver_type = FLASH_DRIVER_I2C,
    .power_setting_array =
    {
        .power_setting_a =
        {
            {
                .seq_type = SENSOR_VREG,
                .seq_val = CAM_VIO,
                .config_val = 0,
                .delay = 0,
            },
            {
                .seq_type = SENSOR_GPIO,
                .seq_val = SENSOR_GPIO_FL_EN,
                .config_val = GPIO_OUT_LOW,
                .delay = 1,
            },
            {
                .seq_type = SENSOR_GPIO,
                .seq_val = SENSOR_GPIO_FL_NOW,
                .config_val = GPIO_OUT_LOW,
                .delay = 1,
            },
        },
        .size = 2,
        .power_down_setting_a =
        {
            {
                .seq_type = SENSOR_VREG,
                .seq_val = CAM_VIO,
                .config_val = 0,
                .delay = 0,
            },
            {
                .seq_type = SENSOR_GPIO,
                .seq_val = SENSOR_GPIO_FL_EN,
                .config_val = GPIO_OUT_LOW,
                .delay = 1,
            },
            {
                .seq_type = SENSOR_GPIO,
                .seq_val = SENSOR_GPIO_FL_NOW,
                .config_val = GPIO_OUT_LOW,
                .delay = 1,
            },
        },
        .size_down = 2,
    },
    .i2c_flash_info =
    {
        .slave_addr = 0xc6,
        .i2c_addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
        .flash_init_settings =
        {
            .reg_setting_a =
            {
                {0x0A, 0x00, 0x00},
            },
            .size = 1,
            .addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
            .data_type = MSM_CAMERA_I2C_BYTE_DATA,
            .delay = 0,
        },
        .flash_off_settings =
        {
            .reg_setting_a =
            {
                {0x0A, 0x00, 0x00},
            },
            .size = 1,
            .addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
            .data_type = MSM_CAMERA_I2C_BYTE_DATA,
            .delay = 0,
        },
        .flash_low_settings =
        {
            .reg_setting_a =
            {
                {0x0A, 0x12, 0x00},
                {0x09, 0x00, 0x00}, //torch current 48.4mA
                {0x06, 0x00, 0x00}, /*add for torch ramp time too long*/
            },
            .size = 3,
            .addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
            .data_type = MSM_CAMERA_I2C_BYTE_DATA,
            .delay = 0,
        },
        .flash_high_settings =
        {
            .reg_setting_a =
            {
                {0x0A, 0x23, 0x00},
                {0x08, 0x04, 0x00},
                {0x09, 0x0A, 0x00},
            },
            .size = 3,
            .addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
            .data_type = MSM_CAMERA_I2C_BYTE_DATA,
            .delay = 0,
        },
    },
    .num_of_flash = 1,
    .frame_skip_main_flash_on = 3,
    .frame_skip_main_flash_off = 3,
    .frame_skip_torch_on = 2,
    .frame_skip_torch_off = 2,
};

void *lm3642_flash_driver_open_lib(void)
{
    return &lm3642_flash_def;
}
