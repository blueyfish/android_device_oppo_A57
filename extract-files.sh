#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

function blob_fixup() {
    case "${1}" in
        vendor/lib64/libalipay_factory.so|vendor/lib64/lib_fpc_tac_shared.so)
            sed -i 's|/system/etc/firmware|/vendor/firmware\x0\x0\x0\x0|g' "${2}"
            ;;
        vendor/lib/libmmcamera2_sensor_modules.so)
            sed -i "s/\/system\/etc\/camera\//\/vendor\/etc\/camera\//g" "${2}"
            ;;
        vendor/lib/libmmcamera2_stats_modules.so)
            sed -i "s|libandroid.so|libcamshim.so|g" "${2}"
            "${PATCHELF}" --replace-needed "libgui.so" "libgui_vendor.so" "${2}"
        ;;
    esac
}

# If we're being sourced by the common script that we called,
# stop right here. No need to go down the rabbit hole.
if [ "${BASH_SOURCE[0]}" != "${0}" ]; then
    return
fi

set -e

export DEVICE=A57
export DEVICE_COMMON=msm8937-common
export VENDOR=oppo

"./../../${VENDOR}/${DEVICE_COMMON}/extract-files.sh" "$@"
