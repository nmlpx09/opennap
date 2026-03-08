#include "pulse.h"

#include <read/errors.h>

namespace NRead {

TPulse::TPulse(
    std::string device,
    std::string sampleFormat,
    std::uint8_t numChannels,
    std::uint32_t sampleRate,
    std::size_t dataSize
)
: Device(std::move(device))
, SampleFormat(std::move(sampleFormat))
, NumChannels(numChannels)
, SampleRate(sampleRate)
, DataSize(dataSize) { }

TPulse::~TPulse() {
    if (SoundDevice != nullptr) {
        pa_simple_drain(SoundDevice, nullptr);
        pa_simple_free(SoundDevice);
    }
}

std::error_code TPulse::Init() noexcept {
    pa_sample_format_t format;

    if (NumChannels != 2) {
        return EErrorCode::Channels;
    }

    if (SampleFormat == "s32le") {
        format = PA_SAMPLE_S32LE;
    } else {
        return EErrorCode::Format;
    }

    if (SampleRate != 48000) {
        return EErrorCode::Rate;
    }

    pa_sample_spec spec = {
        .format = format,
        .rate = SampleRate,
        .channels = NumChannels
    };

    if (SoundDevice = pa_simple_new(nullptr, "opennap", PA_STREAM_RECORD, Device.c_str(), "pclient", &spec, nullptr, nullptr, nullptr); !SoundDevice) {
        return EErrorCode::DeviceInit;
    }

    return {};
}

std::expected<TData, std::error_code> TPulse::Read() const noexcept {
    if (SoundDevice == nullptr) {
        return std::unexpected(EErrorCode::DeviceInit);
    }

    if (DataSize < 1) {
        return TData{};
    }

    TData buffer(DataSize, 0);

    if (auto result = pa_simple_read(SoundDevice, buffer.data(), buffer.size(), nullptr); result < 0) {
        return std::unexpected(EErrorCode::Read);
    }

    return buffer;
}

}
