#pragma once

#include <alsa/asoundlib.h>

#include <write/interface.h>

#include <common/types.h>

#include <cstdint>
#include <string>

namespace NWrite {

struct TAlsa: TInterface {
public:
    TAlsa(
        std::string device,
        std::string format,
        std::int8_t channels,
        std::uint32_t rate
    );
    TAlsa(const TAlsa&) = delete;
    TAlsa(TAlsa&&) noexcept = default;
    ~TAlsa();
    TAlsa& operator=(const TAlsa&) = delete;
    TAlsa& operator=(TAlsa&&) = delete;

    std::error_code Init() noexcept override;
    std::error_code Write(TData&& data) noexcept override;

private:
    std::string Device;
    std::string Format;
    std::int8_t Channels = 0;
    std::uint32_t Rate = 0;
    
    snd_pcm_t* SoundDevice = nullptr;
    std::size_t FrameSize = 1;
};

}
