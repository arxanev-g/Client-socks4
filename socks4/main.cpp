
#include <chrono>
#include "cactus/net/address.h"
#include "socks4.h"
#include <fstream>
#include <iostream>

int main() {
    cactus::Scheduler sched;
    sched.Run([&] {
        try {
            std::vector<Proxy> proxies = {
                {cactus::SocketAddress("cpp-cactus-24.ru", 12000), "prime"},
                {cactus::SocketAddress("4.4.4.4", 80), "prime"},
            };
            cactus::SocketAddress endpoint{"8.8.8.8", 443};
            auto conn = DialProxyChain(proxies, endpoint);
            std::cout << conn->ReadAllToString();
        } catch (...) {
        }
    });
}