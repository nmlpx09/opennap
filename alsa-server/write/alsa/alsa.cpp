#include "alsa.h"

#include <write/errors.h>

namespace NWrite {

TAlsa::TAlsa(
    std::string device,
    std::string format,
    std::int8_t channels,
    std::uint32_t rate
)
: Device(std::move(device))
, Format(std::move(format))
, Channels(channels)
, Rate(rate) { }

TAlsa::~TAlsa() {
    if (SoundDevice != nullptr) {
        snd_pcm_drain(SoundDevice);
        snd_pcm_close(SoundDevice);
    }
}

std::error_code TAlsa::Init() noexcept {
    if (auto err = snd_pcm_open(&SoundDevice, Device.c_str(), SND_PCM_STREAM_PLAYBACK, 0); err < 0) {
        return EErrorCode::DeviceOpen;
    }

    snd_pcm_hw_params_t* hwParams = nullptr;
    snd_pcm_hw_params_alloca(&hwParams);
 
    if (auto err = snd_pcm_hw_params_any(SoundDevice, hwParams); err < 0) {
        return EErrorCode::InitializeParameter;
    }

    if (auto err = snd_pcm_hw_params_set_rate_resample(SoundDevice, hwParams, 1); err < 0) {
        return EErrorCode::Resampling;
    }

    if (auto err = snd_pcm_hw_params_set_access(SoundDevice, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED); err < 0) {
        return EErrorCode::SetAccess;
    }

    if (Channels > 2) {
        return EErrorCode::Channels;
    }

    if (auto err = snd_pcm_hw_params_set_channels(SoundDevice, hwParams, Channels); err < 0) {
        return EErrorCode::SetChannels;
    }

    snd_pcm_format_t format;
    if (Format == "s32le") {
        format = SND_PCM_FORMAT_S32_LE;
        FrameSize = sizeof(std::int32_t) * Channels;
    } else {
        return EErrorCode::Format;
    }

    if (auto err = snd_pcm_hw_params_set_format(SoundDevice, hwParams, format); err < 0) {
        return EErrorCode::SetFormat;
    }

    if (Rate != 48000) {
        return EErrorCode::Rate;
    }

    std::uint32_t rate = Rate;
    if (auto err = snd_pcm_hw_params_set_rate_near(SoundDevice, hwParams, &rate, 0); err < 0) {
        return EErrorCode::SetRate;
    }

    snd_pcm_uframes_t bufferSize = Rate * FrameSize / 100;
    if (auto err = snd_pcm_hw_params_set_buffer_size_near(SoundDevice, hwParams, &bufferSize); err < 0) {
        return EErrorCode::SetBuffer;
    }

    if (auto err = snd_pcm_hw_params(SoundDevice, hwParams); err < 0) {
        return EErrorCode::SetHwParams;
    }

    if (auto err = snd_pcm_prepare(SoundDevice); err < 0) {
        return EErrorCode::Prepare;
    }

    return {};
}

std::error_code TAlsa::Write(TData&& data) noexcept {
    if (SoundDevice == nullptr) {
        return EErrorCode::DeviceInit;
    }
    auto frames = data.size() / FrameSize;

    auto err = snd_pcm_writei(SoundDevice, data.data(), frames);

    if (err == -ENODEV) {
        return Init();
    } else if (err == -EPIPE) {
        snd_pcm_prepare(SoundDevice);
        snd_pcm_writei(SoundDevice, data.data(), frames);
    }
    return {};
}

}
