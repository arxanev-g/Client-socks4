#include "socks4.h"
#include <netinet/in.h>
#include <array>
#include <cstdint>
#include "cactus/io/view.h"

namespace {

struct Socks4Data {
    uint8_t version;
    uint8_t command;
    uint16_t port;
    uint32_t ip;
} __attribute__((packed));

}  // namespace

void Sock4Connect(cactus::IConn* conn, const cactus::SocketAddress& endpoint,
                  const std::string& user) {
    std::array<Socks4Data, 1> pod;
    pod[0] = {0x04, 0x01, htons(endpoint.GetPort()), endpoint.GetIp()};
    auto view = cactus::View(pod);
    conn->Write(view);
    conn->Write(cactus::View(user.c_str(), user.size() + 1));

    conn->ReadFull(view);

    if (pod[0].version != 0 || pod[0].command != 90) {
        throw std::runtime_error("Connection establishment error");
    }
}

std::unique_ptr<cactus::IConn> DialProxyChain(const std::vector<Proxy>& proxies,
                                              const cactus::SocketAddress& endpoint) {
    if (proxies.empty()) {
        return cactus::DialTCP(endpoint);
    }
    std::unique_ptr<cactus::IConn> conn = cactus::DialTCP(proxies.front().proxy_address);
    for (size_t i = 1; i < proxies.size(); ++i) {
        Sock4Connect(conn.get(), proxies[i].proxy_address, proxies[i - 1].username);
    }
    Sock4Connect(conn.get(), endpoint, proxies.back().username);
    return conn;
}