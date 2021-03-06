/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_MIXER_FRAME_COMBINER_H_
#define MODULES_AUDIO_MIXER_FRAME_COMBINER_H_

#include <memory>
#include <vector>

#include "modules/audio_processing/agc2/fixed_gain_controller.h"
#include "modules/audio_processing/include/audio_processing.h"

namespace webrtc {
class ApmDataDumper;
class FixedGainController;

class FrameCombiner {
 public:
  enum class LimiterType { kNoLimiter, kApmAgcLimiter, kApmAgc2Limiter };
  explicit FrameCombiner(LimiterType limiter_type);
  explicit FrameCombiner(bool use_limiter);
  ~FrameCombiner();

  void SetLimiterType(LimiterType limiter_type);

  // Combine several frames into one. Assumes sample_rate,
  // samples_per_channel of the input frames match the parameters. The
  // parameters 'number_of_channels' and 'sample_rate' are needed
  // because 'mix_list' can be empty. The parameter
  // 'number_of_streams' is used for determining whether to pass the
  // data through a limiter.
  void Combine(const std::vector<AudioFrame*>& mix_list,
               size_t number_of_channels,
               int sample_rate,
               size_t number_of_streams,
               AudioFrame* audio_frame_for_mixing);

 private:
  void LogMixingStats(const std::vector<AudioFrame*>& mix_list,
                      int sample_rate,
                      size_t number_of_streams) const;

  LimiterType limiter_type_;
  std::unique_ptr<AudioProcessing> apm_agc_limiter_;
  std::unique_ptr<ApmDataDumper> data_dumper_;
  FixedGainController apm_agc2_limiter_;
  mutable int uma_logging_counter_ = 0;
};
}  // namespace webrtc

#endif  // MODULES_AUDIO_MIXER_FRAME_COMBINER_H_
