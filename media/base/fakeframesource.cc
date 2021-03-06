/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "media/base/fakeframesource.h"

#include "api/video/i420_buffer.h"

namespace cricket {

FakeFrameSource::FakeFrameSource(int width, int height, int interval_us)
    : width_(width), height_(height), interval_us_(interval_us) {
  RTC_CHECK_GT(width_, 0);
  RTC_CHECK_GT(height_, 0);
  RTC_CHECK_GT(interval_us_, 0);
}

webrtc::VideoRotation FakeFrameSource::GetRotation() {
  return rotation_;
}

void FakeFrameSource::SetRotation(webrtc::VideoRotation rotation) {
  rotation_ = rotation;
}

webrtc::VideoFrame FakeFrameSource::GetFrame() {
  return GetFrame(width_, height_, interval_us_);
}

webrtc::VideoFrame FakeFrameSource::GetFrame(int width,
                                             int height,
                                             int interval_us) {
  RTC_CHECK_GT(width, 0);
  RTC_CHECK_GT(height, 0);
  RTC_CHECK_GT(interval_us, 0);

  rtc::scoped_refptr<webrtc::I420Buffer> buffer(
      webrtc::I420Buffer::Create(width, height));

  buffer->InitializeData();
  webrtc::VideoFrame frame =
      webrtc::VideoFrame(buffer, rotation_, next_timestamp_us_);

  next_timestamp_us_ += interval_us;
  return frame;
}

}  // namespace cricket
