#pragma once

#include <read/interface.h>

#include <string>

namespace NRead {

struct TSocket: TInterface {
public:
    TSocket(
        std::string ip,
        std::uint16_t port,
        std::size_t dataSize
    );
    TSocket(const TSocket&) = delete;
    TSocket(TSocket&&) noexcept = default;
    ~TSocket();
    TSocket& operator=(const TSocket&) = delete;
    TSocket& operator=(TSocket&&) = delete;

    std::error_code Init() noexcept override;
    std::expected<TData, std::error_code> Read() const noexcept override;

private:
    std::string Ip;
    std::uint16_t Port = 0;
    std::size_t DataSize = 0;
    std::int32_t Sockfd = -1;
};

}
