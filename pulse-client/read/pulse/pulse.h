#pragma once

#include <pulse/simple.h>

#include <read/interface.h>

#include <string>

namespace NRead {

struct TPulse: TInterface {
public:
    TPulse(
        std::string device,
        std::string sampleFormat,
        std::uint8_t numChannels,
        std::uint32_t sampleRate,
        std::size_t dataSize
    );
    TPulse(const TPulse&) = delete;
    TPulse(TPulse&&) noexcept = default;
    ~TPulse();
    TPulse& operator=(const TPulse&) = delete;
    TPulse& operator=(TPulse&&) = delete;

    std::error_code Init() noexcept override;
    std::expected<TData, std::error_code> Read() const noexcept override;

private:
    std::string Device;
    std::string SampleFormat;
    std::uint8_t NumChannels = 0;
    std::uint32_t SampleRate = 0;
    std::size_t DataSize = 0;

    pa_simple* SoundDevice = nullptr;
};

}
