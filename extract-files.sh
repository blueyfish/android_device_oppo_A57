#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=A57
VENDOR=oppo

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

ANDROID_ROOT="${MY_DIR}/../../.."

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        --only-common )
                ONLY_COMMON=true
                ;;
        --only-target )
                ONLY_TARGET=true
                ;;
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC="adb"
fi

function blob_fixup() {
    case "${1}" in
        product/etc/permissions/vendor.qti.hardware.data.connection-V1.0-java.xml|product/etc/permissions/vendor.qti.hardware.data.connection-V1.1-java.xml)
            sed -i 's/version="2.0"/version="1.0"/g' "${2}"
            ;;
        product/lib/libdpmframework.so|product/lib64/libdpmframework.so)
            "${PATCHELF}" --replace-needed "libhidltransport.so" "libcutils-v29.so" "${2}"
            ;;
        vendor/bin/mm-qcamera-daemon)
            sed -i "s/\xca\x56\xbc\xc0/\xca\x56\xd0\xc0/" "${2}"
            ;;
        vendor/bin/hw/android.hardware.biometrics.fingerprint@2.1-service.A57)
            # Correct firmware files
            sed -i 's|/vendor/firmware_mnt/image/|fingerprint\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0|g' "${2}"
            sed -i 's|\x21\x0c\x06\x91|\x21\x0c\x28\x91|g' "${2}"
            sed -i 's|\xc1\xff\xff\xb0\xc2\xff\xff\x90\xe8\x03\x19\x32|\xc1\xff\xff\x90\xc2\xff\xff\xb0\xe8\x03\x19\x32|g' "${2}"
            sed -i 's|\x42\xe0\x3b\x91|\x42\x0c\x06\x91|g' "${2}"

            # Remove hw_reset support
            sed -i 's|\xa0\x01\x00\x94|\x1f\x20\x03\xd5|g' "${2}"
            sed -i 's|\x84\x01\x00\x94|\x1f\x20\x03\xd5|g' "${2}"
            sed -i 's|\x6e\x01\x00\x94|\x1f\x20\x03\xd5|g' "${2}"
            sed -i 's|\x80\x22\x40\xf9\x5b\x02\x00\x94|\x1f\x20\x03\xd5\x1f\x20\x03\xd5|g' "${2}"
            sed -i 's|\x80\x22\x40\xf9\x43\x02\x00\x94|\x1f\x20\x03\xd5\x1f\x20\x03\xd5|g' "${2}"
            sed -i 's|\x1f\x20\x03\xd5\x20\x01\x00\x34|\x1f\x20\x03\xd5\x09\x00\x00\x14|g' "${2}"
            ;;
        vendor/etc/init/android.hardware.biometrics.fingerprint@2.1-service.A57.rc)
            sed -i 's/mata/A57/g' "${2}"
            sed -i 's/service fps_hal_A57/service vendor.fps_hal_A57/g' "${2}"
            ;;
        vendor/lib/mediadrm/libwvdrmengine.so|vendor/lib64/mediadrm/libwvdrmengine.so)
            "${PATCHELF}" --replace-needed "libprotobuf-cpp-lite.so" "libprotobuf-cpp-lite-v29.so" "${2}"
            ;;
        vendor/lib/libmmcamera2_sensor_modules.so)
            sed -i "s/\/system\/etc\/camera\//\/vendor\/etc\/camera\//g" "${2}"
            sed -i "s/\xca\x56\xbc\xc0/\xca\x56\xd0\xc0/" "${2}"
            ;;
        vendor/lib/libmmcamera2_stats_modules.so)
            sed -i "s|libandroid.so|libcamshim.so|g" "${2}"
            "${PATCHELF}" --replace-needed "libgui.so" "libgui_vendor.so" "${2}"
            ;;
        vendor/lib64/libril-qc-hal-qmi.so)
            "${PATCHELF}" --replace-needed "libprotobuf-cpp-full.so" "libprotobuf-cpp-full-v29.so" "${2}"
            ;;
        vendor/lib64/libsettings.so)
            "${PATCHELF}" --replace-needed "libprotobuf-cpp-full.so" "libprotobuf-cpp-full-v29.so" "${2}"
            ;;
        vendor/lib64/libwvhidl.so)
            "${PATCHELF}" --replace-needed "libprotobuf-cpp-lite.so" "libprotobuf-cpp-lite-v29.so" "${2}"
            ;;
        product/lib64/libdpmframework.so)
            "${PATCHELF}" --add-needed "libshim_dpmframework.so" "${2}"
            ;;
    esac
}

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${ANDROID_ROOT}" false "${CLEAN_VENDOR}"

extract "${MY_DIR}/proprietary-files.txt" "${SRC}" \
        "${KANG}" --section "${SECTION}"

"${MY_DIR}/setup-makefiles.sh"
