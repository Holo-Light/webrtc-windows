/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

#include <vector>
#include "third_party/winuwp_h264/winuwp_h264_factory.h"
#include "third_party/winuwp_h264/H264Encoder/H264Encoder.h"
#include "third_party/winuwp_h264/H264Decoder/H264Decoder.h"
#include "media/engine/webrtcvideoencoderfactory.h"
#include "media/engine/webrtcvideodecoderfactory.h"
#include "rtc_base/logging.h"

namespace webrtc {

  WinUWPH264EncoderFactory::WinUWPH264EncoderFactory() {
    codecList_ =
      std::vector<cricket::VideoCodec> {
        cricket::VideoCodec("H264")
    };
  }

  webrtc::VideoEncoder* WinUWPH264EncoderFactory::CreateVideoEncoder(
    const cricket::VideoCodec& codec) {
    if (codec.name == "H264") {
      return new WinUWPH264EncoderImpl();
    } else {
      return nullptr;
    }
  }

  const std::vector<cricket::VideoCodec>&
    WinUWPH264EncoderFactory::supported_codecs() const {
    return codecList_;
  }

  void WinUWPH264EncoderFactory::DestroyVideoEncoder(
    webrtc::VideoEncoder* encoder) {
      encoder->Release();
      delete encoder;
  }


  // webrtc::VideoDecoder* WinUWPH264DecoderFactory::CreateVideoDecoder(
  //   webrtc::VideoCodecType type) {
  //   if (type == kVideoCodecH264) {
  //     return new WinUWPH264DecoderImpl(d3d_device_.Get());
  //   } else {
  //     return nullptr;
  //   }
  // }

  // void WinUWPH264DecoderFactory::DestroyVideoDecoder(
  //   webrtc::VideoDecoder* decoder) {
  //   decoder->Release();
  //   delete decoder;
  // }

  std::vector<SdpVideoFormat> WinUWPH264EncoderFactoryNew::GetSupportedFormats()
    const {
    std::vector<SdpVideoFormat> formats = {
      SdpVideoFormat(cricket::kH264CodecName,
      {
        //copy-pasted from h264.cc
        {cricket::kH264FmtpProfileLevelId, "42100b"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "0"}
      }),
      SdpVideoFormat(cricket::kH264CodecName,
      {
        {cricket::kH264FmtpProfileLevelId, "42100b"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "1"}
      }),
      // These are needed for android to be able to connect. I think it's H264 CBP
      // (constrained baseline profile) vs BP (baseline profile) though I don't know
      // which is which.
      SdpVideoFormat(cricket::kH264CodecName,
      {
        {cricket::kH264FmtpProfileLevelId, "42e01f"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "0"}
      }),
      SdpVideoFormat(cricket::kH264CodecName,
      {
        {cricket::kH264FmtpProfileLevelId, "42e01f"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "1"}
      })
    };
    return formats;
  }

  VideoEncoderFactory::CodecInfo WinUWPH264EncoderFactoryNew::QueryVideoEncoder(
    const SdpVideoFormat& format) const {
    CodecInfo info;
    //not sure about this. mf does support hw MFTs but doesn't really tell us
    //when using sink writer. it's more of a "silent sw fallback if hw not available" thing
    info.is_hardware_accelerated = false;
    info.has_internal_source = false;
    return info;
  }

  std::unique_ptr<VideoEncoder> WinUWPH264EncoderFactoryNew::CreateVideoEncoder(
    const SdpVideoFormat& format) {
    if (cricket::CodecNamesEq(format.name, cricket::kH264CodecName)) {
      return std::make_unique<WinUWPH264EncoderImpl>();
    }

    RTC_LOG(LS_ERROR) << "Trying to create encoder of unsupported format "
                    << format.name;
    return nullptr;
  }

  std::vector<SdpVideoFormat> WinUWPH264DecoderFactoryNew::GetSupportedFormats() const {

    // COPY-PASTED FROM VIDEOENCODERFACTORYNEW
    std::vector<SdpVideoFormat> formats = {
      SdpVideoFormat(cricket::kH264CodecName,
      {
        //copy-pasted from h264.cc
        {cricket::kH264FmtpProfileLevelId, "42100b"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "0"}
      }),
      SdpVideoFormat(cricket::kH264CodecName,
      {
        {cricket::kH264FmtpProfileLevelId, "42100b"},
        {cricket::kH264FmtpLevelAsymmetryAllowed, "1"},
        {cricket::kH264FmtpPacketizationMode, "1"}
      })
    };
    return formats;
  }

  std::unique_ptr<VideoDecoder> WinUWPH264DecoderFactoryNew::CreateVideoDecoder(
    const SdpVideoFormat& format) {
    if (cricket::CodecNamesEq(format.name, cricket::kH264CodecName)) {
      return std::make_unique<WinUWPH264DecoderImpl>(d3d_device_.Get());
    }

    RTC_LOG(LS_ERROR) << "Trying to create decoder of unsupported format "
                    << format.name;
    return nullptr;
  }

}  // namespace webrtc

