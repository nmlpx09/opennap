#pragma once

#include <common/types.h>
#include <write/interface.h>

#include <netinet/in.h>

#include <cstdint>
#include <string>

namespace NWrite {

struct TSocket: TInterface {
public:
    TSocket(std::string ip, std::uint16_t port);
    TSocket(const TSocket&) = delete;
    TSocket(TSocket&&) noexcept = default;
    ~TSocket();
    TSocket& operator=(const TSocket&) = delete;
    TSocket& operator=(TSocket&&) = delete;

    std::error_code Init() noexcept override;
    std::error_code Write(TData&& data) noexcept override;

private:
    std::string Ip;
    std::uint16_t Port = 0;
    sockaddr_in SockaddrIn;
    std::int32_t Sockfd = -1;
};

}
