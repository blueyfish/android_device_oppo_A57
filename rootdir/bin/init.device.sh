#! /vendor/bin/sh

#
# Copyright (C) 2025 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

operator_name=$(cat /proc/oppoVersion/operatorName)
pcb_version=$(cat /proc/oppoVersion/pcbVersion)

# Set separate soft property
setprop ro.separate.soft 16061

# WiFi NV
if [ $operator_name = 8 ]; then
    # China
    case $pcb_version in
        "10" | "11" )
            setprop ro.vendor.wifi.nv 16061_second
            ;;
        * ) # CN + 112, 113, 114
            setprop ro.vendor.wifi.nv 16061
            ;;
    esac
else
    # Global
    case $operator_name in
        "106" )
            setprop ro.vendor.wifi.nv 16361
            ;;
        "102" | "110" | "111" )
            setprop ro.vendor.wifi.nv 16061_second
            ;;
        * ) # 112, 113, 114
            setprop ro.vendor.wifi.nv 16061
            ;;
    esac
fi
