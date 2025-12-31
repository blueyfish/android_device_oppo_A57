/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "TouchscreenGestureService"

#include <algorithm>
#include <unordered_map>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>

#include "TouchscreenGesture.h"

namespace {
typedef struct {
    int32_t keycode;
    const char* name;
} GestureInfo;

const std::unordered_map<int32_t, GestureInfo> kGestureInfoMap = {
    {0, {255, "Two fingers down swipe"}},
    {1, {250, "Down arrow"}},
    {2, {251, "Up arrow"}},
    {3, {252, "Right arrow"}},
    {4, {253, "Left arrow"}},
    {5, {259, "One finger up swipe"}},
    {6, {258, "One finger down swipe"}},
    {7, {257, "One finger left swipe"}},
    {8, {256, "One finger right swipe"}},
    {9, {260, "Letter M"}},
    {10, {254, "Letter O"}},
    {11, {261, "Letter W"}},
};

std::vector<int32_t> gestureBuf(kGestureInfoMap.size(), false);
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

constexpr const char kControlPath[] = "/proc/touchpanel/gesture_enable";

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    resultCb(gestures);

    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(
    const ::vendor::lineage::touch::V1_0::Gesture& gesture, bool enable) {

    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return false;
    }
    gestureBuf[gesture.id] = enable;

    bool anyEnabled = std::find(gestureBuf.begin(),gestureBuf.end(), true) == gestureBuf.end();
    if (!android::base::WriteStringToFile((anyEnabled ? "0" : "1"), kControlPath)) {
         LOG(ERROR) << "Failed to write " << kControlPath;
         return false;
    }

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
