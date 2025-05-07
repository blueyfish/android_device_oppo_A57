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
        vendor/lib/mediadrm/libwvdrmengine.so|vendor/lib64/mediadrm/libwvdrmengine.so)
            "${PATCHELF}" --replace-needed "libprotobuf-cpp-lite.so" "libprotobuf-cpp-lite-v29.so" "${2}"
            ;;
        vendor/bin/rmt_storage)
            # Drop IO priority set-up
            sed -i "s|\xe0\x0f\x1f\x32|\x0e\x00\x00\x14|g" "${2}"

            # Update setgroups to fix wake locks
            sed -i "s|\x88\x77\xc1\xd2\x08\x7d\x80\xf2|\x08\x7d\x80\xd2\x48\x78\xc1\xf2|g" "${2}"
            ;;
        vendor/lib64/hw/fingerprint.msm8937.so)
            "${PATCHELF}" --add-needed "libshims_binder.so" "${2}"
            ;;
        vendor/lib64/libalipay_factory.so|vendor/lib64/lib_fpc_tac_shared.so)
            sed -i 's|/system/etc/firmware|/vendor/firmware\x0\x0\x0\x0|g' "${2}"
            ;;
        vendor/bin/mm-qcamera-daemon|vendor/lib/libmmcamera2_cpp_module.so|vendor/lib/libmmcamera2_dcrf.so|vendor/lib/libmmcamera2_iface_modules.so|vendor/lib/libmmcamera2_imglib_modules.so|vendor/lib/libmmcamera2_mct.so|vendor/lib/libmmcamera2_pproc_modules.so|vendor/lib/libmmcamera2_q3a_core.so|vendor/lib/libmmcamera2_stats_algorithm.so|vendor/lib/libmmcamera_imglib.so|vendor/lib/libmmcamera_pdaf.so|vendor/lib/libmmcamera_pdafcamif.so|vendor/lib/libmmcamera_tintless_algo.so|vendor/lib/libmmcamera_tintless_bg_pca_algo.so|vendor/lib/libmmcamera_tuning.so|vendor/lib/libremosaic_daemon.so|vendor/lib64/libremosaic_daemon.so)
            sed -i "s|/data/misc/camera/|/data/vendor/qcam/|g" "${2}"
            ;;
        vendor/lib/libmmcamera_dbg.so)
            sed -i "s|/data/vendor/camera/|///data/vendor/qcam/|g" "${2}"
            ;;
        vendor/lib/libmmcamera2_sensor_modules.so)
            sed -i "s|/system/etc/camera/|/vendor/etc/camera/|g" "${2}"
            sed -i "s|/data/misc/camera/|/data/vendor/qcam/|g" "${2}"
            ;;
        vendor/lib/libmmcamera2_stats_modules.so)
            sed -i "s|libandroid.so|libcamshim.so|g" "${2}"
            "${PATCHELF}" --replace-needed "libgui.so" "libgui_vendor.so" "${2}"
            ;;
        vendor/lib/libcvface_api.so)
            "${PATCHELF}" --remove-needed "libjnigraphics.so" "${2}"
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
extract "${MY_DIR}/proprietary-files-camera.txt" "${SRC}" \
        "${KANG}" --section "${SECTION}"

"${MY_DIR}/setup-makefiles.sh"
