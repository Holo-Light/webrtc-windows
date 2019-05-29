/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef THIRD_PARTY_H264_WINUWP_H264_WINUWP_FACTORY_H_
#define THIRD_PARTY_H264_WINUWP_H264_WINUWP_FACTORY_H_

#include <vector>
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "media/base/codec.h"
#include "media/engine/webrtcvideodecoderfactory.h"
#include "media/engine/webrtcvideoencoderfactory.h"
#include <wrl/client.h>
#include <d3d11.h>

namespace webrtc {

class WinUWPH264EncoderFactory : public cricket::WebRtcVideoEncoderFactory {
 public:
  WinUWPH264EncoderFactory();

  webrtc::VideoEncoder* CreateVideoEncoder(
      const cricket::VideoCodec& codec) override;

  const std::vector<cricket::VideoCodec>& supported_codecs() const override;

  void DestroyVideoEncoder(webrtc::VideoEncoder* encoder) override;

 private:
  std::vector<cricket::VideoCodec> codecList_;
};

class WinUWPH264EncoderFactoryNew : public VideoEncoderFactory {
 public:
  std::vector<SdpVideoFormat> GetSupportedFormats() const override;

  CodecInfo QueryVideoEncoder(const SdpVideoFormat& format) const override;

  std::unique_ptr<VideoEncoder> CreateVideoEncoder(
      const SdpVideoFormat& format) override;
};

// class WinUWPH264DecoderFactory : public cricket::WebRtcVideoDecoderFactory {
//   webrtc::VideoDecoder* CreateVideoDecoder(
//       webrtc::VideoCodecType type) override;

//   void DestroyVideoDecoder(webrtc::VideoDecoder* decoder) override;
// };

class WinUWPH264DecoderFactoryNew : public VideoDecoderFactory {
 public:
  WinUWPH264DecoderFactoryNew(ID3D11Device* device) : d3d_device_(device) {}
  // Returns a list of supported video formats in order of preference, to use
  // for signaling etc.
  virtual std::vector<SdpVideoFormat> GetSupportedFormats() const override;

  // Creates a VideoDecoder for the specified format.
  virtual std::unique_ptr<VideoDecoder> CreateVideoDecoder(
      const SdpVideoFormat& format) override;

 private:
  Microsoft::WRL::ComPtr<ID3D11Device> d3d_device_;
};

}  // namespace webrtc

#endif  // THIRD_PARTY_H264_WINUWP_H264_WINUWP_FACTORY_H_
