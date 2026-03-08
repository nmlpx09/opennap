#include "socket.h"

#include <read/errors.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

namespace NRead {

TSocket::TSocket(
    std::string ip,
    std::uint16_t port,
    std::size_t dataSize
)
: Ip(std::move(ip))
, Port(port)
, DataSize(dataSize) { }

TSocket::~TSocket() {
    close(Sockfd);
}

std::error_code TSocket::Init() noexcept {
    if (Sockfd = socket(AF_INET, SOCK_DGRAM, 0); Sockfd < 0) {
        return make_error_code(EErrorCode::SocketOpen);
    }

    auto servaddr = sockaddr_in {
        .sin_family = AF_INET,
        .sin_port = htons(Port),
        .sin_addr = {
            .s_addr = inet_addr(Ip.c_str())
        },
        .sin_zero = {0}
    };

    if (bind(Sockfd, reinterpret_cast<const sockaddr*>(&servaddr), sizeof(servaddr)) < 0) {
        return make_error_code(EErrorCode::SocketBind);
    }

    return {};
}

std::expected<TData, std::error_code> TSocket::Read() const noexcept {
    if (Sockfd < 0) {
        return std::unexpected(EErrorCode::SocketInit);
    }

    if (DataSize < 1) {
        return TData{};
    }

    TData buffer(DataSize, 0);

    if (auto result = recv(Sockfd, buffer.data(), DataSize, MSG_WAITALL); result < 0) {
        return std::unexpected(EErrorCode::Read);
    } else if (result < static_cast<ssize_t>(DataSize)) {
        return TData{};
    }

    return buffer;
}

}
