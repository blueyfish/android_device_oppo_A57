/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <libinit_dalvik_heap.h>
#include <libinit_utils.h>
#include <libinit_variant.h>

#include "vendor_init.h"

#include <android-base/file.h>
#include <android-base/logging.h>

using android::base::ReadFileToString;

static const variant_info_t a57_info = {
    .brand = "OPPO",
    .device = "A57",
    .marketname = "OPPO A57",
    .model = "OPPO A57",
    .build_fingerprint = "OPPO/A57/A57:6.0.1/MMB29M/1527754036:user/release-keys",
    .build_description = "msm8937_64-user 6.0.1 MMB29M eng.root.20191205.095236 dev-keys",
    .build_ota = "A57_11.A.32_0320_201912050917",
};

static const variant_info_t a57t_info = {
    .brand = "OPPO",
    .device = "A57",
    .marketname = "OPPO A57t",
    .model = "OPPO A57t",
    .build_fingerprint = "OPPO/A57t/A57:6.0.1/MMB29M/1527754036:user/release-keys",
    .build_description = "msm8937_64-user 6.0.1 MMB29M eng.root.20191205.101424 dev-keys",
    .build_ota = "A57t_11.A.22_0220_201912050917",
};

static const variant_info_t cph1701_info = {
    .brand = "OPPO",
    .device = "CPH1701",
    .marketname = "OPPO A57",
    .model = "CPH1701",
    .build_fingerprint = "Android/msm8937_64/msm8937_64:6.0.1/MMB29M/root10091402:user/release-keys",
    .build_description = "msm8937_64-user 6.0.1 MMB29M eng.root.20181009.140111 release-keys",
    .build_ota = "CPH1701EX_11.A.36_INT_036_201810091339",
};

static const variant_info_t cph1701fw_info = {
    .brand = "OPPO",
    .device = "CPH1701fw",
    .marketname = "OPPO A57",
    .model = "CPH1701fw",
    .build_fingerprint = "Android/msm8937_64/msm8937_64:6.0.1/MMB29M/root10091402:user/release-keys",
    .build_description = "msm8937_64-user 6.0.1 MMB29M eng.root.20181009.140111 release-keys",
    .build_ota = "CPH1701EX_11.A.36_INT_036_201810091339",
};

static void determine_device() {
    std::string kPrjVersion, kOperatorName;
    int prjVersion, operatorName = 0;

    // Determine project version
    ReadFileToString("/proc/oppoVersion/prjVersion", &kPrjVersion);
    prjVersion = std::stoi(kPrjVersion);

    // Determine operator name
    ReadFileToString("/proc/oppoVersion/operatorName", &kOperatorName);
    operatorName = std::stoi(kOperatorName);

    // Match operator name
    if (prjVersion == 16061) {
        bool isGlobal = false;

        switch (operatorName) {
            /* China */
            case 8:
                switch (ReadPcbVersion()) {
                    /* 16062 -> A57t */
                    case 3:
                    case 5:
                    case 10:
                    case 11:
                        set_variant_props(a57t_info);
                        break;

                    /* 16061 -> A57 */
                    default:
                        set_variant_props(a57_info);
                        break;
                }
                break;

            /* Global */
            case 102:
            case 106:
            case 110:
            case 111:
            case 112:
            case 113:
            case 114:
            {
                std::string reserve_exp1;
                isGlobal = true;
                if (ReadFileToString("/dev/block/bootdevice/by-name/reserve_exp1", &reserve_exp1)) {
                    if (!strncmp(reserve_exp1.c_str(), "00010001", 8)) {
                        set_variant_props(cph1701fw_info);
                        break;
                    }
                }
                set_variant_props(cph1701_info);
                break;
            }

            default:
                LOG(WARNING) << "Unknown operator variant, setting A57";
                set_variant_props(a57_info);
                break;
        }

        if (isGlobal) {
            switch (ReadOperatorName()) {
                case 106:
                    property_override("ro.vendor.wlan_fw_variant", "16361");
                    break;
                case 102:
                case 110:
                case 111:
                    property_override("ro.vendor.wlan_fw_variant", "16061_second");
                    break;
                default: /* 112, 113, 114 */
                    property_override("ro.vendor.wlan_fw_variant", "16061");
                    break;
            }
        } else {
            switch (ReadPcbVersion()) {
                case 10:
                case 11:
                    property_override("ro.vendor.wlan_fw_variant", "16061_second");
                    break;
                default: /* CN + 112, 113, 114 */
                    property_override("ro.vendor.wlan_fw_variant", "16061");
                    break;
            }
        }
    } else {
        LOG(ERROR) << "Unknown device variant";
    }
}

void vendor_load_properties() {
    determine_device();
    set_dalvik_heap();
}
