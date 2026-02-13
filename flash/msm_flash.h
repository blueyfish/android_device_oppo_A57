/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <media/msm_camsensor_sdk-legacy-m.h>

/* For camera stack with `void *data[10]` removed */
struct sdk_msm_sensor_power_setting {
	enum msm_sensor_power_seq_type_t seq_type;
	unsigned short seq_val;
	long config_val;
	unsigned short delay;
};

/* Copy from kernel with pointers removed */
struct sdk_msm_sensor_power_setting_array {
	struct sdk_msm_sensor_power_setting  power_setting_a[MAX_POWER_CONFIG];
	unsigned short size;
	struct sdk_msm_sensor_power_setting  power_down_setting_a[MAX_POWER_CONFIG];
	unsigned short size_down;
};

/* For camera stack with `I2C_REG_SET_MAX` as 100 */
struct sdk_msm_camera_i2c_reg_setting_array {
	struct msm_camera_i2c_reg_array reg_setting_a[100];
	unsigned short size;
	enum msm_camera_i2c_reg_addr_type addr_type;
	enum msm_camera_i2c_data_type data_type;
	unsigned short delay;
};

typedef struct {
	unsigned int slave_addr;
	enum i2c_freq_mode_t i2c_freq_mode;
	enum msm_camera_i2c_reg_addr_type i2c_addr_type;

	/* Derived from log message in flash_process() */
	struct sdk_msm_camera_i2c_reg_setting_array flash_init_settings;
	struct sdk_msm_camera_i2c_reg_setting_array flash_off_settings;
	struct sdk_msm_camera_i2c_reg_setting_array flash_low_settings;
	struct sdk_msm_camera_i2c_reg_setting_array flash_high_settings;
} msm_flash_i2c_params_t;

typedef struct {
	char flash_name[64];
	unsigned int flash_type;
	enum msm_flash_driver_type flash_driver_type;
	struct sdk_msm_sensor_power_setting_array power_setting_array;
	msm_flash_i2c_params_t i2c_flash_info;
	unsigned char num_of_flash;

	/* Derived from log message in flash_init():
	 * "i = %d flash_current = %d flash_duration = %d"
	 *
	 * `torch_max_current` is deduced through legacy drivers.
	 */
	unsigned int flash_max_current[MAX_LED_TRIGGERS];
	unsigned int torch_max_current[MAX_LED_TRIGGERS];
	unsigned int flash_max_duration[MAX_LED_TRIGGERS];

	/* Derived from log message in flash_get_frame_skip_timing_params():
	 * "Flash frame skip timing params: main flash on = %d,
	 *  main flash off = %d, torch on = %d, torch off = %d"
	 */
	unsigned int frame_skip_main_flash_on;
	unsigned int frame_skip_main_flash_off;
	unsigned int frame_skip_torch_on;
	unsigned int frame_skip_torch_off;
} msm_flash_t;
