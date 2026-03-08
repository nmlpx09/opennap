#include "socket.h"

#include <write/errors.h>

#include <arpa/inet.h>
#include <unistd.h>

namespace NWrite {

TSocket::TSocket(
    std::string ip,
    std::uint16_t port
)
: Ip(std::move(ip))
, Port(port) { }

TSocket::~TSocket() {
    close(Sockfd);
}

std::error_code TSocket::Init() noexcept {
    if (Sockfd = socket(AF_INET, SOCK_DGRAM, 0); Sockfd < 0) {
        return make_error_code(EErrorCode::SocketInit);
    }

    SockaddrIn = sockaddr_in {
        .sin_family = AF_INET,
        .sin_port = htons(Port),
        .sin_addr = {
            .s_addr = inet_addr(Ip.c_str())
        },
        .sin_zero = {0}
    };

    return {};
}

std::error_code TSocket::Write(TData&& data) noexcept {
    if (Sockfd < 0) {
        return EErrorCode::SocketInit;
    }

    auto result = sendto(Sockfd, data.data(), data.size(),  MSG_CONFIRM, reinterpret_cast<const sockaddr*>(&SockaddrIn), sizeof(SockaddrIn));

    if (result != static_cast<ssize_t>(data.size())) {
        return EErrorCode::Write;
    } 

    return {};
}

}
